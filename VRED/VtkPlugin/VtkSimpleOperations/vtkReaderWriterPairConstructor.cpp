#include "StdAfx.h"
#include "vtkReaderWriterPairConstructor.h"

#include <vtkStructuredGrid.h>
#include <vtkUnstructuredGrid.h>
#include <vtkStructuredPoints.h>

#include <vtkDataReader.h>
#include <vtkStructuredGridReader.h>
#include <vtkUnstructuredGridReader.h>
#include <vtkStructuredPointsReader.h>

#include <vtkXMLReader.h>
#include <vtkXMLUnstructuredGridReader.h>
#include <vtkXMLStructuredGridReader.h>

#include <vtkDataWriter.h>
#include <vtkStructuredGridWriter.h>
#include <vtkUnstructuredGridWriter.h>
#include <vtkStructuredPointsWriter.h>
#include <vtkXMLWriter.h>
#include <vtkXMLStructuredGridWriter.h>
#include <vtkXMLUnstructuredGridWriter.h>

#include <vtkPointData.h>
#include <vtkDataArray.h>

#include <vtkDataSet.h>


CVtkReaderWriterPairConstructor::CVtkReaderWriterPairConstructor(void){
	m_pvtkDataReader = NULL;
	m_pvtkXMLReader = NULL;
	m_pvtkDataWriter = NULL;
	m_pvtkXMLWriter = NULL;
	m_pvtkDataSet = NULL;
}

CVtkReaderWriterPairConstructor::~CVtkReaderWriterPairConstructor(void){
	if (m_pvtkDataReader != NULL) m_pvtkDataReader->Delete();
	if (m_pvtkXMLReader != NULL) m_pvtkXMLReader->Delete();
	if (m_pvtkDataSet != NULL) m_pvtkDataSet->Delete();
	if (m_pvtkDataWriter != NULL) m_pvtkDataWriter->Delete();
	if (m_pvtkXMLWriter != NULL) m_pvtkXMLWriter->Delete();
}

bool CVtkReaderWriterPairConstructor::LoadFile(std::string filename, bool bForceUseXMLWriter){
	ClearAll();

	bool bReturnVal = false;
	vtkDataReader *helpReader = vtkDataReader::New();
	helpReader->SetFileName(filename.c_str());

	if (helpReader->IsFileStructuredGrid()){
		m_pvtkDataReader = vtkStructuredGridReader::New();
		m_pvtkDataReader->SetFileName(filename.c_str());
		m_pvtkDataReader->ReadAllScalarsOn();
		m_pvtkDataReader->Update();
		m_pvtkDataSet = ((vtkStructuredGridReader*) m_pvtkDataReader)->GetOutput();
		if (bForceUseXMLWriter){
			m_pvtkXMLWriter = vtkXMLStructuredGridWriter::New();
			m_stdstrRequiredSuffix = std::string(".vts");
		} else {
			m_pvtkDataWriter = vtkStructuredGridWriter::New();
			m_stdstrRequiredSuffix = std::string(".vtk");
		}
		bReturnVal = true;
	} else if (helpReader->IsFileUnstructuredGrid()){
		m_pvtkDataReader = vtkUnstructuredGridReader::New();
		m_pvtkDataReader->SetFileName(filename.c_str());
		m_pvtkDataReader->ReadAllScalarsOn();
		m_pvtkDataReader->Update();
		m_pvtkDataSet = ((vtkUnstructuredGridReader*) m_pvtkDataReader)->GetOutput();
		if (bForceUseXMLWriter){
			m_pvtkXMLWriter = vtkXMLUnstructuredGridWriter::New();
			m_stdstrRequiredSuffix = std::string(".vtu");
		} else {
			m_pvtkDataWriter = vtkUnstructuredGridWriter::New();
			m_stdstrRequiredSuffix = std::string(".vtk");
		}
		bReturnVal = true;
	} else if (helpReader->IsFileStructuredPoints()){
		m_pvtkDataReader = vtkStructuredPointsReader::New();
		m_pvtkDataReader->SetFileName(filename.c_str());
		m_pvtkDataReader->ReadAllScalarsOn();
		m_pvtkDataReader->Update();
		m_pvtkDataSet = ((vtkStructuredPointsReader*) m_pvtkDataReader)->GetOutput();
		//does not exist, may be vtkXMLImageWriter ist the right object?
		//if (bForceUseXMLWriter){
		//	m_pvtkXMLWriter = vtkXMLStructuredPointsWriter::New();
		//} else {
			m_pvtkDataWriter = vtkStructuredPointsWriter::New();
			m_stdstrRequiredSuffix = std::string(".vtk");
		//}
		bReturnVal = true;
	} else {
		std::cout << "File type is not supported" << std::endl;
		bReturnVal = false;
	}

	return bReturnVal;
}

