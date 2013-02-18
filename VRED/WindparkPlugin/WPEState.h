#pragma once

#include <string>

class CWPEState
{
public:
	int						m_iId;
	std::string				m_stdstrName;			//Unique name for identification in scenegraph
	std::string				m_stdstrWPETypeName;	//e.g. VESTAS123 or so
	float					m_fPilarHeight;
	float					m_fRotorLength;
	float					m_fXPos;
	float					m_fYPos;
	float					m_fZPos;
	float					m_fInfluenceRadius;

public:
	CWPEState(void);
	~CWPEState(void);
};
