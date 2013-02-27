///////////////////////////////////////////////////////////
//  CQtAttrPanelBase.cpp
//  Implementation of the Class CQtAttrPanelBase
//  Created on:      19-Apr-2008 15:27:56
///////////////////////////////////////////////////////////

#include "CQtAttrPanelBase.h"


CQtAttrPanelBase::CQtAttrPanelBase(QWidget* parent, const char* name, WFlags f):
		QWidget(parent, name, f){
}


CQtAttrPanelBase::~CQtAttrPanelBase(){

}

void CQtAttrPanelBase::SetSize(int x, int y){
	setMinimumWidth(x);
	setMinimumHeight(y);
}

void CQtAttrPanelBase::ShowAttrPanel(){
	show();
}

void CQtAttrPanelBase::HideAttrPanel(){
	hide();
}