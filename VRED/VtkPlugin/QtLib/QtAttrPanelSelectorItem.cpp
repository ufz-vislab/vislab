///////////////////////////////////////////////////////////
//  QtAttrPanelSelectorItem.cpp
//  Implementation of the Class CQtAttrPanelSelectorItem
//  Created on:      21-Apr-2008 16:20:29
///////////////////////////////////////////////////////////

#include "QtAttrPanelSelectorItem.h"


CQtAttrPanelSelectorItem::CQtAttrPanelSelectorItem(QListView* parent, QString label)
    : QListViewItem(parent, label){

}


CQtAttrPanelSelectorItem::CQtAttrPanelSelectorItem(QListViewItem* parent, QString label)
    : QListViewItem(parent, label){

}


CQtAttrPanelSelectorItem::~CQtAttrPanelSelectorItem(){

}


void CQtAttrPanelSelectorItem::SetId(int id){
	m_iId = id;
}


int CQtAttrPanelSelectorItem::GetId(){

	return m_iId;
}