#pragma once

//class CVredWindparkPlugin : public vrModuleBase
// VR_DLL_WITH_QT muss definiert werden sonst beim compilieren des moc source codes
// in den kernel headern nicht VR_DLLIMPORT definiert wird!
#define VR_DLL_WITH_QT

#include <string>
#include <list>
#include <qstring.h>

#include "vrModuleBase.h"


//----------------------------------------------------------------------------
// forward declarations
//----------------------------------------------------------------------------

class QLabel;
class CNavigationMainFrame;

//----------------------------------------------------------------------------

class CVredNavigationPlugin : public vrModuleBase
{
public:
	CVredNavigationPlugin(void);
	~CVredNavigationPlugin(void);

	protected:

    virtual QWidget     *getWidget(void) const;
    virtual void        init(void);

    virtual bool        receivedMessage(vrMessage &msg);

    static CVredNavigationPlugin _the;

private:

    //QLabel *_main;
	CNavigationMainFrame *_main;
};

//----------------------------------------------------------------------------
