///////////////////////////////////////////////////////////
//  AttrPanelSelector.h
//  Implementation of the Class CAttrPanelSelector
//  Created on:      21-Apr-2008 10:52:34
///////////////////////////////////////////////////////////

#if !defined(EA_76E070D6_0913_4bce_A9B6_BB0F994B5D6F__INCLUDED_)
#define EA_76E070D6_0913_4bce_A9B6_BB0F994B5D6F__INCLUDED_

#include "AttrPanelObserver.h"

#include <map>
#include <fstream>

/**
 * Base class for a GUI class that implements a tree or list view and keeps book
 * of all attribute panels of type CAttrPanelObserver and shows them when they are
 * selected in the tree view. Each of these is identified internally by a unique
 * ID. If the user selects one of the different items, HideAttrPanel() is first
 * called on all panels and subsequently ShowAttrPanel is called on the
 * AttrPanelObserver with the corresponding id. The class does not keep track of
 * the instantiation and deletion process!!!!!
 */
class CAttrPanelSelector
{

public:
	CAttrPanelSelector();
	virtual ~CAttrPanelSelector();
	/**
	 * Function is abstract and must be implemented in derived GUI classes! Simply add
	 * a root node to the tree view. 
	 */
	virtual void AddRootNode(std::string rootName) =0;
	/**
	 * Function is abstract and must be implemented in derived GUI classes! Add a root
	 * node with an image to the tree view
	 */
	virtual void AddRootNode(std::string rootName, std::string imageFileName) =0;
	/**
	 * Function is abstract and must be implemented in derived GUI classes! Adds an
	 * item to the tree under the root node with the given name. If the root not does
	 * not exist it is established (using AddRootNode). The returned id must be kept
	 * and is a unique identifier for the differnt attribute panels. The id is stored
	 * together with the pointer to the attribute panel in the IdToAttrPanel map
	 * (using AddAttrPanel().
	 */
	virtual void AddItem(std::string rootName, std::string name, CAttrPanelObserver* panel) =0;

protected:
	static int m_iCurrentId;
	std::map<int, CAttrPanelObserver*> m_stdmapIdToAttrPanel;

	/**
	 * Generate a new unique id by returning and incrementing m_iCurrentId.
	 */
	virtual int GetNewId();
	/**
	 * Lookup the CAttrPanelObserver with the given id using the map. Call
	 * HideAttributePanel() on all panels in the map. Call ShowAttributePanel() on the
	 * panel with the given id.
	 */
	virtual bool ShowAttrPanel(int id);
	/**
	 * Controls if the id already exists. If not the attribute panel and its id are
	 * added to the map. This is a pure helper function as no treeItem for any GUI is
	 * generated.
	 */
	virtual bool AddAttrPanel(int id, CAttrPanelObserver* panel);
	/**
	 * Remove the given attribute panel from the list.
	 */
	virtual bool RemoveAttrPanel(int id);

};
#endif // !defined(EA_76E070D6_0913_4bce_A9B6_BB0F994B5D6F__INCLUDED_)