bool CVtkReaderWriterPairConstructor::WriteFile(std::string filename){
	if (m_pvtkDataWriter != NULL){
		m_pvtkDataWriter->SetFileName(filename.c_str());
		m_pvtkDataWriter->Write();
		return true;
	}
	if (m_pvtkXMLWriter != NULL){
		m_pvtkXMLWriter->SetFileName(filename.c_str());
		m_pvtkXMLWriter->Write();
		return true;
	}
	return false;
}

std::string CVtkReaderWriterPairConstructor::GetCorrectFileSuffix(){
	return m_stdstrRequiredSuffix;
}

std::list<std::string>	CVtkReaderWriterPairConstructor::GetAttributeNames(){
	std::list<std::string> attributeNames;
	if (m_pvtkDataSet == NULL) return attributeNames;

	vtkPointData *pointData = m_pvtkDataSet->GetPointData();
	int numArrays = pointData->GetNumberOfArrays();
	for (int i=0; i<numArrays; i++){
		std::string newAttribute(pointData->GetArray(i)->GetName());
		attributeNames.push_back(newAttribute);
	}

	return attributeNames;
}

std::list<std::string>	CVtkReaderWriterPairConstructor::GetScalarAttributeNames(){
	std::list<std::string> attributeNames;
	if (m_pvtkDataSet == NULL) return attributeNames;

	vtkPointData *pointData = m_pvtkDataSet->GetPointData();
	int numArrays = pointData->GetNumberOfArrays();
	for (int i=0; i<numArrays; i++){
		vtkDataArray *dataArray = pointData->GetArray(i);
		int numComponents = dataArray->GetNumberOfComponents();
		if(numComponents == 1){
			std::string newAttribute(dataArray->GetName());
			attributeNames.push_back(newAttribute);
		}
	}

	return attributeNames;
}

std::list<std::string>	CVtkReaderWriterPairConstructor::GetVectorAttributeNames(){
	std::list<std::string> attributeNames;
	if (m_pvtkDataSet == NULL) return attributeNames;

	vtkPointData *pointData = m_pvtkDataSet->GetPointData();
	int numArrays = pointData->GetNumberOfArrays();
	for (int i=0; i<numArrays; i++){
		vtkDataArray *dataArray = pointData->GetArray(i);
		int numComponents = dataArray->GetNumberOfComponents();
		if(numComponents == 3){
			std::string newAttribute(dataArray->GetName());
			attributeNames.push_back(newAttribute);
		}
	}

	return attributeNames;
}

std::list<std::string>	CVtkReaderWriterPairConstructor::GetTensorAttributeNames(){
	std::list<std::string> attributeNames;
	if (m_pvtkDataSet == NULL) return attributeNames;

	vtkPointData *pointData = m_pvtkDataSet->GetPointData();
	int numArrays = pointData->GetNumberOfArrays();
	for (int i=0; i<numArrays; i++){
		vtkDataArray *dataArray = pointData->GetArray(i);
		int numComponents = dataArray->GetNumberOfComponents();
		if(numComponents == 9){
			std::string newAttribute(dataArray->GetName());
			attributeNames.push_back(newAttribute);
		}
	}

	return attributeNames;
}

vtkDataSet *CVtkReaderWriterPairConstructor::GetReaderOutput(){
	return m_pvtkDataSet;
}

bool CVtkReaderWriterPairConstructor::SetWriterInput(vtkDataSet* dataSet){
	if (m_pvtkDataWriter != NULL){
		m_pvtkDataWriter->SetInput(dataSet);
	} else if (m_pvtkXMLWriter != NULL){
		m_pvtkXMLWriter->SetInput(dataSet);
	} else {
		return false;
	}
	return true;
}

void CVtkReaderWriterPairConstructor::ClearAll(){
	if (m_pvtkDataReader != NULL){
		m_pvtkDataReader->Delete();
		m_pvtkDataReader = NULL;
	}

	if (m_pvtkDataWriter != NULL){
		m_pvtkDataWriter->Delete();
		m_pvtkDataWriter = NULL;
	};

	if (m_pvtkXMLWriter != NULL){
		m_pvtkXMLWriter->Delete();
		m_pvtkXMLWriter = NULL;
	}

	if (m_pvtkDataSet != NULL){
		//does not need to (can not) be deleted as it is a child of the reader ???
		//m_pvtkDataSet->Delete();
		m_pvtkDataSet = NULL;
	}

	if (m_pvtkXMLReader != NULL){
		m_pvtkXMLReader->Delete();
		m_pvtkXMLReader = NULL;
	}
}