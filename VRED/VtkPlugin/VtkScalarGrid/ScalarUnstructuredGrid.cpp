///////////////////////////////////////////////////////////
//  ScalarUnstructuredGrid.cpp
//  Implementation of the Class CScalarUnstructuredGrid
//  Created on:      08-Jul-2008 17:59:57
///////////////////////////////////////////////////////////

#include "ScalarUnstructuredGrid.h"

#include "vtkOsgActor.h"

#include <vtkActor.h>
#include <vtkStructuredPoints.h>
#include <vtkStructuredGrid.h>
#include <vtkUnstructuredGrid.h>
#include <vtkStructuredGridReader.h>
#include <vtkUnstructuredGridReader.h>
#include <vtkStructuredPointsReader.h>
#include <vtkXMLStructuredGridReader.h>
#include <vtkXMLUnstructuredGridReader.h>
#include <vtkStructuredPoints.h>
#include <vtkDataSet.h>
#include <vtkDataSetMapper.h>
#include <vtkPolyDataNormals.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkIdTypeArray.h>
#include <vtkContourFilter.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>
#include <vtkPlane.h>
#include <vtkCutter.h>
#include <vtkCleanPolyData.h>
#include <vtkCellType.h>
#include <vtkCellTypes.h>

#include <vtkLookupTable.h>
#include "vtkLookupTable2D.h"

#include <OpenSG/OSGGroup.h>

OSG_USING_NAMESPACE

CScalarUnstructuredGrid::CScalarUnstructuredGrid(){
	m_pXCutActor = vtkOsgActor::New();
	m_pYCutActor = vtkOsgActor::New();
	m_pZCutActor = vtkOsgActor::New();
	m_pContourActor = vtkOsgActor::New();
	
	//m_pvtkGridReader = vtkUnstructuredGridReader::New();
	//m_pvtkGrid = NULL;
	m_pvtkDataSet = NULL;
	m_pvtkDataReader = NULL;
	m_pvtkXMLReader = NULL;
	
	m_pvtkXCutPlane = NULL;
	m_pvtkYCutPlane = NULL;
	m_pvtkZCutPlane = NULL;
	m_pvtkContourFilter = NULL;
	
	m_pvtkLookupTable = vtkLookupTable2D::New();
	m_pvtkXSectionLookupTable = vtkLookupTable2D::New();
	m_pvtkYSectionLookupTable = vtkLookupTable2D::New();
	m_pvtkZSectionLookupTable = vtkLookupTable2D::New();
	
	m_posgRoot = makeCoredNode<Group>();
	addRefCP(m_posgRoot);
}


CScalarUnstructuredGrid::~CScalarUnstructuredGrid(){
	subRefCP(m_posgRoot);
}



