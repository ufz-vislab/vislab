///////////////////////////////////////////////////////////
//  QtOpenSGObserver.cpp
//  Implementation of the Class CQtOpenSGObserver
//  Created on:      15-Jul-2008 17:38:34
///////////////////////////////////////////////////////////

#include "QtOpenSGObserver.h"

QGLWidget* CQtOpenSGObserver::m_pqGLWidget = NULL;

CQtOpenSGObserver::CQtOpenSGObserver(){

}



CQtOpenSGObserver::~CQtOpenSGObserver(){

}



void CQtOpenSGObserver::Update(){
	if (m_pqGLWidget != NULL) m_pqGLWidget->updateGL();
}


/**
 * Sets the subject this object shall observe. The return value indicates if this
 * observer is suitable to observe the given subject.
 */
bool CQtOpenSGObserver::SetSubject(CSubject* sub){
	m_pSubject = sub;
	return true;
}

void CQtOpenSGObserver::SetQGLWidget(QGLWidget* widget){
	m_pqGLWidget = widget;
}