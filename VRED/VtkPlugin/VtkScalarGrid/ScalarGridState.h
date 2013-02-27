///////////////////////////////////////////////////////////
//  ScalarGridState.h
//  Implementation of the Class CScalarGridState
//  Created on:      08-Jul-2008 17:51:41
///////////////////////////////////////////////////////////

#if !defined(EA_8E765B24_BEFB_4e96_A147_E7A25995E22F__INCLUDED_)
#define EA_8E765B24_BEFB_4e96_A147_E7A25995E22F__INCLUDED_

#include <string>

class CScalarGridState
{
public:
	std::string m_stdstrAttributeName;
	std::string m_stdstrColorAttributeName;
	bool m_bUseSaturationMapping;
	
	double m_dXMin;
	double m_dXMax;
	double m_dYMin;
	double m_dYMax;
	double m_dZMin;
	double m_dZMax;
	double m_dValMin;
	double m_dValMax;
	bool m_bShowXSection;
	double m_dXSectionPosition;
	bool m_bShowYSection;
	double m_dYSectionPosition;
	bool m_bShowZSection;
	double m_dZSectionPosition;
	bool m_bShowContours;
	int m_iNumContours;
	double m_dMinContour;
	double m_dMaxContour;

	CScalarGridState();
	virtual ~CScalarGridState();

};
#endif // !defined(EA_8E765B24_BEFB_4e96_A147_E7A25995E22F__INCLUDED_)
