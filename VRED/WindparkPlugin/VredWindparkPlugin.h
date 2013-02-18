#pragma once

//class CVredWindparkPlugin : public vrModuleBase
// VR_DLL_WITH_QT muss definiert werden sonst beim compilieren des moc source codes
// in den kernel headern nicht VR_DLLIMPORT definiert wird!
#define VR_DLL_WITH_QT

#include <string>
#include <list>
#include <qstring.h>

#include "vrModuleBase.h"

#include "WindparkPluginPanel.h"
#include "WPEMainFrame.h"


//----------------------------------------------------------------------------
// forward declarations
//----------------------------------------------------------------------------

class QLabel;
class QTime;

//----------------------------------------------------------------------------

class CVredWindparkPlugin : public vrModuleBase
{
public:

    CVredWindparkPlugin(void);
    ~CVredWindparkPlugin(void);

protected:

    virtual QWidget     *getWidget(void) const;
    virtual void        init(void);

    virtual bool        receivedMessage(vrMessage &msg);

	virtual void		loop();
	virtual void		UseTime(){m_bUseLoopFunction = true;}

    static CVredWindparkPlugin _the;

private:

    //QLabel *_main;
	//CQTestWidget *_main;
	//CWindparkPluginPanel *_main;
	CWPEMainFrame *_main;
	static QTime	*m_pTime;
	static bool		m_bUseLoopFunction;
};

//----------------------------------------------------------------------------

