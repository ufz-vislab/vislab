#pragma once
#include "vtkreaderwriterpairconstructor.h"

class vtkMergeFields;

class CVtkScalarsToTensor :
	public CVtkReaderWriterPairConstructor
{
public:
	CVtkScalarsToTensor(void);
	~CVtkScalarsToTensor(void);

	bool			LoadFile(std::string filename);
	bool			SetTensorComponents(std::string newCompName, std::list<std::string> components);
protected:
	vtkMergeFields*	m_pvtkMergeFields;
};
