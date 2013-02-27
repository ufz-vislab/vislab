///////////////////////////////////////////////////////////
//  TensorGridState.h
//  Implementation of the Class CTensorGridState
//  Created on:      03-Sep-2008 11:49:28
///////////////////////////////////////////////////////////

#if !defined(EA_678FA2CB_AEE1_4dde_B461_E4950869B9FD__INCLUDED_)
#define EA_678FA2CB_AEE1_4dde_B461_E4950869B9FD__INCLUDED_

#include <string>

class CTensorGridState
{

public:
	CTensorGridState();
	virtual ~CTensorGridState();
	
	std::string m_stdstrActiveTensorName;
	std::string m_stdstrActiveScalarName;
	double m_dXMin;
	double m_dXMax;
	double m_dYMin;
	double m_dYMax;
	double m_dZMin;
	double m_dZMax;
	int m_iNumPoints;
	bool m_bShowGlyphs;
	int m_iGlyphMaskRatio;
	bool m_bUseDeformedSpheres; // else use axes
	bool m_bColorByEigenvalues;
	double m_dGlyphScaleFactor;
};
#endif // !defined(EA_678FA2CB_AEE1_4dde_B461_E4950869B9FD__INCLUDED_)
