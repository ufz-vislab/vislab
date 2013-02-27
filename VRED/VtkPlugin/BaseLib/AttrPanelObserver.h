///////////////////////////////////////////////////////////
//  AttrPanelObserver.h
//  Implementation of the Class CAttrPanelObserver
//  Created on:      19-Apr-2008 11:50:52
///////////////////////////////////////////////////////////

#if !defined(EA_3449BF88_E86E_4845_B907_3D0A4770EE11__INCLUDED_)
#define EA_3449BF88_E86E_4845_B907_3D0A4770EE11__INCLUDED_

#include "Observer.h"

/**
 * Base class for GUI Attribute Panels. For each specific data type like skalar
 * data or vector data a specific attribute panel has to be provided. These will
 * usually be developed using some GUI Toolkit like QT-Designer
 */
class CAttrPanelObserver : public CObserver
{

public:
	CAttrPanelObserver();
	virtual ~CAttrPanelObserver();

	virtual void Update() =0;
	virtual bool SetSubject(CSubject* sub) =0;
	virtual void SetSize(int x, int y) =0;
	virtual void ShowAttrPanel() =0;
	virtual void HideAttrPanel() =0;
	virtual void SetInitialValues() =0;
};
#endif // !defined(EA_3449BF88_E86E_4845_B907_3D0A4770EE11__INCLUDED_)
