#pragma once

#include <string>

class CWPETypeInfo
{
public:
	CWPETypeInfo(void);
	~CWPETypeInfo(void);

	std::string				m_stdstrWPETypeName;	//e.g. VESTAS123 or so
	float					m_fPilarHeight;
	float					m_fRotorLength;
	float					m_fInfluenceRadius;
	float					m_fMWh;
};
