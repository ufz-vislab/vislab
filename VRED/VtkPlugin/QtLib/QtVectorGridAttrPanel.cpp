/****************************************************************************
** Form implementation generated from reading ui file '.\QtVectorGridAttrPanel.ui'
**
** Created: Mo 24. Nov 14:38:06 2008
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "QtVectorGridAttrPanel.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "CQtAttrPanelBase.h"
#include "VectorGrid.h"
#include "VectorGridState.h"
#include "qvalidator.h"
#include "QtVectorGridAttrPanel.ui.h"
/*
 *  Constructs a CQtVectorGridAttrPanel which is a child of 'parent', with the
 *  name 'name'.' 
 */
CQtVectorGridAttrPanel::CQtVectorGridAttrPanel( QWidget* parent,  const char* name )
    : CQtAttrPanelBase( parent, name )
{
    if ( !name )
	setName( "CQtVectorGridAttrPanel" );
    setMinimumSize( QSize( 310, 0 ) );

    VectorGridTabWidget = new QTabWidget( this, "VectorGridTabWidget" );
    VectorGridTabWidget->setGeometry( QRect( 0, 0, 300, 390 ) );
    VectorGridTabWidget->setMinimumSize( QSize( 300, 0 ) );
    VectorGridTabWidget->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    QPalette pal;
    QColorGroup cg;
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 236, 233, 216) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 245, 244, 235) );
    cg.setColor( QColorGroup::Dark, QColor( 118, 116, 108) );
    cg.setColor( QColorGroup::Mid, QColor( 157, 155, 143) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 235, 235, 235) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, black );
    cg.setColor( QColorGroup::LinkVisited, black );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 236, 233, 216) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 255, 254, 249) );
    cg.setColor( QColorGroup::Dark, QColor( 118, 116, 108) );
    cg.setColor( QColorGroup::Mid, QColor( 157, 155, 143) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 235, 235, 235) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, QColor( 0, 0, 255) );
    cg.setColor( QColorGroup::LinkVisited, QColor( 255, 0, 255) );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 236, 233, 216) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 255, 254, 249) );
    cg.setColor( QColorGroup::Dark, QColor( 118, 116, 108) );
    cg.setColor( QColorGroup::Mid, QColor( 157, 155, 143) );
    cg.setColor( QColorGroup::Text, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 235, 235, 235) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, QColor( 0, 0, 255) );
    cg.setColor( QColorGroup::LinkVisited, QColor( 255, 0, 255) );
    pal.setDisabled( cg );
    VectorGridTabWidget->setPalette( pal );
    VectorGridTabWidget->setAutoMask( FALSE );
    VectorGridTabWidget->setTabShape( QTabWidget::Rounded );

    tab = new QWidget( VectorGridTabWidget, "tab" );

    QWidget* privateLayoutWidget = new QWidget( tab, "layout15" );
    privateLayoutWidget->setGeometry( QRect( 20, 11, 256, 194 ) );
    layout15 = new QVBoxLayout( privateLayoutWidget, 11, 6, "layout15"); 

    layout69 = new QHBoxLayout( 0, 0, 6, "layout69"); 

    textLabel1 = new QLabel( privateLayoutWidget, "textLabel1" );
    layout69->addWidget( textLabel1 );
    spacer64 = new QSpacerItem( 150, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout69->addItem( spacer64 );
    layout15->addLayout( layout69 );

    layout67 = new QGridLayout( 0, 1, 1, 0, 6, "layout67"); 

    MinYLabel = new QLabel( privateLayoutWidget, "MinYLabel" );
    MinYLabel->setMinimumSize( QSize( 80, 0 ) );

    layout67->addWidget( MinYLabel, 0, 1 );

    MinXLabel = new QLabel( privateLayoutWidget, "MinXLabel" );
    MinXLabel->setMinimumSize( QSize( 80, 0 ) );

    layout67->addWidget( MinXLabel, 0, 0 );

    MaxZLabel = new QLabel( privateLayoutWidget, "MaxZLabel" );
    MaxZLabel->setMinimumSize( QSize( 80, 0 ) );

    layout67->addWidget( MaxZLabel, 1, 2 );

    MinZLabel = new QLabel( privateLayoutWidget, "MinZLabel" );
    MinZLabel->setMinimumSize( QSize( 80, 0 ) );

    layout67->addWidget( MinZLabel, 0, 2 );

    MaxYLabel = new QLabel( privateLayoutWidget, "MaxYLabel" );
    MaxYLabel->setMinimumSize( QSize( 80, 0 ) );

    layout67->addWidget( MaxYLabel, 1, 1 );

    MaxXLabel = new QLabel( privateLayoutWidget, "MaxXLabel" );
    MaxXLabel->setMinimumSize( QSize( 80, 0 ) );

    layout67->addWidget( MaxXLabel, 1, 0 );
    layout15->addLayout( layout67 );
    spacer65 = new QSpacerItem( 20, 60, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout15->addItem( spacer65 );

    layout14 = new QGridLayout( 0, 1, 1, 0, 6, "layout14"); 

    MaxVecLengthLabel = new QLabel( privateLayoutWidget, "MaxVecLengthLabel" );
    MaxVecLengthLabel->setMinimumSize( QSize( 120, 0 ) );

    layout14->addWidget( MaxVecLengthLabel, 0, 1 );

    MaxVectorLengthLabel = new QLabel( privateLayoutWidget, "MaxVectorLengthLabel" );
    MaxVectorLengthLabel->setMinimumSize( QSize( 120, 0 ) );

    layout14->addWidget( MaxVectorLengthLabel, 0, 0 );

    ActiveScalarNameLabel = new QLabel( privateLayoutWidget, "ActiveScalarNameLabel" );
    ActiveScalarNameLabel->setMinimumSize( QSize( 120, 0 ) );

    layout14->addWidget( ActiveScalarNameLabel, 2, 1 );

    ActiveVectorNameLabel = new QLabel( privateLayoutWidget, "ActiveVectorNameLabel" );
    ActiveVectorNameLabel->setMinimumSize( QSize( 120, 0 ) );

    layout14->addWidget( ActiveVectorNameLabel, 1, 1 );

    ActiveScalarNameTextLabel = new QLabel( privateLayoutWidget, "ActiveScalarNameTextLabel" );
    ActiveScalarNameTextLabel->setMinimumSize( QSize( 120, 0 ) );

    layout14->addWidget( ActiveScalarNameTextLabel, 2, 0 );

    ActiveVectorNameTextLabel = new QLabel( privateLayoutWidget, "ActiveVectorNameTextLabel" );
    ActiveVectorNameTextLabel->setMinimumSize( QSize( 120, 0 ) );

    layout14->addWidget( ActiveVectorNameTextLabel, 1, 0 );
    layout15->addLayout( layout14 );
    VectorGridTabWidget->insertTab( tab, QString::fromLatin1("") );

    tab_2 = new QWidget( VectorGridTabWidget, "tab_2" );

    QWidget* privateLayoutWidget_2 = new QWidget( tab_2, "layout29" );
    privateLayoutWidget_2->setGeometry( QRect( 51, 24, 200, 173 ) );
    layout29 = new QVBoxLayout( privateLayoutWidget_2, 11, 6, "layout29"); 

    layout63 = new QHBoxLayout( 0, 0, 6, "layout63"); 

    ShowGlyphsCheckBox = new QCheckBox( privateLayoutWidget_2, "ShowGlyphsCheckBox" );
    layout63->addWidget( ShowGlyphsCheckBox );
    spacer60 = new QSpacerItem( 90, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout63->addItem( spacer60 );
    layout29->addLayout( layout63 );
    spacer61 = new QSpacerItem( 20, 28, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout29->addItem( spacer61 );

    layout28 = new QGridLayout( 0, 1, 1, 0, 6, "layout28"); 

    MaskRatioLineEdit = new QLineEdit( privateLayoutWidget_2, "MaskRatioLineEdit" );
    MaskRatioLineEdit->setMinimumSize( QSize( 110, 0 ) );

    layout28->addWidget( MaskRatioLineEdit, 0, 1 );

    textLabel14 = new QLabel( privateLayoutWidget_2, "textLabel14" );
    textLabel14->setMinimumSize( QSize( 80, 0 ) );

    layout28->addWidget( textLabel14, 1, 0 );

    textLabel13 = new QLabel( privateLayoutWidget_2, "textLabel13" );
    textLabel13->setMinimumSize( QSize( 80, 0 ) );

    layout28->addWidget( textLabel13, 0, 0 );

    GlyphScaleFactorLineEdit = new QLineEdit( privateLayoutWidget_2, "GlyphScaleFactorLineEdit" );
    GlyphScaleFactorLineEdit->setMinimumSize( QSize( 110, 0 ) );

    layout28->addWidget( GlyphScaleFactorLineEdit, 1, 1 );
    layout29->addLayout( layout28 );
    spacer63 = new QSpacerItem( 20, 30, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout29->addItem( spacer63 );

    ComputeGlyphsButton = new QPushButton( privateLayoutWidget_2, "ComputeGlyphsButton" );
    layout29->addWidget( ComputeGlyphsButton );
    VectorGridTabWidget->insertTab( tab_2, QString::fromLatin1("") );

    TabPage = new QWidget( VectorGridTabWidget, "TabPage" );

    QWidget* privateLayoutWidget_3 = new QWidget( TabPage, "layout65" );
    privateLayoutWidget_3->setGeometry( QRect( 33, 13, 240, 338 ) );
    layout65 = new QVBoxLayout( privateLayoutWidget_3, 11, 6, "layout65"); 

    layout59 = new QHBoxLayout( 0, 0, 6, "layout59"); 

    ShowStreamlinesCheckBox = new QCheckBox( privateLayoutWidget_3, "ShowStreamlinesCheckBox" );
    layout59->addWidget( ShowStreamlinesCheckBox );
    spacer53 = new QSpacerItem( 110, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout59->addItem( spacer53 );
    layout65->addLayout( layout59 );
    spacer57 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout65->addItem( spacer57 );

    layout54 = new QGridLayout( 0, 1, 1, 0, 6, "layout54"); 

    StreamlineLengthLineEdit = new QLineEdit( privateLayoutWidget_3, "StreamlineLengthLineEdit" );
    StreamlineLengthLineEdit->setMinimumSize( QSize( 110, 0 ) );

    layout54->addWidget( StreamlineLengthLineEdit, 1, 1 );

    textLabel16 = new QLabel( privateLayoutWidget_3, "textLabel16" );
    textLabel16->setMinimumSize( QSize( 80, 0 ) );

    layout54->addWidget( textLabel16, 1, 0 );

    TubeRadiusLineEdit = new QLineEdit( privateLayoutWidget_3, "TubeRadiusLineEdit" );
    TubeRadiusLineEdit->setMinimumSize( QSize( 110, 0 ) );

    layout54->addWidget( TubeRadiusLineEdit, 2, 1 );

    StreamlinesDirectionComboBox = new QComboBox( FALSE, privateLayoutWidget_3, "StreamlinesDirectionComboBox" );
    StreamlinesDirectionComboBox->setMinimumSize( QSize( 110, 0 ) );

    layout54->addWidget( StreamlinesDirectionComboBox, 3, 1 );

    textLabel17 = new QLabel( privateLayoutWidget_3, "textLabel17" );
    textLabel17->setMinimumSize( QSize( 80, 0 ) );

    layout54->addWidget( textLabel17, 2, 0 );

    StepLengthLineEdit = new QLineEdit( privateLayoutWidget_3, "StepLengthLineEdit" );
    StepLengthLineEdit->setMinimumSize( QSize( 110, 0 ) );

    layout54->addWidget( StepLengthLineEdit, 0, 1 );

    textLabel15 = new QLabel( privateLayoutWidget_3, "textLabel15" );
    textLabel15->setMinimumSize( QSize( 80, 0 ) );

    layout54->addWidget( textLabel15, 0, 0 );

    textLabel18 = new QLabel( privateLayoutWidget_3, "textLabel18" );
    textLabel18->setMinimumSize( QSize( 80, 0 ) );

    layout54->addWidget( textLabel18, 3, 0 );
    layout65->addLayout( layout54 );
    spacer58 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout65->addItem( spacer58 );

    layout57 = new QHBoxLayout( 0, 0, 6, "layout57"); 

    UseGridAsSourceCheckBox = new QCheckBox( privateLayoutWidget_3, "UseGridAsSourceCheckBox" );
    UseGridAsSourceCheckBox->setMinimumSize( QSize( 140, 0 ) );
    layout57->addWidget( UseGridAsSourceCheckBox );
    spacer54 = new QSpacerItem( 80, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout57->addItem( spacer54 );
    layout65->addLayout( layout57 );

    layout56 = new QHBoxLayout( 0, 0, 6, "layout56"); 

    NumStreamlineSourcesXDirSpinBox = new QSpinBox( privateLayoutWidget_3, "NumStreamlineSourcesXDirSpinBox" );
    NumStreamlineSourcesXDirSpinBox->setMinimumSize( QSize( 60, 0 ) );
    layout56->addWidget( NumStreamlineSourcesXDirSpinBox );
    spacer55 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout56->addItem( spacer55 );

    NumStreamlineSourcesYDirSpinBox = new QSpinBox( privateLayoutWidget_3, "NumStreamlineSourcesYDirSpinBox" );
    NumStreamlineSourcesYDirSpinBox->setMinimumSize( QSize( 60, 0 ) );
    layout56->addWidget( NumStreamlineSourcesYDirSpinBox );
    spacer56 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout56->addItem( spacer56 );

    NumStreamlineSourcesZDirSpinBox = new QSpinBox( privateLayoutWidget_3, "NumStreamlineSourcesZDirSpinBox" );
    NumStreamlineSourcesZDirSpinBox->setMinimumSize( QSize( 60, 0 ) );
    layout56->addWidget( NumStreamlineSourcesZDirSpinBox );
    layout65->addLayout( layout56 );
    spacer59 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout65->addItem( spacer59 );

    layout58 = new QHBoxLayout( 0, 0, 6, "layout58"); 

    UseGeometryAsSourceCheckBox = new QCheckBox( privateLayoutWidget_3, "UseGeometryAsSourceCheckBox" );
    UseGeometryAsSourceCheckBox->setMinimumSize( QSize( 140, 0 ) );
    layout58->addWidget( UseGeometryAsSourceCheckBox );

    StreamlinesGeomObjectNameLineEdit = new QLineEdit( privateLayoutWidget_3, "StreamlinesGeomObjectNameLineEdit" );
    layout58->addWidget( StreamlinesGeomObjectNameLineEdit );
    layout65->addLayout( layout58 );
    spacer62 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout65->addItem( spacer62 );

    ComputeStreamlinesButton = new QPushButton( privateLayoutWidget_3, "ComputeStreamlinesButton" );
    layout65->addWidget( ComputeStreamlinesButton );
    VectorGridTabWidget->insertTab( TabPage, QString::fromLatin1("") );
    languageChange();
    resize( QSize(310, 400).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( ShowGlyphsCheckBox, SIGNAL( stateChanged(int) ), this, SLOT( OnShowGlyphsCheckBoxStateChanged() ) );
    connect( ShowStreamlinesCheckBox, SIGNAL( stateChanged(int) ), this, SLOT( OnShowStreamlinesCheckBoxStateChanged() ) );
    connect( ComputeGlyphsButton, SIGNAL( clicked() ), this, SLOT( OnComputeGlyphsButtonClicked() ) );
    connect( ComputeStreamlinesButton, SIGNAL( clicked() ), this, SLOT( OnComputeStreamlinesButtonClicked() ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
CQtVectorGridAttrPanel::~CQtVectorGridAttrPanel()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CQtVectorGridAttrPanel::languageChange()
{
    setCaption( tr( "QtVectorGridAttrPanel" ) );
    textLabel1->setText( tr( "Extends:" ) );
    MinYLabel->setText( tr( "textLabel3" ) );
    MinXLabel->setText( tr( "textLabel2" ) );
    MaxZLabel->setText( tr( "textLabel7" ) );
    MinZLabel->setText( tr( "textLabel4" ) );
    MaxYLabel->setText( tr( "textLabel6" ) );
    MaxXLabel->setText( tr( "textLabel5" ) );
    MaxVecLengthLabel->setText( tr( "textLabel11" ) );
    MaxVectorLengthLabel->setText( tr( "Maximal vector length" ) );
    ActiveScalarNameLabel->setText( tr( "textLabel4" ) );
    ActiveVectorNameLabel->setText( tr( "textLabel2" ) );
    ActiveScalarNameTextLabel->setText( tr( "Active scalar" ) );
    ActiveVectorNameTextLabel->setText( tr( "Active vector" ) );
    VectorGridTabWidget->changeTab( tab, tr( "Info" ) );
    ShowGlyphsCheckBox->setText( tr( "Show" ) );
    textLabel14->setText( tr( "Scale factor" ) );
    textLabel13->setText( tr( "Mask ratio" ) );
    ComputeGlyphsButton->setText( tr( "Compute" ) );
    VectorGridTabWidget->changeTab( tab_2, tr( "Glyphs" ) );
    ShowStreamlinesCheckBox->setText( tr( "Show" ) );
    textLabel16->setText( tr( "Length" ) );
    textLabel17->setText( tr( "Tube radius" ) );
    textLabel15->setText( tr( "Step length" ) );
    textLabel18->setText( tr( "Direction" ) );
    UseGridAsSourceCheckBox->setText( tr( "Use grid as source" ) );
    UseGeometryAsSourceCheckBox->setText( tr( "Use geometry as source" ) );
    ComputeStreamlinesButton->setText( tr( "Compute" ) );
    VectorGridTabWidget->changeTab( TabPage, tr( "Streamlines" ) );
}

