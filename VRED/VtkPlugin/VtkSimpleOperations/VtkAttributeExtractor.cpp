#include "StdAfx.h"
#include "VtkAttributeExtractor.h"

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

#include <set>

CVtkAttributeExtractor::CVtkAttributeExtractor(void){
}

CVtkAttributeExtractor::~CVtkAttributeExtractor(void){
}

bool CVtkAttributeExtractor::ExtractAttributes(std::string outFilename, std::list<std::string> attributeNames){
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

	std::set<std::string> attributesToKeep;
	std::set<std::string>::iterator attributesToKeepIterator;

	std::list<std::string>::iterator attrListIter;
	for (attrListIter = attributeNames.begin(); attrListIter != attributeNames.end(); attrListIter++){
		attributesToKeep.insert(*attrListIter);
	}

	std::list<std::string> attributesToDelete;

	vtkPointData *pointData = m_pvtkDataSet->GetPointData();
	int numPoints = pointData->GetNumberOfArrays();
	for (int i=0; i<numPoints; i++){
		std::string arrayName = std::string(pointData->GetArray(i)->GetName());
		attributesToKeepIterator = attributesToKeep.find(arrayName);
		if (attributesToKeepIterator == attributesToKeep.end()){
			attributesToDelete.push_back(arrayName);
		}
	}

	for (attrListIter = attributesToDelete.begin(); attrListIter != attributesToDelete.end(); attrListIter++){
		pointData->RemoveArray((*attrListIter).c_str());
	}

	this->SetWriterInput(m_pvtkDataSet);
	this->WriteFile(outFilename);

	return true;
}