bool CScalarUnstructuredGrid::SetFilename(const char* filename){
	std::string strfilename(filename);
	
	std::string basename, suffix, tmpname;
	std::string::size_type pos = strfilename.rfind(".");
	if (pos != std::string::npos){
		basename = strfilename.substr(0, pos);
		suffix = strfilename.substr(pos);
	} else {
		std::cout << "error in filename" << std::endl;
		return false;
	}
	
	if (suffix == std::string(".vts")){
		std::cout << "attempt to read structured grid from xml file" << std::endl;
		std::cout << "attempt to read unstructured grid form xml file" << std::endl;
		m_pvtkXMLReader = vtkXMLStructuredGridReader::New();
		m_pvtkXMLReader->SetFileName(filename);
		m_pvtkXMLReader->Update();
		m_pvtkDataSet = (vtkDataSet*) m_pvtkXMLReader->GetOutputAsDataSet();
		std::cout << ".... done" << std::endl;
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
			m_pvtkDataReader->ReadAllScalarsOn();
			m_pvtkDataReader->SetFileName(filename);
			m_pvtkDataReader->Update();
			m_pvtkDataSet = (vtkDataSet*) ((vtkStructuredGridReader*) m_pvtkDataReader)->GetOutput();
		} else if (helpReader->IsFileUnstructuredGrid()){
			std::cout << "file contains unstructured grid" << std::endl;
			m_pvtkDataReader = vtkUnstructuredGridReader::New();
			m_pvtkDataReader->ReadAllScalarsOn();
			m_pvtkDataReader->SetFileName(filename);
			m_pvtkDataReader->Update();
			m_pvtkDataSet = (vtkDataSet*) ((vtkUnstructuredGridReader*) m_pvtkDataReader)->GetOutput();
		} else if (helpReader->IsFileStructuredPoints()){
			std::cout << "file contains structured points" << std::endl;
			m_pvtkDataReader = vtkStructuredPointsReader::New();
			m_pvtkDataReader->ReadAllScalarsOn();
			m_pvtkDataReader->SetFileName(filename);
			m_pvtkDataReader->Update();
			m_pvtkDataSet = (vtkDataSet*) ((vtkStructuredPointsReader*) m_pvtkDataReader)->GetOutput();
		} else {
			std::cout << "unsupported legacy vtk format" << std::endl;
			return false;
		}
	} else {
		std::cout << "CVectorGrid: could not read file" << filename << std::endl;
		return false;
	}

	if (m_pvtkDataSet == NULL){
		std::cout << "resulting dataset was NULL" << std::endl;
		return false;
	}

	const char* attrName;
	vtkPointData *pointData = m_pvtkDataSet->GetPointData();
	if (pointData == NULL){
		std::cout << "dataset contains no point data" << std::endl;
		return false;
	}
	
	vtkDataArray *dataArray = pointData->GetScalars(0);
	
	if (dataArray == NULL){
		std::list<std::string> attributeNames = GetAvailableScalars();
		if (attributeNames.size() == 0){
			std::cout << "data set contains no point data" << std::endl;
			return false;
		} else {
			std::list<std::string>::iterator iter;
			iter = attributeNames.begin();
			pointData->SetActiveScalars((*iter).c_str());
			dataArray = pointData->GetScalars();
		}
	}
	
	if (dataArray == NULL){
		std::cout << "Problems with reading data" << std::endl;
		return false;
	}
	
	if (! SetStateForAttributeName(std::string(dataArray->GetName()))){
		std::cout << "Could not set appropriate state" << std::endl;
		return false;
	}
	
	
	/*
	dataArray->GetRange(scalarRange);
	attrName = dataArray->GetName();
	
	m_state.m_stdstrAttributeName = std::string(attrName);
	m_state.m_dValMin = scalarRange[0];
	m_state.m_dValMax = scalarRange[1];
		
	double *bounds = m_pvtkDataSet->GetBounds();
	m_state.m_dXMin = bounds[0];
	m_state.m_dXMax = bounds[1];
	m_state.m_dYMin = bounds[2];
	m_state.m_dYMax = bounds[3];
	m_state.m_dZMin = bounds[4];
	m_state.m_dZMax = bounds[5];
	*/
		
	this->InitContourPipeline();
	this->InitXCutPlanePipeline();
	this->InitYCutPlanePipeline();
	this->InitZCutPlanePipeline();
	
	std::cout << "dataset has been loaded successfully" << std::endl;
		
	return true;
}

bool CScalarUnstructuredGrid::SetStateForAttributeName(std::string attrName){
	vtkPointData *pointData = m_pvtkDataSet->GetPointData();
	if (pointData == NULL) return false;
	
	vtkDataArray *dataArray = pointData->GetScalars(attrName.c_str());
	if (dataArray == NULL) return false;
	
	double scalarRange[2];
	dataArray->GetRange(scalarRange);
	attrName = dataArray->GetName();
	
	m_state.m_stdstrAttributeName = std::string(attrName);
	m_state.m_stdstrColorAttributeName = std::string(attrName);
	m_state.m_dValMin = scalarRange[0];
	m_state.m_dValMax = scalarRange[1];
		
	double *bounds = m_pvtkDataSet->GetBounds();
	m_state.m_dXMin = bounds[0];
	m_state.m_dXMax = bounds[1];
	m_state.m_dYMin = bounds[2];
	m_state.m_dYMax = bounds[3];
	m_state.m_dZMin = bounds[4];
	m_state.m_dZMax = bounds[5];
		
	return true;
}
	
