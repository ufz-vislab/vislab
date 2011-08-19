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
#include "qpushbutton.h"
#include "qdatetime.h"
#include "qcheckbox.h"

SwitchPluginForm::SwitchPluginForm(QWidget* parent, const char* name, WFlags fl)
  : SwitchPluginFormBase(parent, name, fl), _maxChoice(-1), _playing(false)
{
  setEnabled(false);
  choiceSpinBox->setMinValue(0);
  choiceSlider->setMinValue(0);
  _timer = new QTime();
  _settings.setPath("UFZ", "VRED-SwitchPlugin");
  std::cout << "Reading " << _settings.readBoolEntry("loop") << std::endl;
  loopCheckBox->setChecked(_settings.readBoolEntry("loop"));
  speedSlider->setValue(_settings.readDoubleEntry("speed", 0.0));
}

SwitchPluginForm::~SwitchPluginForm()
{
}

void SwitchPluginForm::loop()
{
	if(_playing && _timer->elapsed() > speedSlider->value())
	{
		int currentChoice = _switch->getChoice();
		if(currentChoice < _maxChoice)
		{
			choiceSpinBox->setValue(currentChoice + 1);
			update();
			_timer->restart();
		}
		else
		{
			if(loopCheckBox->isChecked())
			{
				choiceSpinBox->setValue(0);
				update();
				_timer->restart();
			}
			else
				playOrStop();
		}
	}
}

void SwitchPluginForm::setSwitchNode(OSG::NodePtr switchNode)
{
  if(switchNode != OSG::NullFC)
  {
    // Dont enable for switch with one or zero children
    if(switchNode->getNChildren() < 2)
	  _switch = OSG::NullFC;
	else
      _switch = OSG::SwitchPtr::dcast(switchNode->getCore());
  }
  else
    _switch == OSG::NullFC;
    
  if(_switch == OSG::NullFC)
  {
    // Disable form
    setEnabled(false);
	_playing = false;
	playPushButton->setText("Play");
  }
  else
  {
    // Enable form
    setEnabled(true);
    int choice = _switch->getChoice();
    _maxChoice = switchNode->getNChildren() -1;
    
    // Set GUI elements
    
    choiceSpinBox->setMaxValue(_maxChoice);
    choiceSpinBox->setValue(choice);
    
    choiceSlider->setMaxValue(_maxChoice);
    choiceSlider->setValue(choice);
  }
}

void SwitchPluginForm::setChoice(int choice)
{
  beginEditCP(_switch, OSG::Switch::ChoiceFieldMask);
  _switch->setChoice(choice);
  endEditCP(_switch, OSG::Switch::ChoiceFieldMask);
}

void SwitchPluginForm::playOrStop()
{
	_playing = !_playing;
	if(_playing)
	{
		playPushButton->setText("Stop");
		// If animation at the end play back from the beginning
		if(_switch->getChoice() >= _maxChoice)
			choiceSpinBox->setValue(0);
		_timer->restart();
	}
	else
	{
		playPushButton->setText("Play");
	}
}

void SwitchPluginForm::setLoop(bool loop)
{
	_settings.writeEntry("loop", loop);
}

void SwitchPluginForm::setSpeed(int speed)
{
	_settings.writeEntry("speed", (double)speed);
}