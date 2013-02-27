/****************************************************************************
** Form interface generated from reading ui file '.\QtScalarGridAttrPanel.ui'
**
** Created: Mo 24. Nov 16:53:58 2008
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CQTSCALARGRIDATTRPANEL_H
#define CQTSCALARGRIDATTRPANEL_H

#include <qvariant.h>
#include <qpixmap.h>
#include "CQtAttrPanelBase.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QComboBox;
class QCheckBox;
class QSpinBox;
class QLineEdit;
class QSlider;

class CQtScalarGridAttrPanel : public CQtAttrPanelBase
{
    Q_OBJECT

public:
    CQtScalarGridAttrPanel( QWidget* parent = 0, const char* name = 0 );
    ~CQtScalarGridAttrPanel();

    QLabel* textLabel2;
    QComboBox* ContourAttrNameComboBox;
    QLabel* Textlabe1;
    QComboBox* ColorAttrNameComboBox;
    QLabel* textLabel3;
    QCheckBox* ShowContoursCheckBox;
    QSpinBox* NumContoursSpinBox;
    QLineEdit* MaxContourLineEdit;
    QLabel* DataValMaxLabel;
    QLabel* DataValMinLabel;
    QLineEdit* MinContourLineEdit;
    QSlider* MinContourSlider;
    QSlider* MaxContourSlider;
    QLabel* textLabel4;
    QCheckBox* ShowXSectionCheckBox;
    QLabel* XMinLabel;
    QLineEdit* XSectionPositionLineEdit;
    QLabel* XMaxLabel;
    QSlider* XSectionSlider;
    QLabel* textLabel5;
    QCheckBox* ShowYSectionCheckBox;
    QLabel* YMinLabel;
    QLineEdit* YSectionPositionLineEdit;
    QLabel* YMaxLabel;
    QSlider* YSectionSlider;
    QLabel* textLabel6;
    QCheckBox* ShowZSectionCheckBox;
    QLabel* ZMinLabel;
    QLineEdit* ZSectionPositionLineEdit;
    QLabel* ZMaxLabel;
    QSlider* ZSectionSlider;

public slots:
    virtual void Update();
    virtual bool SetSubject( CSubject * sub );
    virtual void SetInitialValues();
    virtual void OnShowContoursCheckBoxToggled();
    virtual void OnShowXSectionCheckBoxToggled();
    virtual void OnShowYSectionCheckBoxToggled();
    virtual void OnShowZSectionCheckBoxToggled();
    virtual void OnNumContoursSpinboxValueChanged();
    virtual void OnMinContourSliderValueChanged();
    virtual void OnMaxContourSliderValueChanged();
    virtual void OnXSectionSliderValueChanged();
    virtual void OnYSectionSliderValueChanged();
    virtual void OnZSectionSliderValueChanged();
    virtual void OnContourDataValueChanged();
    virtual void OnColorAttrValueComboBoxTextChanged();
    virtual void OnContourAttrNameComboBoxActivated();

protected:
    QVBoxLayout* layout15;
    QGridLayout* layout14;
    QVBoxLayout* layout33;
    QHBoxLayout* layout16;
    QSpacerItem* spacer15;
    QGridLayout* layout32;
    QVBoxLayout* layout29;
    QHBoxLayout* layout19;
    QSpacerItem* spacer15_2;
    QHBoxLayout* layout20;
    QSpacerItem* spacer19;
    QSpacerItem* spacer19_2;
    QVBoxLayout* layout30;
    QHBoxLayout* layout22;
    QSpacerItem* spacer15_3;
    QHBoxLayout* layout23;
    QSpacerItem* spacer19_3;
    QSpacerItem* spacer19_4;
    QVBoxLayout* layout31;
    QHBoxLayout* layout25;
    QSpacerItem* spacer15_4;
    QHBoxLayout* layout26;
    QSpacerItem* spacer19_3_2;
    QSpacerItem* spacer19_3_3;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;

    void init();

};

#endif // CQTSCALARGRIDATTRPANEL_H