bool CScalarUnstructuredGrid::SetNewContourAttribute(std::string attrName){
	if (m_pvtkContourFilter == NULL){
		std::cout << "CScalarUnstructuredGrid::SetNewContourAttribute() has been called with not initialized pipe!" << std::endl;
		return false;
	}
	
	SetStateForAttributeName(attrName);
	
	std::cout << "Setting input array to process to: " <<  m_state.m_stdstrAttributeName << std::endl;
	m_pvtkContourFilter->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, m_state.m_stdstrAttributeName.c_str());
	double range[2];
	if (m_state.m_iNumContours == 1){
		range[0] = 0.5 * (m_state.m_dValMin + m_state.m_dValMax);
		range[1] = 0.5 * (m_state.m_dValMin + m_state.m_dValMax);		
	} else if (m_state.m_iNumContours > 1){
		range[0] = m_state.m_dValMin;
		range[1] = m_state.m_dValMax;
	}
	
	m_pvtkContourFilter->GenerateValues(m_state.m_iNumContours, range);
	
	if (m_pContourActor != NULL){
		vtkMapper *mapper = m_pContourActor->GetMapper();
		mapper->SelectColorArray(m_state.m_stdstrAttributeName.c_str());
		mapper->SetScalarRange(m_state.m_dValMin, m_state.m_dValMax);
		m_pContourActor->UpdateOsg();
	}
	if (m_pXCutActor != NULL){
		vtkMapper *mapper = m_pXCutActor->GetMapper();
		mapper->SelectColorArray(m_state.m_stdstrAttributeName.c_str());
		mapper->SetScalarRange(m_state.m_dValMin, m_state.m_dValMax);
		m_pXCutActor->UpdateOsg();
	}			
	if (m_pYCutActor != NULL){
		vtkMapper *mapper = m_pYCutActor->GetMapper();
		mapper->SelectColorArray(m_state.m_stdstrAttributeName.c_str());
		mapper->SetScalarRange(m_state.m_dValMin, m_state.m_dValMax);
		m_pYCutActor->UpdateOsg();
	}
	if (m_pZCutActor != NULL){
		vtkMapper *mapper = m_pZCutActor->GetMapper();
		mapper->SelectColorArray(m_state.m_stdstrAttributeName.c_str());
		mapper->SetScalarRange(m_state.m_dValMin, m_state.m_dValMax);
		m_pZCutActor->UpdateOsg();
	}
				
	m_pContourActor->UpdateOsg();
	return true;
}

bool CScalarUnstructuredGrid::SetDataSet(vtkDataSet*, const char *attributeName){
	if (m_pvtkDataSet == NULL) return false;

	double scalarRange[2];
	const char* attrName;
	vtkPointData *pointData = m_pvtkDataSet->GetPointData();
	if (pointData == NULL) return false;
	vtkDataArray *dataArray = pointData->GetScalars();
	if (dataArray == NULL) return false;
	dataArray->GetRange(scalarRange);
	attrName = dataArray->GetName();
	
	m_state.m_stdstrAttributeName = std::string(attrName);
	m_state.m_dValMin = scalarRange[0];
	m_state.m_dValMax = scalarRange[1];
		
	double *bounds = m_pvtkDataSet->GetBounds();
	m_state.m_dXMin = bounds[0];
	m_state.m_dXMax = bounds[1];
	m_state.m_dYMin = bounds[2];
	m_state.m_dYMax = bounds[3];
	m_state.m_dZMin = bounds[4];
	m_state.m_dZMax = bounds[5];
		
	this->InitContourPipeline();
	this->InitXCutPlanePipeline();
	this->InitYCutPlanePipeline();
	this->InitZCutPlanePipeline();
		
	return true;
}


/**
 * Compares the state given to the current state of the visualization. If some of
 * the states attributes are different the necessary functions are called to adapt
 * the visualization to the given state and the current state is set properly.
 */
