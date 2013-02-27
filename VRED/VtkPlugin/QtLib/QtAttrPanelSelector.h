///////////////////////////////////////////////////////////
//  QtAttrPanelSelector.h
//  Implementation of the Class CQtAttrPanelSelector
//  Created on:      21-Apr-2008 15:39:46
///////////////////////////////////////////////////////////

#if !defined(EA_8AFAB98D_A250_4b5c_9E69_9012E1EF303E__INCLUDED_)
#define EA_8AFAB98D_A250_4b5c_9E69_9012E1EF303E__INCLUDED_

#include "AttrPanelObserver.h"
#include "QListView.h"
#include "AttrPanelSelector.h"
#include "QtAttrPanelSelectorItem.h"

#include <QPixmap.h>

#include <string>
#include <map>

/**
 * Class that implements the CAttrPanelSelector within the Qt GUI environment. The
 * class represents a tree view which shows the name of different items for which
 * attribute panels exist. The user can select one of the items and the
 * corresponding attribute panel is shown. Most of the functions are to build up
 * the tree of the view and to switch the attribute panels (hide/show).
 */
class CQtAttrPanelSelector : public QListView, public CAttrPanelSelector
{
Q_OBJECT
public:
	CQtAttrPanelSelector();
	CQtAttrPanelSelector(QWidget* parent, const char* name = 0, WFlags f = 0);
	virtual ~CQtAttrPanelSelector();

	/**
	 * Check if the given name already exists as root-node. If not, add a new root
	 * node to the tree view.
	 */
	void AddRootNode(std::string rootName);
	/**
	 * Check if the given name already exists as a root node. If not, add a root node
	 * with an image to the tree view
	 */
	void AddRootNode(std::string rootName, std::string imageFileName);
	/**
	 * Adds an item to the tree under the root node with the given name. If the root
	 * not does not exist it is established (using AddRootNode). The returned id must
	 * be kept and is a unique identifier for the differnt attribute panels. The id is
	 * stored together with the pointer to the attribute panel in the IdToAttrPanel
	 * map (using AddAttrPanel().
	 */
	void AddItem(std::string rootName, std::string name, CAttrPanelObserver* panel);
	/**
	 * Overrides the selection changed method from QListView. The id is retrieved from
	 * the CQtAttributePanelSelectorItem and used to call ShowAttributePanel(id).
	 */
public slots:
	void OnSelectionChanged(QListViewItem *item);

private:
	std::map<std::string, CQtAttrPanelSelectorItem*> m_stdmapRootNameToItem;

};
#endif // !defined(EA_8AFAB98D_A250_4b5c_9E69_9012E1EF303E__INCLUDED_)
