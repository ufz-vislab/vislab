///////////////////////////////////////////////////////////
//  AttrPanelSelector.cpp
//  Implementation of the Class CAttrPanelSelector
//  Created on:      21-Apr-2008 10:52:34
///////////////////////////////////////////////////////////

#include "AttrPanelSelector.h"

int CAttrPanelSelector::m_iCurrentId = 0;


CAttrPanelSelector::CAttrPanelSelector(){

}


CAttrPanelSelector::~CAttrPanelSelector(){

}


/**
 * Generate a new unique id by returning and incrementing m_iCurrentId.
 */
int CAttrPanelSelector::GetNewId(){
	m_iCurrentId = m_iCurrentId + 1;
	return m_iCurrentId;
}


/**
 * Lookup the CAttrPanelObserver with the given id using the map. Call
 * HideAttributePanel() on all panels in the map. Call ShowAttributePanel() on the
 * panel with the given id.
 */
bool CAttrPanelSelector::ShowAttrPanel(int id){
	std::map<int, CAttrPanelObserver*>::iterator showIter;
	std::map<int, CAttrPanelObserver*>::iterator hideIter;
	showIter = m_stdmapIdToAttrPanel.find(id);
	
	if (showIter != m_stdmapIdToAttrPanel.end()){
		for (hideIter = m_stdmapIdToAttrPanel.begin(); hideIter != m_stdmapIdToAttrPanel.end(); hideIter++){
			((*hideIter).second)->HideAttrPanel();
		}
		((*showIter).second)->ShowAttrPanel();
		return true;
	}else{
		return false;
	}
}


/**
 * Controls if the id already exists. If not the attribute panel and its id are
 * added to the map. This is a pure helper function as no treeItem for any GUI is
 * generated.
 */
bool CAttrPanelSelector::AddAttrPanel(int id, CAttrPanelObserver* panel){
	std::map<int, CAttrPanelObserver*>::value_type item(id, panel);
	return (m_stdmapIdToAttrPanel.insert(item)).second;
}


/**
 * Remove the given attribute panel from the list.
 */
bool CAttrPanelSelector::RemoveAttrPanel(int id){

	return false;
}