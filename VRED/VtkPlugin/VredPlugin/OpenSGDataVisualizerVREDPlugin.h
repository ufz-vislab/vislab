#pragma once

#define VR_DLL_WITH_QT

#include <qstring.h>
#include "vrModuleBase.h"

class CDataVisualizerVREDPanel;

class COpenSGDataVisualizerVREDPlugin : public vrModuleBase
{
public:
	COpenSGDataVisualizerVREDPlugin(void);
	~COpenSGDataVisualizerVREDPlugin(void);

protected:
    virtual QWidget     *getWidget(void) const;
    virtual void        init(void);

    virtual bool        receivedMessage(vrMessage &msg);

    static COpenSGDataVisualizerVREDPlugin	_the;
	
	virtual void		loop(void);
	virtual void		update(void);

private:

    //QLabel *_main;
	CDataVisualizerVREDPanel *_main;
};
