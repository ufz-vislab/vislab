///////////////////////////////////////////////////////////
//  VectorGridState.h
//  Implementation of the Class CVectorGridState
//  Created on:      05-Aug-2008 14:27:22
///////////////////////////////////////////////////////////

#if !defined(EA_628DAF98_24F2_4550_9260_CD3571CDF482__INCLUDED_)
#define EA_628DAF98_24F2_4550_9260_CD3571CDF482__INCLUDED_

#include <string>

class CVectorGridState
{
public:
	CVectorGridState();
	virtual ~CVectorGridState();
	int m_iNumPoints;
	double m_dXMin;
	double m_dXMax;
	double m_dYMin;
	double m_dYMax;
	double m_dZMin;
	double m_dZMax;
	double m_dMaxVecLength;
	int m_iGlyphMaskRatio;
	double m_dGlyphScaleFactor;
	bool m_bShowGlyphs;
	double m_dStepLength;
	double m_dMaxPropagationLength;
	double m_dTubeRadius;
	int	m_iIntegrationDirection;
	std::string m_stdstrActiveVectorName;
	std::string m_stdstrActiveScalarName;
	bool m_bShowStreamlines;
	
	bool m_bUseGridAsSourceForStreamlines;
	int m_iNumStreamlineSourcesXDir;
	int m_iNumStreamlineSourcesYDir;
	int m_iNumStreamlineSourcesZDir;
	bool m_bUseGeomObjectAsSourceForStreamlines;
	std::string m_stdstrStreamlinesGeomObjectName;
	
	enum {FORWARD, BACKWARD, BOTH};
};
#endif // !defined(EA_628DAF98_24F2_4550_9260_CD3571CDF482__INCLUDED_)
