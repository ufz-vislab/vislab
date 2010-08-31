/********************************************************************
	created:	1.7.2009   13:51
	filename: 	e:\bilke\apps\vred-2.5\dev\SkyBackgroundPlugin\SkyBackgroundPlugin.h
	author:		Lars Bilke
	
	purpose:	Definition of the sky background plugin
*********************************************************************/
#ifndef SKYBACKGROUNDPLUGIN_HEADER
#define SKYBACKGROUNDPLUGIN_HEADER

// ** INCLUDES **
#include <string>
#include <list>

#include <qstring.h>

#include "vrModuleBase.h"

class SkyBackgroundPluginForm;


/**
 * SkyBackgroundPlugin
 */
class SkyBackgroundPlugin : public vrModuleBase
{
public:
	SkyBackgroundPlugin();
	~SkyBackgroundPlugin();

protected:
	virtual void        init(void);

	virtual bool        receivedMessage(vrMessage &msg);

	static SkyBackgroundPlugin the;

	virtual QWidget     *getWidget(void) const;
private:

	SkyBackgroundPluginForm* mainWidget;

};

#endif // SKYBACKGROUNDPLUGIN_HEADER