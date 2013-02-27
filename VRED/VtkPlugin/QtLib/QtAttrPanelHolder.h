///////////////////////////////////////////////////////////
//  QtAttrPanelHolder.h
//  Implementation of the Class CQtAttrPanelHolder
//  Created on:      22-Apr-2008 14:20:38
///////////////////////////////////////////////////////////

#if !defined(EA_3D32E794_E21D_4366_A9D1_2F8101798A7D__INCLUDED_)
#define EA_3D32E794_E21D_4366_A9D1_2F8101798A7D__INCLUDED_

#include "QWidget.h"
#include "AttrPanelHolder.h"
#include "AttrPanelObserver.h"

#include "QtOpenSGObserver.h"
#include "Observer.h"

#include <string>

class CQtAttrPanelHolder : public QWidget, public CAttrPanelHolder
{
Q_OBJECT
public:
	CQtAttrPanelHolder(QWidget* parent = 0, const char* name = 0, WFlags f = 0);
	
	virtual ~CQtAttrPanelHolder();
	
	/**
	 * Function is abstract and must be implemented in a derived GUI class. The
	 * function generates and returns the attribute panel specified in the given
	 * string.
	 */
	CAttrPanelObserver* GetAttrPanel(std::string type);
	
	COpenSGObserver* Get3DObserver();

};
#endif // !defined(EA_3D32E794_E21D_4366_A9D1_2F8101798A7D__INCLUDED_)
