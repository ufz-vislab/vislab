/****************************************************************************
** Form implementation generated from reading ui file '.\WPEAttrPanel.ui'
**
** Created: Mi 5. Nov 18:52:59 2008
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "WPEAttrPanel.h"

#include <qvariant.h>
#include <qcheckbox.h>
#include <qdial.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "WPEProjectController.h"
#include "qfiledialog.h"
#include "WPETypeInfo.h"
#include "WPEAttrPanel.ui.h"
/*
 *  Constructs a CWPEAttrPanel as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
CWPEAttrPanel::CWPEAttrPanel( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "CWPEAttrPanel" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)7, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setMinimumSize( QSize( 150, 0 ) );

    QWidget* privateLayoutWidget = new QWidget( this, "layout3" );
    privateLayoutWidget->setGeometry( QRect( 19, 22, 191, 728 ) );
    layout3 = new QVBoxLayout( privateLayoutWidget, 11, 6, "layout3"); 

    UseWPERotorsCheckBox = new QCheckBox( privateLayoutWidget, "UseWPERotorsCheckBox" );
    layout3->addWidget( UseWPERotorsCheckBox );

    WinddirectionDial = new QDial( privateLayoutWidget, "WinddirectionDial" );
    WinddirectionDial->setMinimumSize( QSize( 100, 0 ) );
    WinddirectionDial->setWrapping( TRUE );
    WinddirectionDial->setNotchTarget( 6 );
    WinddirectionDial->setNotchesVisible( TRUE );
    WinddirectionDial->setMaxValue( 360 );
    layout3->addWidget( WinddirectionDial );
    spacer5 = new QSpacerItem( 20, 60, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout3->addItem( spacer5 );

    ScaleUpButton = new QPushButton( privateLayoutWidget, "ScaleUpButton" );
    ScaleUpButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, ScaleUpButton->sizePolicy().hasHeightForWidth() ) );
    ScaleUpButton->setMinimumSize( QSize( 100, 0 ) );
    layout3->addWidget( ScaleUpButton );

    ScaleDownButton = new QPushButton( privateLayoutWidget, "ScaleDownButton" );
    ScaleDownButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, ScaleDownButton->sizePolicy().hasHeightForWidth() ) );
    ScaleDownButton->setMinimumSize( QSize( 100, 0 ) );
    layout3->addWidget( ScaleDownButton );
    spacer3 = new QSpacerItem( 20, 50, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout3->addItem( spacer3 );

    AddItemButton = new QPushButton( privateLayoutWidget, "AddItemButton" );
    AddItemButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, AddItemButton->sizePolicy().hasHeightForWidth() ) );
    AddItemButton->setMinimumSize( QSize( 100, 0 ) );
    layout3->addWidget( AddItemButton );

    RemoveItemButton = new QPushButton( privateLayoutWidget, "RemoveItemButton" );
    RemoveItemButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, RemoveItemButton->sizePolicy().hasHeightForWidth() ) );
    RemoveItemButton->setMinimumSize( QSize( 100, 0 ) );
    layout3->addWidget( RemoveItemButton );
    spacer2 = new QSpacerItem( 20, 50, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout3->addItem( spacer2 );

    textLabel1 = new QLabel( privateLayoutWidget, "textLabel1" );
    layout3->addWidget( textLabel1 );

    WPETypeNamesCombo = new QComboBox( FALSE, privateLayoutWidget, "WPETypeNamesCombo" );
    WPETypeNamesCombo->setMinimumSize( QSize( 100, 0 ) );
    layout3->addWidget( WPETypeNamesCombo );

    textLabel1_2 = new QLabel( privateLayoutWidget, "textLabel1_2" );
    layout3->addWidget( textLabel1_2 );

    GeneratorHeightLineEdit = new QLineEdit( privateLayoutWidget, "GeneratorHeightLineEdit" );
    GeneratorHeightLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, GeneratorHeightLineEdit->sizePolicy().hasHeightForWidth() ) );
    GeneratorHeightLineEdit->setMinimumSize( QSize( 100, 0 ) );
    layout3->addWidget( GeneratorHeightLineEdit );

    textLabel2 = new QLabel( privateLayoutWidget, "textLabel2" );
    layout3->addWidget( textLabel2 );

    RotorLengthLineEdit = new QLineEdit( privateLayoutWidget, "RotorLengthLineEdit" );
    RotorLengthLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, RotorLengthLineEdit->sizePolicy().hasHeightForWidth() ) );
    RotorLengthLineEdit->setMinimumSize( QSize( 100, 0 ) );
    layout3->addWidget( RotorLengthLineEdit );

    textLabel1_3 = new QLabel( privateLayoutWidget, "textLabel1_3" );
    layout3->addWidget( textLabel1_3 );

    WPEMWHLineEdit = new QLineEdit( privateLayoutWidget, "WPEMWHLineEdit" );
    WPEMWHLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, WPEMWHLineEdit->sizePolicy().hasHeightForWidth() ) );
    layout3->addWidget( WPEMWHLineEdit );
    spacer4 = new QSpacerItem( 20, 50, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout3->addItem( spacer4 );

    textLabel2_2 = new QLabel( privateLayoutWidget, "textLabel2_2" );
    layout3->addWidget( textLabel2_2 );

    textLabel1_4 = new QLabel( privateLayoutWidget, "textLabel1_4" );
    layout3->addWidget( textLabel1_4 );

    WindparkProductionLineEdit = new QLineEdit( privateLayoutWidget, "WindparkProductionLineEdit" );
    WindparkProductionLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, WindparkProductionLineEdit->sizePolicy().hasHeightForWidth() ) );
    WindparkProductionLineEdit->setMinimumSize( QSize( 100, 0 ) );
    WindparkProductionLineEdit->setPaletteForegroundColor( QColor( 255, 0, 0 ) );
    QFont WindparkProductionLineEdit_font(  WindparkProductionLineEdit->font() );
    WindparkProductionLineEdit_font.setPointSize( 14 );
    WindparkProductionLineEdit_font.setBold( TRUE );
    WindparkProductionLineEdit->setFont( WindparkProductionLineEdit_font ); 
    layout3->addWidget( WindparkProductionLineEdit );
    languageChange();
    resize( QSize(305, 915).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( AddItemButton, SIGNAL( pressed() ), this, SLOT( OnAddItemButtonPressed() ) );
    connect( RemoveItemButton, SIGNAL( pressed() ), this, SLOT( OnRemoveActiveItemButton() ) );
    connect( ScaleDownButton, SIGNAL( pressed() ), this, SLOT( OnScaleDownButtonPressed() ) );
    connect( ScaleUpButton, SIGNAL( pressed() ), this, SLOT( OnScaleUpButtonPressed() ) );
    connect( WPETypeNamesCombo, SIGNAL( activated(int) ), this, SLOT( OnTypeNamesComboTextChanged() ) );
    connect( WinddirectionDial, SIGNAL( valueChanged(int) ), this, SLOT( OnWinddirectionDialValueChanged() ) );
    connect( UseWPERotorsCheckBox, SIGNAL( toggled(bool) ), this, SLOT( OnUseWPERotorsCheckBoxToggled() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
CWPEAttrPanel::~CWPEAttrPanel()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CWPEAttrPanel::languageChange()
{
    setCaption( tr( "WPEAttrPanel" ) );
    UseWPERotorsCheckBox->setText( tr( "Rotoren an" ) );
    ScaleUpButton->setText( tr( "Groesser" ) );
    ScaleDownButton->setText( tr( "Kleiner" ) );
    AddItemButton->setText( tr( "Neue Anlage" ) );
    RemoveItemButton->setText( tr( "Anlage entfernen" ) );
    textLabel1->setText( tr( "Anlagentyp" ) );
    textLabel1_2->setText( trUtf8( "\x4e\x61\x62\x65\x6e\x68\xc3\xb6\x68\x65" ) );
    textLabel2->setText( tr( "Rotorradius" ) );
    textLabel1_3->setText( tr( "Nennleistung (MW)" ) );
    textLabel2_2->setText( QString::null );
    textLabel1_4->setText( tr( "Nennleistung des Windparks (MW)" ) );
}

