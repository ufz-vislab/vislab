#include "StdAfx.h"
#include "VtkVectorRescaler.h"

#include <string>

#include <vtkDataSet.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>
#include <vtkDoubleArray.h>

#include <vtkDataReader.h>
#include <vtkXMLReader.h>

#include <vtkStructuredGridReader.h>
#include <vtkUnstructuredGridReader.h>
#include <vtkStructuredPointsReader.h>

#include <vtkStructuredGrid.h>
#include <vtkUnstructuredGrid.h>
#include <vtkStructuredPoints.h>

CVtkVectorRescaler::CVtkVectorRescaler(void){
	m_pvtkDataSet = NULL;
}

CVtkVectorRescaler::~CVtkVectorRescaler(void){
}

bool CVtkVectorRescaler::Rescale(std::string filename, std::string vectorAttrName, double scaleFactor, std::string lengthAttrName){
	if (m_pvtkDataReader != NULL){
		std::string filename = std::string(m_pvtkDataReader->GetFileName());

		this->LoadFile(filename);

		if (m_pvtkDataReader->IsFileStructuredGrid()){
			m_pvtkDataSet = ((vtkStructuredGridReader*) m_pvtkDataReader)->GetOutput();
		} else if (m_pvtkDataReader->IsFileUnstructuredGrid()){
			m_pvtkDataSet = ((vtkUnstructuredGridReader*) m_pvtkDataReader)->GetOutput();
		} else if (m_pvtkDataReader->IsFileStructuredPoints()){
			m_pvtkDataSet = ((vtkStructuredPointsReader*) m_pvtkDataReader)->GetOutput();
		}
	} else if (m_pvtkXMLReader != NULL){
		m_pvtkXMLReader->Update();
		m_pvtkDataSet = m_pvtkXMLReader->GetOutputAsDataSet();
	} else {
		return false;
	}

	vtkPointData *pointData = m_pvtkDataSet->GetPointData();
	vtkDataArray *vectorArray = pointData->GetVectors(vectorAttrName.c_str());
	if (vectorArray == NULL) return false;
	if (3 != vectorArray->GetNumberOfComponents()) return false;

	vtkDoubleArray *vectorLengthArray = NULL;
	if (! lengthAttrName.empty()){
		vectorLengthArray = vtkDoubleArray::New();
		vectorLengthArray->SetName(lengthAttrName.c_str());
	}

	int numTuples = vectorArray->GetNumberOfTuples();
	for (int i=0; i<numTuples; i++){
		double *values = vectorArray->GetTuple3(i);

		double newValues[3];
		newValues[0] = scaleFactor * values[0];
		newValues[1] = scaleFactor * values[1];
		newValues[2] = scaleFactor * values[2];
		vectorArray->SetTuple3(i, newValues[0], newValues[1], newValues[2]);

		if (vectorLengthArray != NULL){
			double length = sqrt(newValues[0] * newValues[0] + newValues[1] * newValues[1] + newValues[2] * newValues[2]);
			vectorLengthArray->InsertNextTuple1(length);
		}
	}

	if (vectorLengthArray != NULL){
		pointData->AddArray(vectorLengthArray);
	}

	this->SetWriterInput(m_pvtkDataSet);
	this->WriteFile(filename);

	return true;
}