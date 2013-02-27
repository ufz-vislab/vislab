#pragma once
#include "vtkreaderwriterpairconstructor.h"

class vtkMergeFields;

class CVtkScalarsToVector :
	public CVtkReaderWriterPairConstructor
{
public:
	CVtkScalarsToVector(void);
	~CVtkScalarsToVector(void);

	bool			LoadFile(std::string filename);
	bool			SetVectorComponents(std::string newCompName, std::list<std::string> components);
protected:
	vtkMergeFields*	m_pvtkMergeFields;
};
