///////////////////////////////////////////////////////////
//  ScalarCellPropGrid.cpp
//  Implementation of the Class CScalarCellPropGrid
//  Created on:      08-Sep-2008 16:23:10
///////////////////////////////////////////////////////////

#include "ScalarCellPropGrid.h"


#include "vtkDataReader.h"
#include "vtkXMLReader.h"
#include "vtkDataSet.h"
#include "vtkShrinkFilter.h"
#include "vtkThreshold.h"
#include "vtkPolyDataMapper.h"
#include "vtkGeometryFilter.h"
#include "vtkOsgActor.h"

#include "vtkStructuredGridReader.h"
#include "vtkUnstructuredGridReader.h"
#include "vtkStructuredPointsReader.h"
#include "vtkXMLStructuredGridReader.h"
#include "vtkXMLUnstructuredGridReader.h"
#include "vtkPolyDataNormals.h"

#include "vtkUnstructuredGrid.h"

#include "vtkCellData.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"

#include <OpenSG/OSGGroup.h>
#include <OpenSG/OSGSimpleAttachments.h>

using namespace osg;

CScalarCellPropGrid::CScalarCellPropGrid(){
	m_posgRoot = makeCoredNode<Group>();
	m_pvtkDataReader = NULL;
	m_pvtkXMLReader = NULL;
	m_pvtkDataSet = NULL;
	m_pvtkShrinkFilter = NULL;
	m_pvtkThreshold = NULL;
	m_pvtkPolyDataMapper = NULL;
	m_pCellActor = NULL;
	
	m_bNewOsgObjectsCreated = false;
}



CScalarCellPropGrid::~CScalarCellPropGrid(){
}

bool CScalarCellPropGrid::GetNewOsgObjectsCreated(){
	return m_bNewOsgObjectsCreated;
}	

NodePtr CScalarCellPropGrid::GetOpenSGRoot(){
	return m_posgRoot;
}



bool CScalarCellPropGrid::SetFilename(const char* filename){
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

	// Checking if we have scalar data related to the cells and getting the attribute names.
	// If we can not find at least one we stop loading and return false
	vtkCellData *cellData = m_pvtkDataSet->GetCellData();
	int numArrays = cellData->GetNumberOfArrays();
	for (int i=0; i<numArrays; i++){
		vtkDataArray *anArray = cellData->GetArray(i);
		const char *arrayName = anArray->GetName();
		int numComponents = anArray->GetNumberOfComponents();
		std::cout << "	" << arrayName << " " << numComponents << std::endl;
		//Why does this not work and crashes?
		//vtkAbstractArray *anAbstractArray = (vtkAbstractArray*) anArray;
		//int arrayIsNumeric = anAbstractArray->IsNumeric();
		if ((numComponents == 1)){
			m_stdlistScalarCellAttributes.push_back(std::string(arrayName));
		}
	}
	
	if (m_stdlistScalarCellAttributes.size() == 0) return false;
	
	m_state.m_stdstrCurrentAttribute = *(m_stdlistScalarCellAttributes.begin());
	cellData->SetActiveScalars(m_state.m_stdstrCurrentAttribute.c_str());
	
	SetProperStateValsForCurrentAttribute();
	InitPipeline();
	return true;
}


void CScalarCellPropGrid::SetState(CScalarCellPropGridState state){
	if (m_pvtkDataSet == NULL) return;
	
	bool bCellsChanged = false;
	
	if (m_state.m_stdstrCurrentAttribute != state.m_stdstrCurrentAttribute){
	}
	
	if (m_state.m_dShrinkFactor != state.m_dShrinkFactor){
		m_state.m_dShrinkFactor = state.m_dShrinkFactor;
		m_pvtkShrinkFilter->SetShrinkFactor(m_state.m_dShrinkFactor);
		bCellsChanged = true;
	}
	
	if (m_state.m_dLowerThreshold != state.m_dLowerThreshold){
		m_state.m_dLowerThreshold = state.m_dLowerThreshold;
		m_pvtkThreshold->ThresholdBetween(m_state.m_dLowerThreshold, m_state.m_dUpperThreshold);
		bCellsChanged = true;
	}
	
	if (m_state.m_dUpperThreshold != state.m_dUpperThreshold){
		m_state.m_dUpperThreshold = state.m_dUpperThreshold;
		m_pvtkThreshold->ThresholdBetween(m_state.m_dLowerThreshold, m_state.m_dUpperThreshold);
		bCellsChanged = true;
	}
	
	if (bCellsChanged){
		m_pCellActor->UpdateOsg();
	}		 
}


