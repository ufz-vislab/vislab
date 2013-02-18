#include "WPEState.h"

CWPEState::CWPEState(void){
	m_iId = 0;
	m_stdstrName = std::string("0");
	m_stdstrWPETypeName = std::string("default wpe");
	m_fPilarHeight = 0.0;
	m_fRotorLength = 0.0;
	m_fXPos = 0.0;
	m_fYPos = 0.0;
	m_fZPos = 0.0;
	m_fInfluenceRadius = 0.0;
}

CWPEState::~CWPEState(void)
{
}
