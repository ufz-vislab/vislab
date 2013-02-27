///////////////////////////////////////////////////////////
//  TensorGrid.cpp
//  Implementation of the Class CTensorGrid
//  Created on:      03-Sep-2008 11:48:43
///////////////////////////////////////////////////////////

#include "TensorGrid.h"

#include "vtkDataReader.h"
#include "vtkXMLReader.h"
#include "vtkDataSet.h"
#include "vtkMaskPoints.h"
#include "vtkTensorGlyph.h"
#include "vtkOsgActor.h"
#include "vtkHyperStreamline.h"
#include "vtkSphereSource.h"
#include "vtkArrowSource.h"
#include "vtkAxes.h"
#include "vtkTubeFilter.h"
#include "vtkPolyDataNormals.h"
#include "vtkPolyDataMapper.h"

#include "vtkUnstructuredGridReader.h"
#include "vtkStructuredGridReader.h"
#include "vtkStructuredPointsReader.h"
#include "vtkXMLUnstructuredGridReader.h"
#include "vtkXMLStructuredGridReader.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"

#include <OpenSG/OSGGroup.h>
#include <OpenSG/OSGSimpleAttachments.h>

using namespace osg;

CTensorGrid::CTensorGrid(){
	m_bNewOsgObjectsCreated = false;
	
	m_posgRoot = makeCoredNode<Group>();
	setName(m_posgRoot, "TensorGrid");
	
	m_posgGlyphsRoot = NullFC;
	
	m_pvtkDataReader = NULL;
	m_pvtkXMLReader = NULL;
	m_pvtkDataSet = NULL;
	m_pvtkMaskPointsForGlyphs = NULL;
	m_pvtkSphereSource = NULL;
	m_pvtkAxes = NULL;
	m_pvtkTensorGlyphs = NULL;
	m_pGlyphActor = NULL;
	m_pvtkHyperStreamline1 = NULL;
	m_pHyperStreamline1Actor = NULL;
	m_pvtkHyperStreamline2 = NULL;
	m_pHyperStreamline2Actor = NULL;
}



CTensorGrid::~CTensorGrid(){

}


bool CTensorGrid::GetNewOsgObjectsCreated(){
	return m_bNewOsgObjectsCreated;
}
	
OSG::NodePtr CTensorGrid::GetOpenSGRoot(){
	return m_posgRoot;
}


/**
 * Function loads data from the VTK file in several formats and sets up the
 * necessary pipelines
 */
