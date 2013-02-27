///////////////////////////////////////////////////////////
//  ScalarCellPropGridState.cpp
//  Implementation of the Class CScalarCellPropGridState
//  Created on:      08-Sep-2008 16:23:38
///////////////////////////////////////////////////////////

#include "ScalarCellPropGridState.h"
#include <iostream>

CScalarCellPropGridState::CScalarCellPropGridState(){
	m_dXMin = 0.0;
	m_dXMax = 0.0;
	m_dYMin = 0.0;
	m_dYMax = 0.0;
	m_dZMin = 0.0;
	m_dZMax = 0.0;
	m_dCurrentAttributeMin = 0.0;
	m_dCurrentAttributeMax = 0.0;
	m_dShrinkFactor = 1.0;
	m_dLowerThreshold = 0.0;
	m_dUpperThreshold = 0.0;
}



CScalarCellPropGridState::~CScalarCellPropGridState(){

}

void CScalarCellPropGridState::PrintState(){
	std::cout << "extends:" << std::endl;
	std::cout << "  " << m_dXMin << "	" << m_dYMin << "	" << m_dZMin << std::endl;
	std::cout << "  " << m_dXMax << "	" << m_dYMax << "	" << m_dZMax << std::endl;
	std::cout << "CurrentAttribute: " << m_stdstrCurrentAttribute << std::endl;
	std::cout << "  min/max: " << m_dCurrentAttributeMin << " " << m_dCurrentAttributeMax << std::endl;
	std::cout << "Further attributes: " << std::endl;
	std::list<std::string>::iterator iter;
	std::cout << "Lower threshold: " << m_dLowerThreshold << std::endl;
	std::cout << "Upper threshold: " << m_dUpperThreshold << std::endl;
	std::cout << "Shring factor: " << m_dShrinkFactor << std::endl;
}