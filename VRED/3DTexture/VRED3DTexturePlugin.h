#pragma once

// VR_DLL_WITH_QT muss definiert werden sonst beim compilieren des moc source codes
// in den kernel headern nicht VR_DLLIMPORT definiert wird!
#define VR_DLL_WITH_QT

#include <string>
#include <list>

#include <qstring.h>

#include "vrModuleBase.h"

#include "VRED3DTexturePanel.h"

//#include "CQTestWidget.h"

//----------------------------------------------------------------------------
// forward declarations
//----------------------------------------------------------------------------

class QLabel;
class QVRED3DTexturePanel;


class CVRED3DTexturePlugin : public vrModuleBase {
public:

    CVRED3DTexturePlugin(void);
    ~CVRED3DTexturePlugin(void);

protected:

    virtual QWidget     *getWidget(void) const;
    virtual void        init(void);

    virtual bool        receivedMessage(vrMessage &msg);

    static CVRED3DTexturePlugin	_the;
	
	virtual void		loop(void);
	virtual void		update(void);

private:

	//CQtVRED3DTexturePanel *_main;
    //QLabel *_main;
	//CQTestWidget *_main;
	QVRED3DTexturePanel *_main;
};