bool CTensorGrid::SetFilename( const char* filename){
	std::string strfilename(filename);
	
	std::string basename, suffix, tmpname;
	std::string::size_type pos = strfilename.rfind(".");
	if (pos != std::string::npos){
		basename = strfilename.substr(0, pos);
		suffix = strfilename.substr(pos);
	} else {
		return false;
	}
	
	if (suffix == std::string(".vts")){
		std::cout << "attempt to read structured grid from xml file" << std::endl;
	} else if (suffix == std::string(".vtu")){
		std::cout << "attempt to read unstructured grid form xml file" << std::endl;
		m_pvtkXMLReader = vtkXMLUnstructuredGridReader::New();
		m_pvtkXMLReader->SetFileName(filename);
		m_pvtkXMLReader->Update();
		m_pvtkDataSet = (vtkDataSet*) m_pvtkXMLReader->GetOutputAsDataSet();
		std::cout << ".... done" << std::endl;
	} else if (suffix == std::string(".vtk")){
		std::cout << "attempt to read grid from legacy vtk file" << std::endl;	
		vtkDataReader *helpReader = vtkDataReader::New();
		helpReader->SetFileName(filename);
	
		if (helpReader->IsFileStructuredGrid()){
			std::cout << "file contains structured grid" << std::endl;
			m_pvtkDataReader = vtkStructuredGridReader::New();
			m_pvtkDataReader->SetFileName(filename);
			m_pvtkDataReader->Update();
			m_pvtkDataSet = (vtkDataSet*) ((vtkStructuredGridReader*) m_pvtkDataReader)->GetOutput();
		} else if (helpReader->IsFileUnstructuredGrid()){
			std::cout << "file contains unstructured grid" << std::endl;
			m_pvtkDataReader = vtkUnstructuredGridReader::New();
			m_pvtkDataReader->SetFileName(filename);
			m_pvtkDataReader->Update();
			m_pvtkDataSet = (vtkDataSet*) ((vtkUnstructuredGridReader*) m_pvtkDataReader)->GetOutput();
		} else if (helpReader->IsFileStructuredPoints()){
			std::cout << "file contains structured points" << std::endl;
			m_pvtkDataReader = vtkStructuredPointsReader::New();
			m_pvtkDataReader->SetFileName(filename);
			m_pvtkDataReader->Update();
			m_pvtkDataSet = (vtkDataSet*) ((vtkStructuredPointsReader*) m_pvtkDataReader)->GetOutput();
		} else {
			return false;
		}
	} else {
		std::cout << "CVectorGrid: could not read file" << filename << std::endl;
		return false;
	}

	// Checking if we have an active tensor in the data. If not we try to find one.
	// If we can not find one we stop loading and return false
	vtkPointData *pointData = m_pvtkDataSet->GetPointData();
	vtkDataArray *tensorArray = pointData->GetTensors();
	if (tensorArray == NULL){
		int numArrays = pointData->GetNumberOfArrays();
		for (int i=0; i<numArrays; i++){
			vtkDataArray *anArray = pointData->GetArray(i);
			const char *arrayName = anArray->GetName();
			int numComponents = anArray->GetNumberOfComponents();
			std::cout << "	" << arrayName << " " << numComponents << std::endl;
			//Why does this not work and crashes?
			//vtkAbstractArray *anAbstractArray = (vtkAbstractArray*) anArray;
			//int arrayIsNumeric = anAbstractArray->IsNumeric();
			if ((numComponents == 9)){
				pointData->SetActiveTensors(arrayName);
				tensorArray = anArray;
				break;
			}
		}
		if (tensorArray == NULL) return false;
	} else {
		std::cout << "got a tensor data array with name: " << tensorArray->GetName() << std::endl;
	}
	
	// Retreving some resonable values for the initial state settings
	m_state.m_stdstrActiveTensorName = std::string(pointData->GetTensors()->GetName());
	m_state.m_stdstrActiveScalarName = std::string(pointData->GetScalars()->GetName());
		
	double bounds[6];
	m_pvtkDataSet->Update();
	m_pvtkDataSet->GetBounds(bounds);
	m_state.m_dXMin = bounds[0];
	m_state.m_dXMax = bounds[1];
	m_state.m_dYMin = bounds[2];
	m_state.m_dYMax = bounds[3];
	m_state.m_dZMin = bounds[4];
	m_state.m_dZMax = bounds[5];
	
	double sidelength = bounds[1] + bounds[3] + bounds[5] - bounds[0] - bounds[2] - bounds[4];
	m_state.m_iNumPoints = m_pvtkDataSet->GetNumberOfPoints();
	if (m_state.m_iNumPoints > 1000){
		m_state.m_iGlyphMaskRatio = (int) m_state.m_iNumPoints / 1000;
	}
	
	return InitGlyphPipeline();
}


/**
 * Comparing the state to the local m_state variable and performing the necessary
 * actions and changes
 */
