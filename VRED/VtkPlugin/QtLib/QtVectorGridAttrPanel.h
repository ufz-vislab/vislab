/****************************************************************************
** Form interface generated from reading ui file '.\QtVectorGridAttrPanel.ui'
**
** Created: Mo 24. Nov 14:38:05 2008
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CQTVECTORGRIDATTRPANEL_H
#define CQTVECTORGRIDATTRPANEL_H

#include <qvariant.h>
#include <qpixmap.h>
#include "CQtAttrPanelBase.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QTabWidget;
class QWidget;
class QLabel;
class QCheckBox;
class QLineEdit;
class QPushButton;
class QComboBox;
class QSpinBox;

class CQtVectorGridAttrPanel : public CQtAttrPanelBase
{
    Q_OBJECT

public:
    CQtVectorGridAttrPanel( QWidget* parent = 0, const char* name = 0 );
    ~CQtVectorGridAttrPanel();

    QTabWidget* VectorGridTabWidget;
    QWidget* tab;
    QLabel* textLabel1;
    QLabel* MinYLabel;
    QLabel* MinXLabel;
    QLabel* MaxZLabel;
    QLabel* MinZLabel;
    QLabel* MaxYLabel;
    QLabel* MaxXLabel;
    QLabel* MaxVecLengthLabel;
    QLabel* MaxVectorLengthLabel;
    QLabel* ActiveScalarNameLabel;
    QLabel* ActiveVectorNameLabel;
    QLabel* ActiveScalarNameTextLabel;
    QLabel* ActiveVectorNameTextLabel;
    QWidget* tab_2;
    QCheckBox* ShowGlyphsCheckBox;
    QLineEdit* MaskRatioLineEdit;
    QLabel* textLabel14;
    QLabel* textLabel13;
    QLineEdit* GlyphScaleFactorLineEdit;
    QPushButton* ComputeGlyphsButton;
    QWidget* TabPage;
    QCheckBox* ShowStreamlinesCheckBox;
    QLineEdit* StreamlineLengthLineEdit;
    QLabel* textLabel16;
    QLineEdit* TubeRadiusLineEdit;
    QComboBox* StreamlinesDirectionComboBox;
    QLabel* textLabel17;
    QLineEdit* StepLengthLineEdit;
    QLabel* textLabel15;
    QLabel* textLabel18;
    QCheckBox* UseGridAsSourceCheckBox;
    QSpinBox* NumStreamlineSourcesXDirSpinBox;
    QSpinBox* NumStreamlineSourcesYDirSpinBox;
    QSpinBox* NumStreamlineSourcesZDirSpinBox;
    QCheckBox* UseGeometryAsSourceCheckBox;
    QLineEdit* StreamlinesGeomObjectNameLineEdit;
    QPushButton* ComputeStreamlinesButton;

public slots:
    virtual void Update();
    virtual bool SetSubject( CSubject * sub );
    virtual void SetInitialValues();
    virtual void OnShowGlyphsCheckBoxStateChanged();
    virtual void OnShowStreamlinesCheckBoxStateChanged();
    virtual void OnComputeGlyphsButtonClicked();
    virtual void OnComputeStreamlinesButtonClicked();

protected:
    QVBoxLayout* layout15;
    QSpacerItem* spacer65;
    QHBoxLayout* layout69;
    QSpacerItem* spacer64;
    QGridLayout* layout67;
    QGridLayout* layout14;
    QVBoxLayout* layout29;
    QSpacerItem* spacer61;
    QSpacerItem* spacer63;
    QHBoxLayout* layout63;
    QSpacerItem* spacer60;
    QGridLayout* layout28;
    QVBoxLayout* layout65;
    QSpacerItem* spacer57;
    QSpacerItem* spacer58;
    QSpacerItem* spacer59;
    QSpacerItem* spacer62;
    QHBoxLayout* layout59;
    QSpacerItem* spacer53;
    QGridLayout* layout54;
    QHBoxLayout* layout57;
    QSpacerItem* spacer54;
    QHBoxLayout* layout56;
    QSpacerItem* spacer55;
    QSpacerItem* spacer56;
    QHBoxLayout* layout58;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;

    void init();

};

#endif // CQTVECTORGRIDATTRPANEL_H
