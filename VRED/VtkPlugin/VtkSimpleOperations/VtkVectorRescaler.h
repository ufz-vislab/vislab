#pragma once
#include "vtkreaderwriterpairconstructor.h"

class CVtkVectorRescaler :
	public CVtkReaderWriterPairConstructor
{
public:
	CVtkVectorRescaler(void);
	~CVtkVectorRescaler(void);

	bool Rescale(std::string filename, std::string vectorAttrName, double scaleFactor, std::string lengthAttrName = std::string());
};