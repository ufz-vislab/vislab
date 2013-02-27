///////////////////////////////////////////////////////////
//  ScalarGridState.cpp
//  Implementation of the Class CScalarGridState
//  Created on:      08-Jul-2008 17:51:42
///////////////////////////////////////////////////////////

#include "ScalarGridState.h"




CScalarGridState::CScalarGridState(){
	m_bUseSaturationMapping = false;
	m_dXMin = 0.0;
	m_dXMax = 0.0;
	m_dYMin = 0.0;
	m_dYMax = 0.0;
	m_dZMin = 0.0;
	m_dZMax = 0.0;
	m_dValMin = 0.0;
	m_dValMax = 0.0;
	m_bShowXSection = false;
	m_dXSectionPosition = 0.0;
	m_bShowYSection = false;
	m_dYSectionPosition = 0.0;
	m_bShowZSection = false;
	m_dZSectionPosition = 0.0;
	m_bShowContours = false;
	m_iNumContours = 0;
	m_dMinContour = 0.0;
	m_dMaxContour = 0.0;
}


CScalarGridState::~CScalarGridState(){

}