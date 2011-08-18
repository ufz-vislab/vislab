/**
 * \file SwitchPluginForm.h
 * 18/08/2011 LB Initial implementation
 */

#ifndef SWITCHPLUGINFORM_H
#define SWITCHPLUGINFORM_H

#include "SwitchPluginFormBase.h"

#include "vrAEBase.h"
#include "OpenSG/OSGSwitch.h"
#include "OpenSG/OSGNode.h"

/// @brief
class SwitchPluginForm : public SwitchPluginFormBase, vrAEBase
{
  Q_OBJECT
  
public:
  SwitchPluginForm(QWidget* parent = 0, const char* name = 0, WFlags fl = 0);
  ~SwitchPluginForm();
  
  void setSwitchNode(OSG::NodePtr switchNode);
  void loop();

protected:
  OSG::SwitchPtr _switch;

public slots:
  void setChoice(int);

};

#endif // SWITCHPLUGINFORM_H
