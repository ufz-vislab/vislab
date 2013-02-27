///////////////////////////////////////////////////////////
//  AttrPanelPanel.h
//  Implementation of the Class CAttrPanelPanel
//  Created on:      19-Apr-2008 11:48:14
///////////////////////////////////////////////////////////

#if !defined(EA_837F7756_D3E6_4e65_8580_91CD062967F3__INCLUDED_)
#define EA_837F7756_D3E6_4e65_8580_91CD062967F3__INCLUDED_

#include "AttrPanelObserver.h"
#include <map>

class CAttrPanelPanel
{

public:
	CAttrPanelPanel();
	virtual ~CAttrPanelPanel();

	void ShowAttributePanel(int id);
	void AddAttributePanel(int id, CAttrPanelObserver* attrPanel);

private:
	std::map<int, CAttrPanelObserver*> m_stdmapIdToPanel;

};
#endif // !defined(EA_837F7756_D3E6_4e65_8580_91CD062967F3__INCLUDED_)
