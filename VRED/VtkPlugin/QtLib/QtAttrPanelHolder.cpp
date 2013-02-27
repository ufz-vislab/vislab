///////////////////////////////////////////////////////////
//  QtAttrPanelHolder.cpp
//  Implementation of the Class CQtAttrPanelHolder
//  Created on:      22-Apr-2008 14:20:39
///////////////////////////////////////////////////////////

#include "QtAttrPanelHolder.h"

// all the different possible attribute panels
#include "CQtTestPanel.h"
#include "QtScalarGridAttrPanel.h"
#include "QtVectorGridAttrPanel.h"

CQtAttrPanelHolder::CQtAttrPanelHolder(QWidget* parent, const char* name, WFlags f):
	QWidget(parent, name, f), CAttrPanelHolder(){

}

CQtAttrPanelHolder::~CQtAttrPanelHolder(){

}




/**
 * Function is abstract and must be implemented in a derived GUI class. The
 * function generates and returns the attribute panel specified in the given
 * string.
 */
CAttrPanelObserver* CQtAttrPanelHolder::GetAttrPanel(std::string type){
	if (type == std::string("TestPanel")){
		return new CQtTestPanel(this, "my test panel");
	} else if (type == std::string("ScalarGrid")){
		return new CQtScalarGridAttrPanel(this, "Scalar grid");
	} else if (type == std::string("VectorGrid")){
		return new CQtVectorGridAttrPanel(this, "Vector grid");
	} else {
		return NULL;
	}
}

COpenSGObserver* CQtAttrPanelHolder::Get3DObserver(){
	return new CQtOpenSGObserver();
};