void CTensorGrid::SetState(CTensorGridState state){
	if (m_pvtkDataSet == NULL) return;
	
	bool bRecomputeGlyphs = false;
	
	if (m_state.m_bShowGlyphs != state.m_bShowGlyphs){
		m_state.m_bShowGlyphs = state.m_bShowGlyphs;
		beginEditCP(m_posgGlyphsRoot, Node::TravMaskFieldMask);{
			if (m_state.m_bShowGlyphs){
				m_posgGlyphsRoot->setTravMask(1);
			} else {
				m_posgGlyphsRoot->setTravMask(0);
			}
		};endEditCP(m_posgGlyphsRoot, Node::TravMaskFieldMask);
	}
	
	if (m_state.m_iGlyphMaskRatio != state.m_iGlyphMaskRatio){
		m_state.m_iGlyphMaskRatio = state.m_iGlyphMaskRatio;
		m_pvtkMaskPointsForGlyphs->SetOnRatio(m_state.m_iGlyphMaskRatio);
		bRecomputeGlyphs = true;
	}
	
	if (m_state.m_bUseDeformedSpheres != state.m_bUseDeformedSpheres){
		m_state.m_bUseDeformedSpheres = state.m_bUseDeformedSpheres;
		if (m_state.m_bUseDeformedSpheres){
			m_pvtkTensorGlyphs->SetSource(m_pvtkSphereSource->GetOutput());
		} else {
			m_pvtkTensorGlyphs->SetSource(m_pvtkAxes->GetOutput());
		}
		bRecomputeGlyphs = true;
	}
	
	if (m_state.m_bColorByEigenvalues != state.m_bColorByEigenvalues){
		m_state.m_bColorByEigenvalues = state.m_bColorByEigenvalues;
		if (m_state.m_bColorByEigenvalues){
			m_pvtkTensorGlyphs->SetColorModeToEigenvalues();
		} else {
			m_pvtkTensorGlyphs->SetColorModeToScalars();
		}
		bRecomputeGlyphs = true;
	}
	
	if (m_state.m_dGlyphScaleFactor != state.m_dGlyphScaleFactor){
		m_state.m_dGlyphScaleFactor = state.m_dGlyphScaleFactor;
		m_pvtkTensorGlyphs->SetScaleFactor(m_state.m_dGlyphScaleFactor);
		bRecomputeGlyphs = true;
	}
	
	if (bRecomputeGlyphs){
		m_pGlyphActor->UpdateOsg();
	}	
}


/**
 * Return the current state, stored in m_state
 */
CTensorGridState CTensorGrid::GetState(){

	return  m_state;
}


/**
 * Set up the pipeline to represent the tensors as tensor-glyphs
 */
bool CTensorGrid::InitGlyphPipeline(){
	if (m_pvtkDataSet == NULL) return false;
	
	m_pvtkMaskPointsForGlyphs = vtkMaskPoints::New();
	m_pvtkMaskPointsForGlyphs->SetInput(m_pvtkDataSet);
	m_pvtkMaskPointsForGlyphs->SetOnRatio(m_state.m_iGlyphMaskRatio);
	m_pvtkMaskPointsForGlyphs->RandomModeOn();
	
	m_pvtkSphereSource = vtkSphereSource::New();
	m_pvtkAxes = vtkAxes::New();
	
	m_pvtkTensorGlyphs = vtkTensorGlyph::New();
	m_pvtkTensorGlyphs->SetInput(m_pvtkMaskPointsForGlyphs->GetOutput());
	m_pvtkTensorGlyphs->SetSource(m_pvtkSphereSource->GetOutput());
	m_pvtkTensorGlyphs->SetScaleFactor(1.0);
	m_pvtkTensorGlyphs->ClampScalingOn();
	
	vtkPolyDataNormals *normals = vtkPolyDataNormals::New();
	normals->SetInput(m_pvtkTensorGlyphs->GetOutput());
	
	vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
	mapper->SetInput(normals->GetOutput());
	
	m_pGlyphActor = vtkOsgActor::New();
	m_pGlyphActor->SetMapper(mapper);
	m_pGlyphActor->UpdateOsg();
	
	m_posgGlyphsRoot = m_pGlyphActor->GetOsgRoot();
	setName(m_posgGlyphsRoot, "TensorGlyphs");
	
	beginEditCP(m_posgRoot, Node::ChildrenFieldMask);{
		m_posgRoot->addChild(m_posgGlyphsRoot);
	};endEditCP(m_posgRoot, Node::ChildrenFieldMask);
	
	return true;
}


/**
 * Setting up the pipeline to 
 */
bool CTensorGrid::InitHyperstreamlinePipeline(){
	return false;
}