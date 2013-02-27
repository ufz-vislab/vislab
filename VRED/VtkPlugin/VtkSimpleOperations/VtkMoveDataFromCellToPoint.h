#pragma once
#include "vtkreaderwriterpairconstructor.h"

class vtkCellDataToPointData;

class CVtkMoveDataFromCellToPoint :
	public CVtkReaderWriterPairConstructor
{
public:
	CVtkMoveDataFromCellToPoint(void);
	~CVtkMoveDataFromCellToPoint(void);

	bool LoadFile(std::string filename);

protected:
	vtkCellDataToPointData *m_pvtkCellDataToPointData;
};
