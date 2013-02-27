/****************************************************************************
** Form implementation generated from reading ui file '.\QtScalarGridAttrPanel.ui'
**
** Created: Mo 24. Nov 16:53:58 2008
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "QtScalarGridAttrPanel.h"

#include <qvariant.h>
#include "CQtAttrPanelBase.h"
#include <qpushbutton.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qspinbox.h>
#include <qlineedit.h>
#include <qslider.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "ScalarGridState.h"
#include "ScalarUnstructuredGrid.h"
#include "string"
#include "list"
#include "QtScalarGridAttrPanel.ui.h"
/*
 *  Constructs a CQtScalarGridAttrPanel which is a child of 'parent', with the
 *  name 'name'.' 
 */
CQtScalarGridAttrPanel::CQtScalarGridAttrPanel( QWidget* parent,  const char* name )
    : CQtAttrPanelBase( parent, name )
{
    if ( !name )
	setName( "CQtScalarGridAttrPanel" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setMinimumSize( QSize( 320, 500 ) );
    setMaximumSize( QSize( 320, 32767 ) );

    QWidget* privateLayoutWidget = new QWidget( this, "layout15" );
    privateLayoutWidget->setGeometry( QRect( 4, 5, 305, 431 ) );
    layout15 = new QVBoxLayout( privateLayoutWidget, 4, 0, "layout15"); 

    layout14 = new QGridLayout( 0, 1, 1, 0, 6, "layout14"); 

    textLabel2 = new QLabel( privateLayoutWidget, "textLabel2" );
    textLabel2->setMinimumSize( QSize( 110, 0 ) );

    layout14->addWidget( textLabel2, 1, 0 );

    ContourAttrNameComboBox = new QComboBox( FALSE, privateLayoutWidget, "ContourAttrNameComboBox" );

    layout14->addWidget( ContourAttrNameComboBox, 0, 1 );

    Textlabe1 = new QLabel( privateLayoutWidget, "Textlabe1" );
    Textlabe1->setMinimumSize( QSize( 110, 0 ) );

    layout14->addWidget( Textlabe1, 0, 0 );

    ColorAttrNameComboBox = new QComboBox( FALSE, privateLayoutWidget, "ColorAttrNameComboBox" );

    layout14->addWidget( ColorAttrNameComboBox, 1, 1 );
    layout15->addLayout( layout14 );

    layout33 = new QVBoxLayout( 0, 0, 6, "layout33"); 

    layout16 = new QHBoxLayout( 0, 0, 6, "layout16"); 

    textLabel3 = new QLabel( privateLayoutWidget, "textLabel3" );
    layout16->addWidget( textLabel3 );

    ShowContoursCheckBox = new QCheckBox( privateLayoutWidget, "ShowContoursCheckBox" );
    layout16->addWidget( ShowContoursCheckBox );

    NumContoursSpinBox = new QSpinBox( privateLayoutWidget, "NumContoursSpinBox" );
    layout16->addWidget( NumContoursSpinBox );
    spacer15 = new QSpacerItem( 140, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout16->addItem( spacer15 );
    layout33->addLayout( layout16 );

    layout32 = new QGridLayout( 0, 1, 1, 0, 6, "layout32"); 

    MaxContourLineEdit = new QLineEdit( privateLayoutWidget, "MaxContourLineEdit" );
    MaxContourLineEdit->setMinimumSize( QSize( 90, 0 ) );

    layout32->addWidget( MaxContourLineEdit, 1, 1 );

    DataValMaxLabel = new QLabel( privateLayoutWidget, "DataValMaxLabel" );

    layout32->addWidget( DataValMaxLabel, 0, 1 );

    DataValMinLabel = new QLabel( privateLayoutWidget, "DataValMinLabel" );

    layout32->addWidget( DataValMinLabel, 0, 0 );

    MinContourLineEdit = new QLineEdit( privateLayoutWidget, "MinContourLineEdit" );
    MinContourLineEdit->setMinimumSize( QSize( 90, 0 ) );

    layout32->addWidget( MinContourLineEdit, 1, 0 );
    layout33->addLayout( layout32 );

    MinContourSlider = new QSlider( privateLayoutWidget, "MinContourSlider" );
    MinContourSlider->setMaxValue( 500 );
    MinContourSlider->setOrientation( QSlider::Horizontal );
    layout33->addWidget( MinContourSlider );

    MaxContourSlider = new QSlider( privateLayoutWidget, "MaxContourSlider" );
    MaxContourSlider->setMaxValue( 500 );
    MaxContourSlider->setValue( 500 );
    MaxContourSlider->setOrientation( QSlider::Horizontal );
    layout33->addWidget( MaxContourSlider );
    layout15->addLayout( layout33 );

    layout29 = new QVBoxLayout( 0, 0, 6, "layout29"); 

    layout19 = new QHBoxLayout( 0, 0, 6, "layout19"); 

    textLabel4 = new QLabel( privateLayoutWidget, "textLabel4" );
    layout19->addWidget( textLabel4 );

    ShowXSectionCheckBox = new QCheckBox( privateLayoutWidget, "ShowXSectionCheckBox" );
    layout19->addWidget( ShowXSectionCheckBox );
    spacer15_2 = new QSpacerItem( 190, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout19->addItem( spacer15_2 );
    layout29->addLayout( layout19 );

    layout20 = new QHBoxLayout( 0, 0, 6, "layout20"); 

    XMinLabel = new QLabel( privateLayoutWidget, "XMinLabel" );
    layout20->addWidget( XMinLabel );
    spacer19 = new QSpacerItem( 30, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout20->addItem( spacer19 );

    XSectionPositionLineEdit = new QLineEdit( privateLayoutWidget, "XSectionPositionLineEdit" );
    XSectionPositionLineEdit->setMinimumSize( QSize( 80, 0 ) );
    XSectionPositionLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    layout20->addWidget( XSectionPositionLineEdit );
    spacer19_2 = new QSpacerItem( 30, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout20->addItem( spacer19_2 );

    XMaxLabel = new QLabel( privateLayoutWidget, "XMaxLabel" );
    layout20->addWidget( XMaxLabel );
    layout29->addLayout( layout20 );

    XSectionSlider = new QSlider( privateLayoutWidget, "XSectionSlider" );
    XSectionSlider->setMaxValue( 500 );
    XSectionSlider->setValue( 250 );
    XSectionSlider->setOrientation( QSlider::Horizontal );
    layout29->addWidget( XSectionSlider );
    layout15->addLayout( layout29 );

    layout30 = new QVBoxLayout( 0, 0, 6, "layout30"); 

    layout22 = new QHBoxLayout( 0, 0, 6, "layout22"); 

    textLabel5 = new QLabel( privateLayoutWidget, "textLabel5" );
    layout22->addWidget( textLabel5 );

    ShowYSectionCheckBox = new QCheckBox( privateLayoutWidget, "ShowYSectionCheckBox" );
    layout22->addWidget( ShowYSectionCheckBox );
    spacer15_3 = new QSpacerItem( 190, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout22->addItem( spacer15_3 );
    layout30->addLayout( layout22 );

    layout23 = new QHBoxLayout( 0, 0, 6, "layout23"); 

    YMinLabel = new QLabel( privateLayoutWidget, "YMinLabel" );
    layout23->addWidget( YMinLabel );
    spacer19_3 = new QSpacerItem( 30, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout23->addItem( spacer19_3 );

    YSectionPositionLineEdit = new QLineEdit( privateLayoutWidget, "YSectionPositionLineEdit" );
    YSectionPositionLineEdit->setMinimumSize( QSize( 80, 0 ) );
    YSectionPositionLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    layout23->addWidget( YSectionPositionLineEdit );
    spacer19_4 = new QSpacerItem( 30, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout23->addItem( spacer19_4 );

    YMaxLabel = new QLabel( privateLayoutWidget, "YMaxLabel" );
    layout23->addWidget( YMaxLabel );
    layout30->addLayout( layout23 );

    YSectionSlider = new QSlider( privateLayoutWidget, "YSectionSlider" );
    YSectionSlider->setMaxValue( 500 );
    YSectionSlider->setValue( 250 );
    YSectionSlider->setOrientation( QSlider::Horizontal );
    layout30->addWidget( YSectionSlider );
    layout15->addLayout( layout30 );

    layout31 = new QVBoxLayout( 0, 0, 6, "layout31"); 

    layout25 = new QHBoxLayout( 0, 0, 6, "layout25"); 

    textLabel6 = new QLabel( privateLayoutWidget, "textLabel6" );
    layout25->addWidget( textLabel6 );

    ShowZSectionCheckBox = new QCheckBox( privateLayoutWidget, "ShowZSectionCheckBox" );
    layout25->addWidget( ShowZSectionCheckBox );
    spacer15_4 = new QSpacerItem( 190, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout25->addItem( spacer15_4 );
    layout31->addLayout( layout25 );

    layout26 = new QHBoxLayout( 0, 0, 6, "layout26"); 

    ZMinLabel = new QLabel( privateLayoutWidget, "ZMinLabel" );
    layout26->addWidget( ZMinLabel );
    spacer19_3_2 = new QSpacerItem( 30, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout26->addItem( spacer19_3_2 );

    ZSectionPositionLineEdit = new QLineEdit( privateLayoutWidget, "ZSectionPositionLineEdit" );
    ZSectionPositionLineEdit->setMinimumSize( QSize( 80, 0 ) );
    ZSectionPositionLineEdit->setMaximumSize( QSize( 80, 32767 ) );
    layout26->addWidget( ZSectionPositionLineEdit );
    spacer19_3_3 = new QSpacerItem( 30, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout26->addItem( spacer19_3_3 );

    ZMaxLabel = new QLabel( privateLayoutWidget, "ZMaxLabel" );
    layout26->addWidget( ZMaxLabel );
    layout31->addLayout( layout26 );

    ZSectionSlider = new QSlider( privateLayoutWidget, "ZSectionSlider" );
    ZSectionSlider->setMaxValue( 500 );
    ZSectionSlider->setValue( 250 );
    ZSectionSlider->setOrientation( QSlider::Horizontal );
    layout31->addWidget( ZSectionSlider );
    layout15->addLayout( layout31 );
    languageChange();
    resize( QSize(320, 500).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( ShowContoursCheckBox, SIGNAL( toggled(bool) ), this, SLOT( OnShowContoursCheckBoxToggled() ) );
    connect( ShowXSectionCheckBox, SIGNAL( toggled(bool) ), this, SLOT( OnShowXSectionCheckBoxToggled() ) );
    connect( ShowYSectionCheckBox, SIGNAL( toggled(bool) ), this, SLOT( OnShowYSectionCheckBoxToggled() ) );
    connect( ShowZSectionCheckBox, SIGNAL( toggled(bool) ), this, SLOT( OnShowZSectionCheckBoxToggled() ) );
    connect( NumContoursSpinBox, SIGNAL( valueChanged(int) ), this, SLOT( OnNumContoursSpinboxValueChanged() ) );
    connect( MinContourSlider, SIGNAL( valueChanged(int) ), this, SLOT( OnContourDataValueChanged() ) );
    connect( MaxContourSlider, SIGNAL( valueChanged(int) ), this, SLOT( OnContourDataValueChanged() ) );
    connect( NumContoursSpinBox, SIGNAL( valueChanged(int) ), this, SLOT( OnContourDataValueChanged() ) );
    connect( XSectionSlider, SIGNAL( valueChanged(int) ), this, SLOT( OnXSectionSliderValueChanged() ) );
    connect( YSectionSlider, SIGNAL( valueChanged(int) ), this, SLOT( OnYSectionSliderValueChanged() ) );
    connect( ZSectionSlider, SIGNAL( valueChanged(int) ), this, SLOT( OnZSectionSliderValueChanged() ) );
    connect( ColorAttrNameComboBox, SIGNAL( activated(const QString&) ), this, SLOT( OnColorAttrValueComboBoxTextChanged() ) );
    connect( ContourAttrNameComboBox, SIGNAL( activated(int) ), this, SLOT( OnContourAttrNameComboBoxActivated() ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
CQtScalarGridAttrPanel::~CQtScalarGridAttrPanel()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CQtScalarGridAttrPanel::languageChange()
{
    setCaption( tr( "CQtAttrPanelBaseForm" ) );
    textLabel2->setText( tr( "Attribute for colors" ) );
    Textlabe1->setText( tr( "Attribute for contours" ) );
    textLabel3->setText( tr( "Data       " ) );
    ShowContoursCheckBox->setText( tr( "show" ) );
    DataValMaxLabel->setText( tr( "textLabel2" ) );
    DataValMinLabel->setText( tr( "textLabel1" ) );
    textLabel4->setText( tr( "X-Section" ) );
    ShowXSectionCheckBox->setText( tr( "show" ) );
    XMinLabel->setText( tr( "textLabel1" ) );
    XMaxLabel->setText( tr( "textLabel2" ) );
    textLabel5->setText( tr( "Y-Section" ) );
    ShowYSectionCheckBox->setText( tr( "show" ) );
    YMinLabel->setText( tr( "textLabel1" ) );
    YMaxLabel->setText( tr( "textLabel2" ) );
    textLabel6->setText( tr( "Z-Section" ) );
    ShowZSectionCheckBox->setText( tr( "show" ) );
    ZMinLabel->setText( tr( "textLabel1" ) );
    ZMaxLabel->setText( tr( "textLabel2" ) );
}

