///////////////////////////////////////////////////////////
//  QtAttrPanelSelectorItem.h
//  Implementation of the Class CQtAttrPanelSelectorItem
//  Created on:      21-Apr-2008 16:20:29
///////////////////////////////////////////////////////////

#if !defined(EA_A2DDD766_D8D7_4eec_ABC1_973FDF9EA9AD__INCLUDED_)
#define EA_A2DDD766_D8D7_4eec_ABC1_973FDF9EA9AD__INCLUDED_

#include "QListView.h"

/**
 * Extends Qts QListViewItem to store and provide the attribute panels unique id.
 */
class CQtAttrPanelSelectorItem : public QListViewItem
{

public:
	CQtAttrPanelSelectorItem(QListView* parent, QString label);
	CQtAttrPanelSelectorItem(QListViewItem* parent, QString label);
	virtual ~CQtAttrPanelSelectorItem();
	void SetId(int id);
	int GetId();

protected:
	int m_iId;

};
#endif // !defined(EA_A2DDD766_D8D7_4eec_ABC1_973FDF9EA9AD__INCLUDED_)
