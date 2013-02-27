///////////////////////////////////////////////////////////
//  VtkDataSetLoader.cpp
//  Implementation of the Class CVtkDataSetLoader
//  Created on:      24-Sep-2008 17:28:46
///////////////////////////////////////////////////////////

#include "VtkDataSetLoader.h"

#include "vtkDataReader.h"
#include "vtkXMLReader.h"
#include "vtkDataSet.h"
#include "vtkDataObject.h"

#include "vtkUnstructuredGridReader.h"
#include "vtkStructuredGridReader.h"
#include "vtkStructuredPointsReader.h"
#include "vtkXMLUnstructuredGridReader.h"
#include "vtkXMLStructuredGridReader.h"
#include "vtkPointData.h"
#include "vtkCellData.h"
#include "vtkDataArray.h"

CVtkDataSetLoader::CVtkDataSetLoader(){
	m_pvtkDataReader = NULL;
	m_pvtkXMLReader = NULL;
	
	m_bNewOsgObjectsCreated = false;
}



CVtkDataSetLoader::~CVtkDataSetLoader(){

}


bool CVtkDataSetLoader::SetFilename(const char* filename){
	std::string strfilename(filename);
	
	std::string basename, suffix, tmpname;
	std::string::size_type pos = strfilename.rfind(".");
	if (pos != std::string::npos){
		basename = strfilename.substr(0, pos);
		suffix = strfilename.substr(pos);
	} else {
		return false;
	}
	
	vtkDataSet *dataSet;
	
	if (suffix == std::string(".vts")){
		std::cout << "attempt to read structured grid from xml file" << std::endl;
		m_pvtkXMLReader = vtkXMLStructuredGridReader::New();
		m_pvtkXMLReader->SetFileName(filename);
		m_pvtkXMLReader->Update();
		dataSet = (vtkDataSet*) m_pvtkXMLReader->GetOutputAsDataSet();
		std::cout << ".... done" << std::endl;
	} else if (suffix == std::string(".vtu")){
		std::cout << "attempt to read unstructured grid form xml file" << std::endl;
		m_pvtkXMLReader = vtkXMLUnstructuredGridReader::New();
		m_pvtkXMLReader->SetFileName(filename);
		m_pvtkXMLReader->Update();
		dataSet = (vtkDataSet*) m_pvtkXMLReader->GetOutputAsDataSet();
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
			dataSet = (vtkDataSet*) ((vtkStructuredGridReader*) m_pvtkDataReader)->GetOutput();
			std::cout << "... done" << std::endl;
		} else if (helpReader->IsFileUnstructuredGrid()){
			std::cout << "file contains unstructured grid" << std::endl;
			m_pvtkDataReader = vtkUnstructuredGridReader::New();
			m_pvtkDataReader->SetFileName(filename);
			m_pvtkDataReader->Update();
			dataSet = (vtkDataSet*) ((vtkUnstructuredGridReader*) m_pvtkDataReader)->GetOutput();
			std::cout << "... done" << std::endl;
		} else if (helpReader->IsFileStructuredPoints()){
			std::cout << "file contains structured points" << std::endl;
			m_pvtkDataReader = vtkStructuredPointsReader::New();
			m_pvtkDataReader->SetFileName(filename);
			m_pvtkDataReader->Update();
			dataSet = (vtkDataSet*) ((vtkStructuredPointsReader*) m_pvtkDataReader)->GetOutput();
			std::cout << "... done" << std::endl;
		} else {
			return false;
		}
	} else {
		std::cout << "CVectorGrid: could not read file" << filename << std::endl;
		return false;
	}

	//Retrieving the different attribute names
	vtkPointData *pointData = dataSet->GetPointData();
	vtkCellData *cellData = dataSet->GetCellData();
	
	int i;
	if (pointData != NULL){
		int numPointData = pointData->GetNumberOfArrays();
		for (i=0; i<numPointData; i++){
			vtkDataArray *anArray = pointData->GetArray(i);
			const char *arrayName = anArray->GetName();
			int numComponents = anArray->GetNumberOfComponents();
			//Why does this not work and crashes?
			//vtkAbstractArray *anAbstractArray = (vtkAbstractArray*) anArray;
			//int arrayIsNumeric = anAbstractArray->IsNumeric();
			if (numComponents == 1){
				m_stdstrlistScalarVertexAttributes.push_back(std::string(arrayName));
			} else if (numComponents == 3){
				m_stdstrlistVectorVertexAttributes.push_back(std::string(arrayName));
			} else if (numComponents == 9){
				m_stdstrlistTensorVertexAttributes.push_back(std::string(arrayName));
			} else {
				std::cout << "Attribute: " << arrayName << "with " << numComponents;
				std::cout << " components has unkown type" << std::endl;
			}
		}
	}
	
	if (cellData != NULL){
		int numCellData = cellData->GetNumberOfArrays();
		for (i=0; i<numCellData; i++){
			vtkDataArray *anArray = cellData->GetArray(i);
			const char *arrayName = anArray->GetName();
			int numComponents = anArray->GetNumberOfComponents();
			//Why does this not work and crashes?
			//vtkAbstractArray *anAbstractArray = (vtkAbstractArray*) anArray;
			//int arrayIsNumeric = anAbstractArray->IsNumeric();
			if (numComponents == 1){
				m_stdstrlistScalarCellAttributes.push_back(std::string(arrayName));
			} else if (numComponents == 3){
				m_stdstrlistVectorCellAttributes.push_back(std::string(arrayName));
			} else if (numComponents == 9){
				m_stdstrlistTensorCellAttributes.push_back(std::string(arrayName));
			} else {
				std::cout << "Attribute: " << arrayName << "with " << numComponents;
				std::cout << " components has unkown type" << std::endl;
			}
		}
	}
		
	return true;
}