void CScalarUnstructuredGrid::SetState(CScalarGridState state){
	std::cout << "state:   " << state.m_stdstrColorAttributeName << std::endl;
	std::cout << "m_state: " << m_state.m_stdstrColorAttributeName << std::endl;

	// If No grid has been loaded so far
	if (m_pvtkDataSet == NULL) return;
	
	if (m_state.m_bShowXSection != state.m_bShowXSection){
		m_state.m_bShowXSection = state.m_bShowXSection;
		NodePtr xSectionRoot = m_pXCutActor->GetOsgRoot();
		beginEditCP(xSectionRoot, Node::TravMaskFieldMask);{
			if (m_state.m_bShowXSection){
				xSectionRoot->setTravMask(1);
			}else{
				xSectionRoot->setTravMask(0);
			}
		};endEditCP(xSectionRoot, Node::TravMaskFieldMask);
	}
	
	if (m_state.m_dXSectionPosition != state.m_dXSectionPosition){
		m_state.m_dXSectionPosition = state.m_dXSectionPosition;
		m_pvtkXCutPlane->SetOrigin(m_state.m_dXSectionPosition, 0.5 * (m_state.m_dYMin + m_state.m_dYMax), 0.5 * (m_state.m_dZMin +  m_state.m_dZMax));
		m_pXCutActor->UpdateOsg();
	}
	
	if (m_state.m_bShowYSection != state.m_bShowYSection){
		m_state.m_bShowYSection = state.m_bShowYSection;
		NodePtr ySectionRoot = m_pYCutActor->GetOsgRoot();
		beginEditCP(ySectionRoot, Node::TravMaskFieldMask);{
			if (m_state.m_bShowYSection){
				ySectionRoot->setTravMask(1);
			} else {
				ySectionRoot->setTravMask(0);
			}
		};endEditCP(ySectionRoot, Node::TravMaskFieldMask);
	}
	
	if (m_state.m_dYSectionPosition != state.m_dYSectionPosition){
		m_state.m_dYSectionPosition = state.m_dYSectionPosition;
		m_pvtkYCutPlane->SetOrigin(0.5 * (m_state.m_dXMin + m_state.m_dXMax), m_state.m_dYSectionPosition, 0.5 * (m_state.m_dZMin +  m_state.m_dZMax));
		m_pYCutActor->UpdateOsg();
	}
	
	if (m_state.m_bShowZSection != state.m_bShowZSection){
		m_state.m_bShowZSection = state.m_bShowZSection;
		
		NodePtr zSectionRoot = m_pZCutActor->GetOsgRoot();
		beginEditCP(zSectionRoot, Node::TravMaskFieldMask);{
			if (m_state.m_bShowZSection){
				zSectionRoot->setTravMask(1);
			} else {
				zSectionRoot->setTravMask(0);
			}
		};endEditCP(zSectionRoot, Node::TravMaskFieldMask);
	}
	
	if (m_state.m_dZSectionPosition != state.m_dZSectionPosition){
		m_state.m_dZSectionPosition = state.m_dZSectionPosition;
		m_pvtkZCutPlane->SetOrigin(0.5 * (m_state.m_dXMin + m_state.m_dXMax), 0.5 * (m_state.m_dYMin +  m_state.m_dYMax), m_state.m_dZSectionPosition);
		m_pZCutActor->UpdateOsg();
	}
	
	if (m_state.m_bShowContours != state.m_bShowContours){
		m_state.m_bShowContours = state.m_bShowContours;
		NodePtr contoursRoot = m_pContourActor->GetOsgRoot();
		beginEditCP(contoursRoot, Node::TravMaskFieldMask);{
			if (m_state.m_bShowContours){
				contoursRoot->setTravMask(1);
			} else {
				contoursRoot->setTravMask(0);
			}
		};endEditCP(contoursRoot, Node::TravMaskFieldMask);
	}
	
	if ((m_state.m_iNumContours != state.m_iNumContours) ||
			(m_state.m_dMinContour != state.m_dMinContour) ||
			(m_state.m_dMaxContour != state.m_dMaxContour)){
		m_state.m_iNumContours = state.m_iNumContours;
		m_state.m_dMinContour = state.m_dMinContour;
		m_state.m_dMaxContour = state.m_dMaxContour;
		
		double range[2];
		if (m_state.m_iNumContours == 1){
			range[0] = m_state.m_dMinContour;
			range[1] = m_state.m_dMinContour;
		} else {
			range[0] = m_state.m_dMinContour;
			range[1] = m_state.m_dMaxContour;
		}
		
		m_pvtkContourFilter->GenerateValues(m_state.m_iNumContours, range);
		m_pContourActor->UpdateOsg();
	}
	
	bool bRedoColorMapping = false;
	
	if (m_state.m_bUseSaturationMapping != state.m_bUseSaturationMapping){
		m_state.m_bUseSaturationMapping = state.m_bUseSaturationMapping;
		bRedoColorMapping = true;
	}
	
	if (m_state.m_stdstrColorAttributeName != state.m_stdstrColorAttributeName){
		m_state.m_stdstrColorAttributeName = state.m_stdstrColorAttributeName;
		bRedoColorMapping = true;
	}
	
	if (bRedoColorMapping){
		std::string colorAttributeName;
		if (m_state.m_bUseSaturationMapping){
			colorAttributeName = m_state.m_stdstrAttributeName;
			
			double min, max;
			if (! GetMinMaxForAttribute(m_state.m_stdstrColorAttributeName, min, max)){
				min = 0.0;
				max = 0.0;
			}
		
			m_pvtkLookupTable->SetScalarsForSecondaryData(m_state.m_stdstrColorAttributeName);
			m_pvtkLookupTable->SetMinMax(min, max);
			m_pvtkLookupTable->SetUseSecondaryData(true);
		
			m_pvtkXSectionLookupTable->SetScalarsForSecondaryData(m_state.m_stdstrColorAttributeName);
			m_pvtkXSectionLookupTable->SetMinMax(min, max);
			m_pvtkXSectionLookupTable->SetUseSecondaryData(true);
		
			m_pvtkYSectionLookupTable->SetScalarsForSecondaryData(m_state.m_stdstrColorAttributeName);
			m_pvtkYSectionLookupTable->SetMinMax(min, max);
			m_pvtkYSectionLookupTable->SetUseSecondaryData(true);
		
			m_pvtkZSectionLookupTable->SetScalarsForSecondaryData(m_state.m_stdstrColorAttributeName);
			m_pvtkZSectionLookupTable->SetMinMax(min, max);
			m_pvtkZSectionLookupTable->SetUseSecondaryData(true);
		} else {
			colorAttributeName = m_state.m_stdstrColorAttributeName;
			m_pvtkLookupTable->SetScalarsForSecondaryData(std::string("NONE"));
			m_pvtkXSectionLookupTable->SetScalarsForSecondaryData(std::string("NONE"));
			m_pvtkYSectionLookupTable->SetScalarsForSecondaryData(std::string("NONE"));
			m_pvtkZSectionLookupTable->SetScalarsForSecondaryData(std::string("NONE"));
		}
		
		double colorsRange[2];
		vtkPointData *pointData = m_pvtkDataSet->GetPointData();
		if (pointData != NULL){
			vtkDataArray *dataArray = pointData->GetScalars(colorAttributeName.c_str());
			if (dataArray != NULL){
				dataArray->GetRange(colorsRange);
				std::cout << "setting new values on the mappers" << std::endl;
				if (m_pContourActor != NULL){
					vtkMapper *mapper = m_pContourActor->GetMapper();
					mapper->SelectColorArray(colorAttributeName.c_str());
					mapper->SetScalarRange(colorsRange);
					mapper->Modified();
					m_pContourActor->UpdateOsg();
				}
					
				if (m_pXCutActor != NULL){
					vtkMapper *mapper = m_pXCutActor->GetMapper();
					mapper->SelectColorArray(colorAttributeName.c_str());
					mapper->SetScalarRange(colorsRange);
					mapper->Modified();
					m_pXCutActor->UpdateOsg();
				}
				
				if (m_pYCutActor != NULL){
					vtkMapper *mapper = m_pYCutActor->GetMapper();
					mapper->SelectColorArray(colorAttributeName.c_str());
					mapper->SetScalarRange(colorsRange);
					mapper->Modified();
					m_pYCutActor->UpdateOsg();
				}
				
				if (m_pZCutActor != NULL){
					vtkMapper *mapper = m_pZCutActor->GetMapper();
					mapper->SelectColorArray(colorAttributeName.c_str());
					mapper->SetScalarRange(colorsRange);
					mapper->Modified();
					m_pZCutActor->UpdateOsg();	
				}
			}
		}	
	}
}


