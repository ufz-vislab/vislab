#pragma once

#include <string>

#include "WPEProjectController.h"

class CProjectControllerLoader
{
protected:
	std::string		m_stdstrTerrainFilename;
	std::string		m_stdstrWPETypesFilename;
	std::string		m_stdstrArealImageFilename;
	std::string		m_stdstrWPE3DModelFilename;
public:
	CProjectControllerLoader(void);
	~CProjectControllerLoader(void);

	void			SetTerrainFilename(const char* filename){m_stdstrTerrainFilename = std::string(filename);}
	void			SetWPETypesFilename(const char* filename){m_stdstrWPETypesFilename = std::string(filename);}
	void			SetArealImageFilename(const char* filename){m_stdstrArealImageFilename = std::string(filename);}
	void			SetWPE3DModelFilename(const char* filename){m_stdstrWPE3DModelFilename = std::string(filename);}

	bool			AllDataComplete();
	void			LoadAll();
};
