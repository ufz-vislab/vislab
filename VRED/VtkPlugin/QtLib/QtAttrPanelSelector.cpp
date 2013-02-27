///////////////////////////////////////////////////////////
//  QtAttrPanelSelector.cpp
//  Implementation of the Class CQtAttrPanelSelector
//  Created on:      21-Apr-2008 15:39:46
///////////////////////////////////////////////////////////

#include "QtAttrPanelSelector.h"


CQtAttrPanelSelector::CQtAttrPanelSelector(){
	this->addColumn("items");
}

CQtAttrPanelSelector::CQtAttrPanelSelector(QWidget * parent, const char *name, WFlags f):
		QListView(parent, name, f), CAttrPanelSelector(){
	connect(this, SIGNAL( selectionChanged(QListViewItem*) ), this, SLOT( OnSelectionChanged(QListViewItem*) ));
	this->addColumn("items");
}

CQtAttrPanelSelector::~CQtAttrPanelSelector(){

}



/**
 * Check if the given name already exists as root-node. If not, add a new root
 * node to the tree view.
 */
void CQtAttrPanelSelector::AddRootNode(std::string rootName){
	std::map<std::string, CQtAttrPanelSelectorItem*>::iterator iter;
	iter = m_stdmapRootNameToItem.find(rootName);
	if (iter == m_stdmapRootNameToItem.end()){
		CQtAttrPanelSelectorItem *item = new CQtAttrPanelSelectorItem(this, QString(rootName.c_str()));
		item->SetId(this->GetNewId());
		std::map<std::string, CQtAttrPanelSelectorItem*>::value_type myPair(rootName, item);
		m_stdmapRootNameToItem.insert(myPair);
	}
}


/**
 * Check if the given name already exists as a root node. If not, add a root node
 * with an image to the tree view
 */
void CQtAttrPanelSelector::AddRootNode(std::string rootName, std::string imageFileName){
	std::map<std::string, CQtAttrPanelSelectorItem*>::iterator iter;
	iter = m_stdmapRootNameToItem.find(rootName);
	if (iter == m_stdmapRootNameToItem.end()){
		CQtAttrPanelSelectorItem *item = new CQtAttrPanelSelectorItem(this, QString(rootName.c_str()));
		item->SetId(this->GetNewId());
		QPixmap image;
		image.load(imageFileName.c_str());
		item->setPixmap(0, image);
		std::map<std::string, CQtAttrPanelSelectorItem*>::value_type myPair(rootName, item);
		m_stdmapRootNameToItem.insert(myPair);
	}
}


/**
 * Adds an item to the tree under the root node with the given name. If the root
 * node does not exist it is established (using AddRootNode). The returned id must
 * be kept and is a unique identifier for the differnt attribute panels. The id is
 * stored together with the pointer to the attribute panel in the IdToAttrPanel
 * map (using AddAttrPanel().
 */
void CQtAttrPanelSelector::AddItem(std::string rootName, std::string name, CAttrPanelObserver* panel){
	std::map<std::string, CQtAttrPanelSelectorItem*>::iterator iter;
	iter = m_stdmapRootNameToItem.find(rootName);
	if (iter == m_stdmapRootNameToItem.end()){
		AddRootNode(rootName);
		iter = m_stdmapRootNameToItem.find(rootName);
		if (iter == m_stdmapRootNameToItem.end()) return;
	}
	
	int newId = this->GetNewId();
	std::map<int, CAttrPanelObserver*>::value_type idPanelPair(newId, panel);
	m_stdmapIdToAttrPanel.insert(idPanelPair);
	
	CQtAttrPanelSelectorItem *item = new CQtAttrPanelSelectorItem((*iter).second, QString(name.c_str()));
	item->SetId(newId);
}


/**
 * Overrides the selection changed method from QListView. The id is retrieved from
 * the CQtAttributePanelSelectorItem and used to call ShowAttributePanel(id).
 */
void CQtAttrPanelSelector::OnSelectionChanged(QListViewItem *item){
	CQtAttrPanelSelectorItem *selectorItem = (CQtAttrPanelSelectorItem*) item;
	int id = selectorItem->GetId();
	this->ShowAttrPanel(id);
}