/**
 * Return the current state of the visualization (CScalarGridState)
 */
CScalarGridState CScalarUnstructuredGrid::GetState(){
	return  m_state;
}


/**
 * For observers that want to get access to the 3D scenegraph of this subject. If
 * this is not available NullFC is returned.
 */
OSG::NodePtr CScalarUnstructuredGrid::GetOpenSGRoot(){
	return  m_posgRoot;
}

std::list<std::string> CScalarUnstructuredGrid::GetAvailableScalars(){
	std::list<std::string> scalarsNames;
	if (m_pvtkDataSet == NULL){
		return scalarsNames;
	}
	vtkPointData *pointData = m_pvtkDataSet->GetPointData();
	if (pointData == NULL){
		return scalarsNames;
	}
	
	vtkDataArray *dataArray;
	int numArrays = pointData->GetNumberOfArrays();
	std::cout << "-----number of arrays: " << numArrays << std::endl;
	for (int i=0; i<numArrays; i++){
		vtkDataArray *anArray = pointData->GetArray(i);
		const char *arrayName = anArray->GetName();
		int numComponents = anArray->GetNumberOfComponents();
		//Why does this not work and crashes?
		//vtkAbstractArray *anAbstractArray = (vtkAbstractArray*) anArray;
		//int arrayIsNumeric = anAbstractArray->IsNumeric();
		if ((numComponents == 1)){
			std::cout << "pushing back array name: " << arrayName << std::endl;
			scalarsNames.push_back(std::string(arrayName));
		}
	}
	return scalarsNames;
}


