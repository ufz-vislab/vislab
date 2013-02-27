///////////////////////////////////////////////////////////
//  ScalarCellPropGridState.h
//  Implementation of the Class CScalarCellPropGridState
//  Created on:      08-Sep-2008 16:23:38
///////////////////////////////////////////////////////////

#if !defined(EA_C90D8D03_00AA_48f4_A7FF_00E4CB39576C__INCLUDED_)
#define EA_C90D8D03_00AA_48f4_A7FF_00E4CB39576C__INCLUDED_

#include <string>
#include <list>

class CScalarCellPropGridState
{

public:
	CScalarCellPropGridState();
	virtual ~CScalarCellPropGridState();
	double m_dXMin;
	double m_dXMax;
	double m_dYMin;
	double m_dYMax;
	double m_dZMin;
	double m_dZMax;
	std::string m_stdstrCurrentAttribute;
	double m_dCurrentAttributeMin;
	double m_dCurrentAttributeMax;
	double m_dShrinkFactor;
	double m_dLowerThreshold;
	double m_dUpperThreshold;
	
	void PrintState();
};
#endif // !defined(EA_C90D8D03_00AA_48f4_A7FF_00E4CB39576C__INCLUDED_)
