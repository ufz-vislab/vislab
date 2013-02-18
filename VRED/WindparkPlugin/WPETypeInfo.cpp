#include "WPETypeInfo.h"

CWPETypeInfo::CWPETypeInfo(void)
{
	m_stdstrWPETypeName = std::string("unknown");
	m_fPilarHeight = 0.0;
	m_fRotorLength = 0.0;
	m_fInfluenceRadius = 30.0;
	m_fMWh = 0.0;
}

CWPETypeInfo::~CWPETypeInfo(void)
{
}