/**
 * If not done so far, this function sets up the vtk pipeline for looking at
 * contours, adds the generated geometry to internal OpenSG root and sets the
 * m_bNewOsgObjectsCreated flag to true
 */
bool CScalarUnstructuredGrid::InitContourPipeline(){
	std::cout << "Initing contour pipeline" << std::endl;
	// If No grid has been loaded so far
	if (m_pvtkDataSet == NULL) return false;

	// The pipeline has already been set up
	if (m_pvtkContourFilter != NULL) return true;

	m_pvtkContourFilter = vtkContourFilter::New();
	m_pvtkContourFilter->SetInput(m_pvtkDataSet);
	m_pvtkContourFilter->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS,
		m_state.m_stdstrAttributeName.c_str());
	m_pvtkContourFilter->UseScalarTreeOn();
	
	vtkDataArray *forColorsArray = m_pvtkDataSet->GetPointData()->GetScalars(m_state.m_stdstrColorAttributeName.c_str());
	if (forColorsArray == NULL){
		std::cout << "InitContourPipeline() failed to retrieve array for coloring" << std::endl;
		return false;
	}
	double forColorsRange[2];
	forColorsArray->GetRange(forColorsRange);
	
	double range[2];
	range[0] = m_state.m_dValMin;
	range[1] = m_state.m_dValMax;
	m_pvtkContourFilter->GenerateValues(5, range);

	vtkPolyDataNormals *normalsFilter = vtkPolyDataNormals::New();
	normalsFilter->SetInput(m_pvtkContourFilter->GetOutput());
	
	vtkPolyDataMapper *contourMapper = vtkPolyDataMapper::New();
	m_pvtkLookupTable->SetActiveMapper(contourMapper);
	contourMapper->SetLookupTable(m_pvtkLookupTable);
	contourMapper->SetScalarModeToUsePointFieldData();
	contourMapper->ScalarVisibilityOn();
	contourMapper->SetUseLookupTableScalarRange(0);
	contourMapper->SetColorModeToMapScalars();
	//contourMapper->SelectColorArray(m_state.m_stdstrColorAttributeName.c_str());
	//contourMapper->SetScalarRange(forColorsRange);
	contourMapper->SelectColorArray(m_state.m_stdstrAttributeName.c_str());
	contourMapper->SetScalarRange(range);
	//contourMapper->SetInput(m_pvtkContourFilter->GetOutput());
	contourMapper->SetInput(normalsFilter->GetOutput());
	m_pContourActor->SetMapper(contourMapper);
	
	NodePtr contourRoot = m_pContourActor->GetOsgRoot();
	if (contourRoot != NullFC){
		std::cout << "got a node for the contour root" << std::endl;
	} else {
		std::cout << "got NullFC as contour root" << std::endl;
	}
	
	if (! m_state.m_bShowContours){
		beginEditCP(contourRoot);{
			contourRoot->setTravMask(0);
		};endEditCP(contourRoot);
	}
	
	m_pContourActor->UpdateOsg();
	
	beginEditCP(m_posgRoot, Node::ChildrenFieldMask);{
		m_posgRoot->addChild(contourRoot);
	};endEditCP(m_posgRoot, Node::ChildrenFieldMask);
	
	m_bNewOsgObjectsCreated = true;
	
	return true;
}


/**
 * If not done so far, this function sets up the vtk pipeline for looking at at a
 * section perpendicularl to the x-axis, adds the generated geometry to the
 * internal OpenSG root and sets the m_bNewOsgObjectsCreated flag to true
 */
