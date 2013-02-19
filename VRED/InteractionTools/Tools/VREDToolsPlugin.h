#pragma once

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
class CQtVREDToolsPanel;


class CVREDToolsPlugin : public vrModuleBase {
public:

    CVREDToolsPlugin(void);
    ~CVREDToolsPlugin(void);

protected:

    virtual QWidget     *getWidget(void) const;
    virtual void        init(void);

    virtual bool        receivedMessage(vrMessage &msg);

    static CVREDToolsPlugin	_the;
	
	virtual void		loop(void);
	virtual void		update(void);

private:

	CQtVREDToolsPanel *_main;
    //QLabel *_main;
	//CQTestWidget *_main;
};
