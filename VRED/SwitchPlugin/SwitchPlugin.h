/**
 * \file SwitchPlugin.h
 * 18/08/2011 LB Initial implementation
 */

#ifndef SWITCHPLUGIN_H
#define SWITCHPLUGIN_H

#include <vrModuleBase.h>

class SwitchPluginForm;
 
/// @brief
class SwitchPlugin : public vrModuleBase
{
public:
  SwitchPlugin();
  ~SwitchPlugin();

protected:
  virtual QWidget* getWidget(void) const;
  //virtual void init(void);
  virtual bool receivedMessage(vrMessage& message);
  virtual void loop();
  
  static SwitchPlugin the;

private:
  SwitchPluginForm* _mainWidget;
};

#endif // SWITCHPLUGIN_H
