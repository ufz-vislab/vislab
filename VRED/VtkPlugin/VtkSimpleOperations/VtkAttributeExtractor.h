#pragma once
#include "vtkreaderwriterpairconstructor.h"

class CVtkAttributeExtractor :
	public CVtkReaderWriterPairConstructor
{
public:
	CVtkAttributeExtractor(void);
	~CVtkAttributeExtractor(void);

	bool ExtractAttributes(std::string outFilename, std::list<std::string> attributeNames);
};
