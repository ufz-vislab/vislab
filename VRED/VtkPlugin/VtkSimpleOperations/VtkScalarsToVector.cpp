#include "StdAfx.h"
#include "VtkScalarsToVector.h"

#include <vtkMergeFields.h>

CVtkScalarsToVector::CVtkScalarsToVector(void){
	m_pvtkMergeFields = NULL;
}

CVtkScalarsToVector::~CVtkScalarsToVector(void)
{
}

bool CVtkScalarsToVector::LoadFile(std::string filename){
	if (CVtkReaderWriterPairConstructor::LoadFile(filename)){
	} else {
		std::cout << "CVtkScalarsToTensor::LoadFile(...) could not load file" << std::endl;
	}

	if (m_pvtkDataSet == NULL) return false;

	if (m_pvtkMergeFields == NULL){
		m_pvtkMergeFields = vtkMergeFields::New();
	}

	m_pvtkMergeFields->SetInput(m_pvtkDataSet);
	SetWriterInput(m_pvtkMergeFields->GetOutput());

	return true;
}

bool CVtkScalarsToVector::SetVectorComponents(std::string newCompName, std::list<std::string> components){
	if (components.size() != 3) return false;
	if (m_pvtkDataSet == NULL) return false;
	if (m_pvtkMergeFields == NULL) return false;

	m_pvtkMergeFields->SetOutputField(newCompName.c_str(), vtkMergeFields::POINT_DATA);
	m_pvtkMergeFields->SetNumberOfComponents(3);

	int idx = 0;
	std::list<std::string>::iterator compsIter;
	for (compsIter = components.begin(); compsIter != components.end(); compsIter++){
		m_pvtkMergeFields->Merge(idx, (*compsIter).c_str(), 0);
		idx++;
	}
	return true;
}