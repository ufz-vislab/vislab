/****************************************************************************
** Form interface generated from reading ui file '.\WPEAttrPanel.ui'
**
** Created: Mi 5. Nov 18:52:59 2008
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CWPEATTRPANEL_H
#define CWPEATTRPANEL_H

#include <qvariant.h>
#include <qwidget.h>
#include "WPETypeInfo.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QCheckBox;
class QDial;
class QPushButton;
class QLabel;
class QComboBox;
class QLineEdit;

class CWPEAttrPanel : public QWidget
{
    Q_OBJECT

public:
    CWPEAttrPanel( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~CWPEAttrPanel();

    QCheckBox* UseWPERotorsCheckBox;
    QDial* WinddirectionDial;
    QPushButton* ScaleUpButton;
    QPushButton* ScaleDownButton;
    QPushButton* AddItemButton;
    QPushButton* RemoveItemButton;
    QLabel* textLabel1;
    QComboBox* WPETypeNamesCombo;
    QLabel* textLabel1_2;
    QLineEdit* GeneratorHeightLineEdit;
    QLabel* textLabel2;
    QLineEdit* RotorLengthLineEdit;
    QLabel* textLabel1_3;
    QLineEdit* WPEMWHLineEdit;
    QLabel* textLabel2_2;
    QLabel* textLabel1_4;
    QLineEdit* WindparkProductionLineEdit;

public slots:
    virtual void OnAddItemButtonPressed();
    virtual void OnRemoveActiveItemButton();
    virtual void OnLoadArealImageButton();
    virtual void OnScaleDownButtonPressed();
    virtual void OnScaleUpButtonPressed();
    virtual void AddTypeNameToTypeNamesCombo( const char * typeName );
    virtual const char * GetCurrentTypeNameFromTypeNamesCombo();
    virtual void OnTypeNamesComboTextChanged();
    virtual void OnLoadWPETypesButtonPressed();
    virtual void OnLoadTerrainButtonPressed();
    virtual void OnSet3DModelFileNameButtonPressed();
    virtual void SetCurrentWPEType( CWPETypeInfo wpeType );
    virtual void SetWPETypeAttributes( CWPETypeInfo typeInfo );
    virtual void SetEnergyProduction( float production );
    virtual void OnSaveWindparkButtonPressed();
    virtual void OnWinddirectionDialValueChanged();
    virtual float GetWinddirection();
    virtual void OnUseWPERotorsCheckBoxToggled();
    virtual void OnLoadLandscapeButtonPressed();

protected:
    QVBoxLayout* layout3;
    QSpacerItem* spacer5;
    QSpacerItem* spacer3;
    QSpacerItem* spacer2;
    QSpacerItem* spacer4;

protected slots:
    virtual void languageChange();

};

#endif // CWPEATTRPANEL_H
