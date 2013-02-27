///////////////////////////////////////////////////////////
//  TensorGridState.cpp
//  Implementation of the Class CTensorGridState
//  Created on:      03-Sep-2008 11:49:28
///////////////////////////////////////////////////////////

#include "TensorGridState.h"


CTensorGridState::CTensorGridState(){
	m_dXMin = 0.0;
	m_dXMax = 0.0;
	m_dYMin = 0.0;
	m_dYMax = 0.0;
	m_dZMin = 0.0;
	m_dZMax = 0.0;
	m_iNumPoints = 0;
	m_bShowGlyphs = true;
	m_iGlyphMaskRatio = 1;
	m_bUseDeformedSpheres = true; // else use axes
	m_bColorByEigenvalues = true;
	m_dGlyphScaleFactor = 1.0;
}



CTensorGridState::~CTensorGridState(){

}