bool CScalarUnstructuredGrid::InitXCutPlanePipeline(){
	// No grid has been loaded so far
	if (m_pvtkDataSet == NULL) return false;

	// The pipeline has already been set up
	if (m_pvtkXCutPlane != NULL) return true;
	
	// setting up the cut plane along x
	m_pvtkXCutPlane = vtkPlane::New();
	m_pvtkXCutPlane->SetOrigin(0.5 * (m_state.m_dXMin + m_state.m_dXMax), 0.5 * (m_state.m_dYMin + m_state.m_dYMax), 0.5 * (m_state.m_dZMin +  m_state.m_dZMax));
	m_pvtkXCutPlane->SetNormal(1.0, 0.0, 0.0);
	vtkCutter *xCut = vtkCutter::New();
	xCut->SetInput(m_pvtkDataSet);
	xCut->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS,
		m_state.m_stdstrAttributeName.c_str());
	xCut->SetCutFunction(m_pvtkXCutPlane);
	vtkCleanPolyData *xCutCleaner = vtkCleanPolyData::New();
	xCutCleaner->SetInput(xCut->GetOutput());
	vtkPolyDataNormals *polyDataNormals = vtkPolyDataNormals::New();
	polyDataNormals->SetInput(xCutCleaner->GetOutput());
	
	vtkPolyDataMapper *xCutMapper = vtkPolyDataMapper::New();
	m_pvtkXSectionLookupTable->SetActiveMapper(xCutMapper);
	xCutMapper->SetLookupTable(m_pvtkXSectionLookupTable);
	//xCutMapper->SetInput(xCutCleaner->GetOutput());
	xCutMapper->SetInput(polyDataNormals->GetOutput());
	xCutMapper->SetScalarModeToUsePointFieldData();
	xCutMapper->ScalarVisibilityOn();
	xCutMapper->SetUseLookupTableScalarRange(0);
	xCutMapper->SetColorModeToMapScalars();
	xCutMapper->SelectColorArray(m_state.m_stdstrAttributeName.c_str());
	xCutMapper->SetScalarRange(m_state.m_dValMin, m_state.m_dValMax);
	//xCutMapper->SetScalarRange(m_pvtkDataSet->GetPointData()->GetScalars()->GetRange());
	m_pXCutActor->SetMapper(xCutMapper);
	
	NodePtr xSectionRoot = m_pXCutActor->GetOsgRoot();
	if (! m_state.m_bShowXSection){
		beginEditCP(xSectionRoot);{
			xSectionRoot->setTravMask(0);
		};endEditCP(xSectionRoot);
	}
	
	m_pXCutActor->UpdateOsg();
	
	beginEditCP(m_posgRoot, Node::ChildrenFieldMask);{
		m_posgRoot->addChild(xSectionRoot);
	};endEditCP(m_posgRoot, Node::ChildrenFieldMask);
	
	m_bNewOsgObjectsCreated = true;
	
	return true;
}


/**
 * If not done so far, this function sets up the vtk pipeline for looking at at a
 * section perpendicularl to the y-axis, adds the generated geometry to the
 * internal OpenSG root and sets the m_bNewOsgObjectsCreated flag to true
 */
bool CScalarUnstructuredGrid::InitYCutPlanePipeline(){
	// No grid has been loaded so far
	if (m_pvtkDataSet == NULL) return false;

	// The pipeline has already been set up
	if (m_pvtkYCutPlane != NULL) return true;
	
	m_pvtkYCutPlane = vtkPlane::New();
	m_pvtkYCutPlane->SetOrigin(0.5 * (m_state.m_dXMin + m_state.m_dXMax), 0.5 * (m_state.m_dYMin + m_state.m_dYMax), 0.5 * (m_state.m_dZMin +  m_state.m_dZMax));
	m_pvtkYCutPlane->SetNormal(0.0, 1.0, 0.0);
	vtkCutter *yCut = vtkCutter::New();
	yCut->SetInput(m_pvtkDataSet);
	yCut->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS,
		m_state.m_stdstrAttributeName.c_str());
	yCut->SetCutFunction(m_pvtkYCutPlane);
	vtkCleanPolyData *yCutCleaner = vtkCleanPolyData::New();
	yCutCleaner->SetInput(yCut->GetOutput());
	vtkPolyDataNormals *polyDataNormals = vtkPolyDataNormals::New();
	polyDataNormals->SetInput(yCutCleaner->GetOutput());
	
	vtkPolyDataMapper *yCutMapper = vtkPolyDataMapper::New();
	m_pvtkYSectionLookupTable->SetActiveMapper(yCutMapper);
	yCutMapper->SetLookupTable(m_pvtkYSectionLookupTable);
	//yCutMapper->SetInput(yCutCleaner->GetOutput());
	yCutMapper->SetInput(polyDataNormals->GetOutput());
	yCutMapper->SetScalarModeToUsePointFieldData();
	yCutMapper->ScalarVisibilityOn();
	yCutMapper->SetUseLookupTableScalarRange(0);
	yCutMapper->SetColorModeToMapScalars();
	yCutMapper->SelectColorArray(m_state.m_stdstrAttributeName.c_str());
	yCutMapper->SetScalarRange(m_state.m_dValMin, m_state.m_dValMax);
	//yCutMapper->SetScalarRange(m_pvtkDataSet->GetPointData()->GetScalars()->GetRange());
	m_pYCutActor->SetMapper(yCutMapper);
	
	NodePtr ySectionRoot = m_pYCutActor->GetOsgRoot();
	if (! m_state.m_bShowYSection){
		beginEditCP(ySectionRoot);{
			ySectionRoot->setTravMask(0);
		};endEditCP(ySectionRoot);
	}

	m_pYCutActor->UpdateOsg();
	
	beginEditCP(m_posgRoot, Node::ChildrenFieldMask);{
		m_posgRoot->addChild(ySectionRoot);
	};endEditCP(m_posgRoot, Node::ChildrenFieldMask);
	
	m_bNewOsgObjectsCreated = true;
	
	return true;
}