vtkDataSet* CVtkDataSetLoader::GetDataSet(){
	vtkDataObject *dataObject;
	vtkDataSet *dataSet;
	if (m_pvtkDataReader != NULL){
		dataObject = m_pvtkDataReader->GetOutputDataObject(0);
	} else if (m_pvtkXMLReader != NULL){
		dataObject = m_pvtkXMLReader->GetOutputDataObject(0);
	} else {
		dataObject = NULL;
	}
	
	if (dataObject != NULL){
		dataSet = (vtkDataSet*) dataObject;
	} else {
		dataSet = NULL;
	}
	
	return dataSet;	
}

std::list<std::string> CVtkDataSetLoader::GetScalarVertexAttributes(){
	return  m_stdstrlistScalarVertexAttributes;
}


bool CVtkDataSetLoader::IsScalarVertexAttribute(std::string attrName){
	std::list<std::string>::iterator iter;
	for (iter = m_stdstrlistScalarVertexAttributes.begin(); iter != m_stdstrlistScalarVertexAttributes.end(); iter++){
		if (*iter == attrName) return true;
	}
	return false;
}


std::list<std::string> CVtkDataSetLoader::GetVectorVertexAttributes(){

	return  m_stdstrlistVectorVertexAttributes;
}


bool CVtkDataSetLoader::IsVectorVertexAttribute(std::string attrName){
	std::list<std::string>::iterator iter;
	for (iter = m_stdstrlistVectorVertexAttributes.begin(); iter != m_stdstrlistVectorVertexAttributes.end(); iter++){
		if (*iter == attrName) return true;
	}
	return false;
}


std::list<std::string> CVtkDataSetLoader::GetTensorVertexAttributes(){

	return  m_stdstrlistTensorVertexAttributes;
}


bool CVtkDataSetLoader::IsTensorVertexAttribute(std::string attrName){
	std::list<std::string>::iterator iter;
	for (iter = m_stdstrlistTensorVertexAttributes.begin(); iter != m_stdstrlistTensorVertexAttributes.end(); iter++){
		if (*iter == attrName) return true;
	}
	return false;
}


std::list<std::string> CVtkDataSetLoader::GetScalarCellAttributes(){

	return  m_stdstrlistScalarCellAttributes;
}


bool CVtkDataSetLoader::IsScalarCellAttribute(std::string attrName){
	std::list<std::string>::iterator iter;
	for (iter = m_stdstrlistScalarCellAttributes.begin(); iter != m_stdstrlistScalarCellAttributes.end(); iter++){
		if (*iter == attrName) return true;
	}
	return false;
}


std::list<std::string> CVtkDataSetLoader::GetVectorCellAttributes(){
	return  m_stdstrlistVectorCellAttributes;
}


bool CVtkDataSetLoader::IsVectorCellAttribute(std::string attrName){
	std::list<std::string>::iterator iter;
	for (iter = m_stdstrlistVectorCellAttributes.begin(); iter != m_stdstrlistVectorCellAttributes.end(); iter++){
		if (*iter == attrName) return true;
	}
	return false;
}


std::list<std::string> CVtkDataSetLoader::GetTensorCellAttributes(){

	return  m_stdstrlistTensorCellAttributes;
}


bool CVtkDataSetLoader::IsTensorCellAttribute(std::string attrName){
	std::list<std::string>::iterator iter;
	for (iter = m_stdstrlistTensorCellAttributes.begin(); iter != m_stdstrlistTensorCellAttributes.end(); iter++){
		if (*iter == attrName) return true;
	}
	return false;
}

bool CVtkDataSetLoader::GetNewOsgObjectsCreated(){
	return false;
}

OSG::NodePtr CVtkDataSetLoader::GetOpenSGRoot(){
	return OSG::NullFC;
}