/********************************************************************
	created:	13.7.2009   14:17
	filename: 	e:\bilke\projects\SpaceNavigator\SpaceNavigatorPlugin\SpaceNavigatorPlugin.cpp
	author:		Lars Bilke
	
	purpose:	Implementation of the SpaceNavigator plugin module
*********************************************************************/

// ** INCLUDES **
#include "SpaceNavigatorPlugin.h"
#include "SpaceNavigatorPluginForm.h"

#include "vrController.h"
#include "exWindowManager.h"

SpaceNavigatorPlugin SpaceNavigatorPlugin::the;

SpaceNavigatorPlugin::SpaceNavigatorPlugin() : vrModuleBase("SpaceNavigator"), mainWidget(NULL)
{
	mainWidget = new SpaceNavigatorPluginForm(vrController::getModuleManager(), "SpaceNavigatorPluginForm");
	vrController::getModuleManager()->add("SpaceNavigator", mainWidget);
}

SpaceNavigatorPlugin::~SpaceNavigatorPlugin()
{
	if (mainWidget == NULL)
		return;

	vrController::getModuleManager()->sub(mainWidget);
}

void SpaceNavigatorPlugin::init( void )
{

}

bool SpaceNavigatorPlugin::receivedMessage( vrMessage &msg )
{
	return false;
}

QWidget     * SpaceNavigatorPlugin::getWidget( void ) const
{
	return mainWidget;
}