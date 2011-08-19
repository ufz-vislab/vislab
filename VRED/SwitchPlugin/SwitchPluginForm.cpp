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
  this->choiceSpinBox->setMinValue(0);
  this->choiceSlider->setMinValue(0);
  this->_timer = new QTime();
}

SwitchPluginForm::~SwitchPluginForm()
{
}

void SwitchPluginForm::loop()
{
	if(_playing && _timer->elapsed() > this->speedSlider->value())
	{
		int currentChoice = _switch->getChoice();
		if(currentChoice < _maxChoice)
		{
			this->choiceSpinBox->setValue(currentChoice + 1);
			this->update();
			//setChoice(currentChoice + 1);
			_timer->restart();
		}
		else
		{
			if(loopCheckBox->isChecked())
			{
				this->choiceSpinBox->setValue(0);
				this->update();
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
    _switch = OSG::SwitchPtr::dcast(switchNode->getCore());
  else
    _switch == OSG::NullFC;
    
  if(_switch == OSG::NullFC)
  {
    // Disable form
    this->setEnabled(false);
	_playing = false;
	this->playPushButton->setText("Play");
  }
  else
  {
    // Enable form
    this->setEnabled(true);
    int choice = _switch->getChoice();
    _maxChoice = switchNode->getNChildren() -1;
    
    // Set GUI elements
    
    this->choiceSpinBox->setMaxValue(_maxChoice);
    this->choiceSpinBox->setValue(choice);
    
    this->choiceSlider->setMaxValue(_maxChoice);
    this->choiceSlider->setValue(choice);
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
		this->playPushButton->setText("Stop");
		// If animation at the end play back from the beginning
		if(_switch->getChoice() >= _maxChoice)
			this->choiceSpinBox->setValue(0);
		_timer->restart();
	}
	else
	{
		this->playPushButton->setText("Play");
	}
}