CScalarCellPropGridState CScalarCellPropGrid::GetState(){

	return  m_state;
}


bool CScalarCellPropGrid::SetProperStateValsForCurrentAttribute(){
	vtkPointData *pointData = m_pvtkDataSet->GetPointData();
	vtkCellData *cellData = m_pvtkDataSet->GetCellData();

	//Extends of the dataset
	double bounds[6];
	m_pvtkDataSet->Update();
	m_pvtkDataSet->GetBounds(bounds);
	m_state.m_dXMin = bounds[0];
	m_state.m_dXMax = bounds[1];
	m_state.m_dYMin = bounds[2];
	m_state.m_dYMax = bounds[3];
	m_state.m_dZMin = bounds[4];
	m_state.m_dZMax = bounds[5];
	
	//Range of the chosen data array
	vtkDataArray *dataArray = cellData->GetScalars(m_state.m_stdstrCurrentAttribute.c_str());
	if (dataArray != NULL){
		double range[2];
		dataArray->GetRange(range);
		m_state.m_dCurrentAttributeMin = m_state.m_dLowerThreshold = range[0];
		m_state.m_dCurrentAttributeMax = m_state.m_dUpperThreshold = range[1];
	} else {
		std::cout << "could not get range of active array" << std::endl;
	}
	return true;
}


bool CScalarCellPropGrid::InitPipeline(){
	if (m_pvtkDataSet == NULL) return false;
	
	m_pvtkShrinkFilter = vtkShrinkFilter::New();
	m_pvtkShrinkFilter->SetInput(m_pvtkDataSet);
	m_pvtkShrinkFilter->SetShrinkFactor(0.8);
	
	m_pvtkThreshold = vtkThreshold::New();
	m_pvtkThreshold->SetInput(m_pvtkShrinkFilter->GetOutput());
	m_pvtkThreshold->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_CELLS, m_state.m_stdstrCurrentAttribute.c_str());
	m_pvtkThreshold->ThresholdBetween(m_state.m_dLowerThreshold, m_state.m_dUpperThreshold);
	
	vtkGeometryFilter *geometryFilter = vtkGeometryFilter::New();
	geometryFilter->SetInput(m_pvtkThreshold->GetOutput());
	
	vtkPolyDataNormals *normals = vtkPolyDataNormals::New();
	normals->ComputeCellNormalsOn();
	normals->SetInput(geometryFilter->GetOutput());
	
	m_pvtkPolyDataMapper = vtkPolyDataMapper::New();
	m_pvtkPolyDataMapper->SetInput(normals->GetOutput());
	m_pvtkPolyDataMapper->SetScalarModeToUseCellFieldData();
	m_pvtkPolyDataMapper->SelectColorArray(m_state.m_stdstrCurrentAttribute.c_str());
	m_pvtkPolyDataMapper->SetColorModeToMapScalars();
	m_pvtkPolyDataMapper->SetScalarRange(m_state.m_dCurrentAttributeMin, m_state.m_dCurrentAttributeMax);
	std::cout << "Current attribute: " << m_state.m_stdstrCurrentAttribute.c_str() << std::endl;
	
	m_pCellActor = vtkOsgActor::New();
	m_pCellActor->SetVerbose(true);
	m_pCellActor->SetMapper(m_pvtkPolyDataMapper);
	m_pCellActor->UpdateOsg();
	
	NodePtr propNode = m_pCellActor->GetOsgRoot();
	setName(propNode, "Scalar_Cell_Properties");
	beginEditCP(m_posgRoot, Node::ChildrenFieldMask);{
		m_posgRoot->addChild(propNode);
	};endEditCP(m_posgRoot, Node::ChildrenFieldMask);
	
	return true;
}