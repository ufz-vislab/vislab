/**
 * \file SwitchPlugin.cpp
 * 18/08/2011 LB Initial implementation
 * 
 * Implementation of SwitchPlugin class
 */

// ** INCLUDES **
#include "SwitchPlugin.h"

#include "SwitchPluginForm.h"

#include "vrController.h"
#include "exWindowManager.h"
#include "vrScenegraph.h"

SwitchPlugin SwitchPlugin::the;

SwitchPlugin::SwitchPlugin(void)
  : vrModuleBase("Switch", NULL, VRED_MSG_SELECTED_NODE),
    _mainWidget(NULL)
{
  _mainWidget = new SwitchPluginForm(vrController::getModuleManager(), "SwitchPluginForm");
  
  vrController::getModuleManager()->add("Switch", _mainWidget);
  
  addLoop();
}

SwitchPlugin::~SwitchPlugin(void)
{
  if(!_mainWidget)
    return;
  
  vrController::getModuleManager()->sub(_mainWidget);
}

QWidget* SwitchPlugin::getWidget(void) const
{
  return _mainWidget;
}

bool SwitchPlugin::receivedMessage(vrMessage& message)
{
  if(message.id() == VRED_MSG_SELECTED_NODE)
  {
    OSG::NodePtr selectedNode = vrScenegraph::getSelectedNode();
    _mainWidget->setSwitchNode(selectedNode);
  }
  
  return false;
}

void SwitchPlugin::loop()
{
  _mainWidget->loop();
}