/**
 * \file SwitchPluginForm.h
 * 18/08/2011 LB Initial implementation
 */

#ifndef SWITCHPLUGINFORM_H
#define SWITCHPLUGINFORM_H

class QTime;

#include "SwitchPluginFormBase.h"

#include "vrAEBase.h"
#include "OpenSG/OSGSwitch.h"
#include "OpenSG/OSGNode.h"
#include "qsettings.h"

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
  int _maxChoice;
  bool _playing;
  QTime* _timer;
  QSettings _settings;

protected slots:
  void setChoice(int);
  void playOrStop();
  void setLoop(bool loop);
  void setSpeed(int speed);

};

#endif // SWITCHPLUGINFORM_H
