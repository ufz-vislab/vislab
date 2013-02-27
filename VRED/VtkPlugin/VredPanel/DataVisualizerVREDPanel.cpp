/****************************************************************************
** Form implementation generated from reading ui file '.\DataVisualizerVREDPanel.ui'
**
** Created: Mo 24. Nov 17:05:24 2008
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "DataVisualizerVREDPanel.h"

#include <qvariant.h>
#include <qtabwidget.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qframe.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "qsplitter.h"
#include "QtAttrPanelSelector.h"
#include "QtAttrPanelHolder.h"
#include "DataVisApp.h"
#include "qfiledialog.h"
#include "vrScenegraph.h"
#include "OpenSG/OSGNode.h"
#include "DataVisualizerVREDPanel.ui.h"
/*
 *  Constructs a CDataVisualizerVREDPanel as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
CDataVisualizerVREDPanel::CDataVisualizerVREDPanel( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "CDataVisualizerVREDPanel" );
    setMinimumSize( QSize( 340, 770 ) );

    tabWidget2 = new QTabWidget( this, "tabWidget2" );
    tabWidget2->setGeometry( QRect( 0, 0, 360, 790 ) );
    tabWidget2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)3, 0, 0, tabWidget2->sizePolicy().hasHeightForWidth() ) );
    tabWidget2->setMinimumSize( QSize( 340, 770 ) );

    tab = new QWidget( tabWidget2, "tab" );

    QWidget* privateLayoutWidget = new QWidget( tab, "layout18" );
    privateLayoutWidget->setGeometry( QRect( 10, 11, 342, 738 ) );
    layout18 = new QVBoxLayout( privateLayoutWidget, 11, 6, "layout18"); 

    layout3 = new QGridLayout( 0, 1, 1, 0, 6, "layout3"); 

    LoadVtlVectorDataButton = new QPushButton( privateLayoutWidget, "LoadVtlVectorDataButton" );
    LoadVtlVectorDataButton->setEnabled( TRUE );

    layout3->addWidget( LoadVtlVectorDataButton, 2, 1 );
    spacer2 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout3->addItem( spacer2, 0, 2 );

    textLabel2 = new QLabel( privateLayoutWidget, "textLabel2" );

    layout3->addWidget( textLabel2, 2, 0 );
    spacer1 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout3->addItem( spacer1, 0, 1 );

    ShowSciVizDataCheckBox = new QCheckBox( privateLayoutWidget, "ShowSciVizDataCheckBox" );

    layout3->addWidget( ShowSciVizDataCheckBox, 0, 0 );

    LoadScalarStructuredGridButton = new QPushButton( privateLayoutWidget, "LoadScalarStructuredGridButton" );
    LoadScalarStructuredGridButton->setEnabled( FALSE );

    layout3->addWidget( LoadScalarStructuredGridButton, 1, 1 );

    LoadScalarUnstructuredGridButton = new QPushButton( privateLayoutWidget, "LoadScalarUnstructuredGridButton" );

    layout3->addWidget( LoadScalarUnstructuredGridButton, 1, 2 );

    textLabel1 = new QLabel( privateLayoutWidget, "textLabel1" );

    layout3->addWidget( textLabel1, 1, 0 );
    layout18->addLayout( layout3 );

    VTKDataPanel = new QFrame( privateLayoutWidget, "VTKDataPanel" );
    VTKDataPanel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)3, 0, 0, VTKDataPanel->sizePolicy().hasHeightForWidth() ) );
    VTKDataPanel->setMinimumSize( QSize( 340, 650 ) );
    VTKDataPanel->setFrameShape( QFrame::StyledPanel );
    VTKDataPanel->setFrameShadow( QFrame::Raised );
    layout18->addWidget( VTKDataPanel );
    tabWidget2->insertTab( tab, QString::fromLatin1("") );

    tab_2 = new QWidget( tabWidget2, "tab_2" );
    tabWidget2->insertTab( tab_2, QString::fromLatin1("") );
    languageChange();
    resize( QSize(466, 834).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( LoadScalarStructuredGridButton, SIGNAL( clicked() ), this, SLOT( OnLoadScalarStructuredGridButton() ) );
    connect( LoadScalarUnstructuredGridButton, SIGNAL( clicked() ), this, SLOT( OnLoadScalarUnstructuredGridButton() ) );
    connect( LoadVtlVectorDataButton, SIGNAL( clicked() ), this, SLOT( OnLoadVtkVectorDataButton() ) );
    connect( ShowSciVizDataCheckBox, SIGNAL( clicked() ), this, SLOT( OnShowSciVizDataCheckBoxClicked() ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
CDataVisualizerVREDPanel::~CDataVisualizerVREDPanel()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CDataVisualizerVREDPanel::languageChange()
{
    setCaption( tr( "Form1" ) );
    LoadVtlVectorDataButton->setText( tr( "Load vtk vector data" ) );
    textLabel2->setText( tr( "Vector data" ) );
    ShowSciVizDataCheckBox->setText( tr( "Show" ) );
    LoadScalarStructuredGridButton->setText( tr( "Structured" ) );
    LoadScalarUnstructuredGridButton->setText( tr( "Unstructured" ) );
    textLabel1->setText( tr( "Scalar data" ) );
    tabWidget2->changeTab( tab, tr( "VTK Data" ) );
    tabWidget2->changeTab( tab_2, tr( "Tools" ) );
}

