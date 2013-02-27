///////////////////////////////////////////////////////////
//  Observer.cpp
//  Implementation of the Class CObserver
//  Created on:      19-Apr-2008 11:51:56
///////////////////////////////////////////////////////////

#include "Observer.h"
#include "Subject.h"



CObserver::CObserver(){
	m_pSubject = NULL;
}


CObserver::~CObserver(){

}


void CObserver::RemoveSubject(){
	m_pSubject = NULL;
}