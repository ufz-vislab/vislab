///////////////////////////////////////////////////////////
//  VectorGridState.cpp
//  Implementation of the Class CVectorGridState
//  Created on:      05-Aug-2008 14:27:22
///////////////////////////////////////////////////////////

#include "VectorGridState.h"


CVectorGridState::CVectorGridState(){
	m_iNumPoints = 0;
	m_dXMin = 0.0;
	m_dXMax = 0.0;
	m_dYMin = 0.0;
	m_dYMax = 0.0;
	m_dZMin = 0.0;
	m_dZMax = 0.0;
	m_dMaxVecLength = 0.0;
	m_iGlyphMaskRatio = 1.0;
	m_dGlyphScaleFactor = 1.0;
	m_bShowGlyphs = false;
	m_dStepLength = 0.1;
	m_dMaxPropagationLength = 100.0;
	m_dTubeRadius = 0.05;
	m_iIntegrationDirection = BOTH;
	m_bShowStreamlines = false;
	
	m_bUseGridAsSourceForStreamlines = false;
	m_iNumStreamlineSourcesXDir = 2;
	m_iNumStreamlineSourcesYDir = 2;
	m_iNumStreamlineSourcesZDir = 2;
	m_bUseGeomObjectAsSourceForStreamlines = false;
}



CVectorGridState::~CVectorGridState(){

}