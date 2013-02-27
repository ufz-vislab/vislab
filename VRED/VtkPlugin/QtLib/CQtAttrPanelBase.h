///////////////////////////////////////////////////////////
//  CQtAttrPanelBase.h
//  Implementation of the Class CQtAttrPanelBase
//  Created on:      19-Apr-2008 15:27:56
///////////////////////////////////////////////////////////

#if !defined(EA_F0390ABB_59FE_4b7c_B863_38D2D0FCA500__INCLUDED_)
#define EA_F0390ABB_59FE_4b7c_B863_38D2D0FCA500__INCLUDED_

#include "QWidget.h"
#include "AttrPanelObserver.h"

/**
 * Base class for all attribute panels while using the Qt GUI toolkit. This class
 * will be used as a template in Qt Designer. All derived widget will are
 * generated outside of Visual Studio and outside of Enterprise Architekt using Qt-
 * Designer
 */
class CQtAttrPanelBase : public QWidget, public CAttrPanelObserver
{
Q_OBJECT
public:
	CQtAttrPanelBase(QWidget* parent = 0, const char* name = 0, WFlags f = 0);
	virtual ~CQtAttrPanelBase();

	virtual void SetSize(int x, int y);
	virtual void ShowAttrPanel();
	virtual void HideAttrPanel();
};
#endif // !defined(EA_F0390ABB_59FE_4b7c_B863_38D2D0FCA500__INCLUDED_)
