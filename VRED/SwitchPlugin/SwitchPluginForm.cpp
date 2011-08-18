/**
 * \file SwitchPluginForm.cpp
 * 18/08/2011 LB Initial implementation
 * 
 * Implementation of SwitchPluginForm class
 */

// ** INCLUDES **
#include "SwitchPluginForm.h"

#include "qspinbox.h"
#include "qslider.h"

SwitchPluginForm::SwitchPluginForm(QWidget* parent, const char* name, WFlags fl)
  : SwitchPluginFormBase(parent, name, fl)
{
  this->choiceSpinBox->setMinValue(0);
  this->choiceSlider->setMinValue(0);
}

SwitchPluginForm::~SwitchPluginForm()
{
}

void SwitchPluginForm::loop()
{

}

void SwitchPluginForm::setSwitchNode(OSG::NodePtr switchNode)
{
  if(switchNode != OSG::NullFC)
    _switch = OSG::SwitchPtr::dcast(switchNode->getCore());
  else
    _switch == OSG::NullFC;
    
  if(_switch == OSG::NullFC)
  {
    // Disable form
    this->setEnabled(false);
  }
  else
  {
    // Enable form
    this->setEnabled(true);
    int choice = _switch->getChoice();
    int maxChoice = switchNode->getNChildren() -1;
    
    // Set GUI elements
    
    this->choiceSpinBox->setMaxValue(maxChoice);
    this->choiceSpinBox->setValue(choice);
    
    this->choiceSlider->setMaxValue(maxChoice);
    this->choiceSlider->setValue(choice);
  }
}

void SwitchPluginForm::setChoice(int choice)
{
  beginEditCP(_switch, OSG::Switch::ChoiceFieldMask);
  _switch->setChoice(choice);
  endEditCP(_switch, OSG::Switch::ChoiceFieldMask);
}