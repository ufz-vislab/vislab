#pragma once

#include "ToolBase.h"

class CToolFactory
{
private:
	static CToolFactory *m_pInstance;
public:
	static CToolFactory *Instance();

	CToolBase* GenerateTool(std::string tooltype);

protected:
	CToolFactory(void);
	~CToolFactory(void);
};