/**
 * If not done so far, this function sets up the vtk pipeline for looking at at a
 * section perpendicularl to the z-axis, adds the generated geometry to the
 * internal OpenSG root and sets the m_bNewOsgObjectsCreated flag to true
 */
bool CScalarUnstructuredGrid::InitZCutPlanePipeline(){
	// No grid has been loaded so far
	if (m_pvtkDataSet == NULL) return false;

	// The pipeline has already been set up
	if (m_pvtkZCutPlane != NULL) return true;
	
	m_pvtkZCutPlane = vtkPlane::New();
	m_pvtkZCutPlane->SetOrigin(0.5 * (m_state.m_dXMin + m_state.m_dXMax), 0.5 * (m_state.m_dYMin + m_state.m_dYMax), 0.5 * (m_state.m_dZMin +  m_state.m_dZMax));
	m_pvtkZCutPlane->SetNormal(0.0, 0.0, 1.0);
	vtkCutter *zCut = vtkCutter::New();
	zCut->SetInput(m_pvtkDataSet);
	zCut->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS,
		m_state.m_stdstrAttributeName.c_str());
	zCut->SetCutFunction(m_pvtkZCutPlane);
	vtkCleanPolyData *zCutCleaner = vtkCleanPolyData::New();
	zCutCleaner->SetInput(zCut->GetOutput());
	vtkPolyDataNormals *polyDataNormals = vtkPolyDataNormals::New();
	polyDataNormals->SetInput(zCutCleaner->GetOutput());
	
	vtkPolyDataMapper *zCutMapper = vtkPolyDataMapper::New();
	m_pvtkZSectionLookupTable->SetActiveMapper(zCutMapper);
	zCutMapper->SetLookupTable(m_pvtkZSectionLookupTable);
	//zCutMapper->SetInput(zCutCleaner->GetOutput());
	zCutMapper->SetInput(polyDataNormals->GetOutput());
	zCutMapper->SetScalarModeToUsePointFieldData();
	zCutMapper->ScalarVisibilityOn();
	zCutMapper->SetUseLookupTableScalarRange(0);
	zCutMapper->SetColorModeToMapScalars();
	zCutMapper->SelectColorArray(m_state.m_stdstrAttributeName.c_str());
	zCutMapper->SetScalarRange(m_state.m_dValMin, m_state.m_dValMax);
	//zCutMapper->SetScalarRange(m_pvtkDataSet->GetPointData()->GetScalars()->GetRange());
	m_pZCutActor->SetMapper(zCutMapper);
	
	NodePtr zSectionRoot = m_pZCutActor->GetOsgRoot();
	if (! m_state.m_bShowZSection){
		beginEditCP(zSectionRoot);{
			zSectionRoot->setTravMask(0);
		};endEditCP(zSectionRoot);
	}
	
	m_pZCutActor->UpdateOsg();
	
	beginEditCP(m_posgRoot, Node::ChildrenFieldMask);{
		m_posgRoot->addChild(zSectionRoot);
	};endEditCP(m_posgRoot, Node::ChildrenFieldMask);
	
	m_bNewOsgObjectsCreated = true;
	
	return true;
}

bool CScalarUnstructuredGrid::GetMinMaxForAttribute(std::string attributeName, double &min, double &max){
	if (m_pvtkDataSet == NULL) return false;
	
	vtkPointData *pointData = m_pvtkDataSet->GetPointData();
	if (pointData == NULL) return false;
	
	vtkDataArray *dataArray = pointData->GetArray(attributeName.c_str());
	if (dataArray == NULL) return false;
	
	double range[2];
	dataArray->GetRange(range);
	min = range[0];
	max = range[1];
	return true;
}
