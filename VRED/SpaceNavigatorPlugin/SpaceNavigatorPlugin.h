/********************************************************************
	created:	13.7.2009   14:17
	filename: 	e:\bilke\projects\SpaceNavigator\SpaceNavigatorPlugin\SpaceNavigatorPlugin.h
	author:		Lars Bilke
	
	purpose:	Definition of the SpaceNavigator plugin module
*********************************************************************/
#ifndef SPACENAVIGATORPLUGIN_HEADER
#define SPACENAVIGATORPLUGIN_HEADER

// ** INCLUDES **
#include "vrModuleBase.h"

class SpaceNavigatorPluginForm;

/**
 * SpaceNavigatorPlugin
 */
class SpaceNavigatorPlugin : public vrModuleBase
{
public:
	SpaceNavigatorPlugin();
	~SpaceNavigatorPlugin();

protected:
	virtual void        init(void);

	virtual bool        receivedMessage(vrMessage &msg);

	static SpaceNavigatorPlugin the;

	virtual QWidget     *getWidget(void) const;

private:
	SpaceNavigatorPluginForm* mainWidget;

};

#endif // SPACENAVIGATORPLUGIN_HEADER