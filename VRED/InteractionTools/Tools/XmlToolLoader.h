#pragma once

class CXmlToolLoader
{
public:
	CXmlToolLoader(void);
	~CXmlToolLoader(void);

	static void LoadFile(const char *filename);
	static void GenerateDefaultFile(const char *filename);
};
