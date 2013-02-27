#include "StdAfx.h"
#include "VtkMoveDataFromCellToPoint.h"
#include "vtkCellDataToPointData.h"

CVtkMoveDataFromCellToPoint::CVtkMoveDataFromCellToPoint(void){
	m_pvtkCellDataToPointData = NULL;
}

CVtkMoveDataFromCellToPoint::~CVtkMoveDataFromCellToPoint(void)
{
	if (m_pvtkCellDataToPointData != NULL){
		m_pvtkCellDataToPointData->Delete();
	}
}

bool CVtkMoveDataFromCellToPoint::LoadFile(std::string filename){
	if (CVtkReaderWriterPairConstructor::LoadFile(filename)){
	} else {
		std::cout << "CVtkMoveDataFromCellToPoint::LoadFile(...) could not load file" << std::endl;
	}

	if (m_pvtkDataSet == NULL) return false;

	if (m_pvtkCellDataToPointData == NULL){
		m_pvtkCellDataToPointData = vtkCellDataToPointData::New();
	}

	m_pvtkCellDataToPointData->SetInput(m_pvtkDataSet);
	SetWriterInput(m_pvtkCellDataToPointData->GetOutput());

	return true;
}
