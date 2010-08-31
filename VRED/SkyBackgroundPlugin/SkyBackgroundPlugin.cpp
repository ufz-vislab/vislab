/********************************************************************
	created:	1.7.2009   13:52
	filename: 	e:\bilke\apps\vred-2.5\dev\SkyBackgroundPlugin\SkyBackgroundPlugin.cpp
	author:		Lars Bilke
	
	purpose:	Implementation of the sky background plugin
*********************************************************************/

// ** INCLUDES **
#include "SkyBackgroundPlugin.h"
#include "SkyBackgroundPluginForm.h"

#include "vrController.h"
#include "exWindowManager.h"


SkyBackgroundPlugin SkyBackgroundPlugin::the;

SkyBackgroundPlugin::SkyBackgroundPlugin() : vrModuleBase("SkyBackground"), mainWidget(NULL)
{
	mainWidget = new SkyBackgroundPluginForm(vrController::getModuleManager(), "SkyBackgroundPluginForm");

	vrController::getModuleManager()->add("SkyBackground", mainWidget);
}

SkyBackgroundPlugin::~SkyBackgroundPlugin()
{
	if(mainWidget == NULL)
		return;

	vrController::getModuleManager()->sub(mainWidget);
}

QWidget     * SkyBackgroundPlugin::getWidget( void ) const
{
	return mainWidget;
}

void SkyBackgroundPlugin::init( void )
{

}

bool SkyBackgroundPlugin::receivedMessage( vrMessage &msg )
{
	return false;
}