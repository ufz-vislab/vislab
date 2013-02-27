///////////////////////////////////////////////////////////
//  OpenSGObserver.cpp
//  Implementation of the Class COpenSGObserver
//  Created on:      15-Jul-2008 16:48:27
///////////////////////////////////////////////////////////

#include "OpenSGObserver.h"


COpenSGObserver::COpenSGObserver(){

}



COpenSGObserver::~COpenSGObserver(){

}





/**
 * Function is abstract. In derived classes this function shall call the necessary
 * functions to update the 3D view. For example calling the render action from
 * OpenSG or updateGL of a QGlWidget.
 */
//void COpenSGObserver::Update(){
//	COpenSGCanvas::Instance()->ShowAll();
//}


/**
 * Sets the subject this object shall observe. The return value indicates if this
 * observer is suitable to observe the given subject.
 */
//bool COpenSGObserver::SetSubject(CSubject* sub){
//
//	m_pSubject = sub;
//	return true;
//}