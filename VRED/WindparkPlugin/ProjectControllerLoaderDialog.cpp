/****************************************************************************
** Form implementation generated from reading ui file '.\ProjectControllerLoaderDialog.ui'
**
** Created: So 22. Jun 14:42:10 2008
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "ProjectControllerLoaderDialog.h"

#include <qvariant.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "qfiledialog.h"
#include "qmessagebox.h"
#include "ProjectControllerLoaderDialog.ui.h"
/*
 *  Constructs a CProjectControllerLoaderDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
CProjectControllerLoaderDialog::CProjectControllerLoaderDialog( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "CProjectControllerLoaderDialog" );
    setMinimumSize( QSize( 650, 300 ) );

    QWidget* privateLayoutWidget = new QWidget( this, "layout7" );
    privateLayoutWidget->setGeometry( QRect( 40, 41, 660, 232 ) );
    layout7 = new QVBoxLayout( privateLayoutWidget, 11, 6, "layout7"); 

    layout1 = new QGridLayout( 0, 1, 1, 0, 6, "layout1"); 

    WorkingDirectoryLineEdit = new QLineEdit( privateLayoutWidget, "WorkingDirectoryLineEdit" );
    WorkingDirectoryLineEdit->setMinimumSize( QSize( 530, 0 ) );

    layout1->addWidget( WorkingDirectoryLineEdit, 0, 1 );

    ArealImageFilenameButton = new QPushButton( privateLayoutWidget, "ArealImageFilenameButton" );
    ArealImageFilenameButton->setMinimumSize( QSize( 120, 0 ) );

    layout1->addWidget( ArealImageFilenameButton, 2, 0 );

    WPETypesFilenameLineEdit = new QLineEdit( privateLayoutWidget, "WPETypesFilenameLineEdit" );
    WPETypesFilenameLineEdit->setMinimumSize( QSize( 530, 0 ) );

    layout1->addWidget( WPETypesFilenameLineEdit, 3, 1 );

    WPETypesFilenameButton = new QPushButton( privateLayoutWidget, "WPETypesFilenameButton" );
    WPETypesFilenameButton->setMinimumSize( QSize( 120, 0 ) );

    layout1->addWidget( WPETypesFilenameButton, 3, 0 );

    ArealImageFilenameLineEdit = new QLineEdit( privateLayoutWidget, "ArealImageFilenameLineEdit" );
    ArealImageFilenameLineEdit->setMinimumSize( QSize( 530, 0 ) );

    layout1->addWidget( ArealImageFilenameLineEdit, 2, 1 );

    WPEModelFilenameButton = new QPushButton( privateLayoutWidget, "WPEModelFilenameButton" );
    WPEModelFilenameButton->setMinimumSize( QSize( 120, 0 ) );

    layout1->addWidget( WPEModelFilenameButton, 4, 0 );

    TerrainFilenameLineEdit = new QLineEdit( privateLayoutWidget, "TerrainFilenameLineEdit" );
    TerrainFilenameLineEdit->setMinimumSize( QSize( 530, 0 ) );

    layout1->addWidget( TerrainFilenameLineEdit, 1, 1 );

    TerrainFilenameButton = new QPushButton( privateLayoutWidget, "TerrainFilenameButton" );
    TerrainFilenameButton->setMinimumSize( QSize( 120, 0 ) );

    layout1->addWidget( TerrainFilenameButton, 1, 0 );

    SetCurrentDirectoryButton = new QPushButton( privateLayoutWidget, "SetCurrentDirectoryButton" );
    SetCurrentDirectoryButton->setMinimumSize( QSize( 120, 0 ) );

    layout1->addWidget( SetCurrentDirectoryButton, 0, 0 );

    WPE3DModelFilenameLineEdit = new QLineEdit( privateLayoutWidget, "WPE3DModelFilenameLineEdit" );
    WPE3DModelFilenameLineEdit->setMinimumSize( QSize( 530, 0 ) );

    layout1->addWidget( WPE3DModelFilenameLineEdit, 4, 1 );
    layout7->addLayout( layout1 );
    spacer2 = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout7->addItem( spacer2 );

    layout2 = new QHBoxLayout( 0, 0, 6, "layout2"); 

    LoadAllButton = new QPushButton( privateLayoutWidget, "LoadAllButton" );
    LoadAllButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, LoadAllButton->sizePolicy().hasHeightForWidth() ) );
    LoadAllButton->setMinimumSize( QSize( 100, 25 ) );
    layout2->addWidget( LoadAllButton );
    spacer1 = new QSpacerItem( 310, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout2->addItem( spacer1 );

    CloseButton = new QPushButton( privateLayoutWidget, "CloseButton" );
    CloseButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, CloseButton->sizePolicy().hasHeightForWidth() ) );
    CloseButton->setMinimumSize( QSize( 100, 25 ) );
    layout2->addWidget( CloseButton );
    layout7->addLayout( layout2 );
    languageChange();
    resize( QSize(799, 393).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( CloseButton, SIGNAL( pressed() ), this, SLOT( OnCloseButtonPressed() ) );
    connect( LoadAllButton, SIGNAL( pressed() ), this, SLOT( OnLoadAllButtonPressed() ) );
    connect( SetCurrentDirectoryButton, SIGNAL( pressed() ), this, SLOT( OnCurrentDirectoryButtonPressed() ) );
    connect( TerrainFilenameButton, SIGNAL( pressed() ), this, SLOT( OnTerrainFilenameButtonPressed() ) );
    connect( ArealImageFilenameButton, SIGNAL( pressed() ), this, SLOT( OnArealImageFilenameButtonPressed() ) );
    connect( WPETypesFilenameButton, SIGNAL( pressed() ), this, SLOT( OnWPETypeFilenameButtonPressed() ) );
    connect( WPEModelFilenameButton, SIGNAL( pressed() ), this, SLOT( OnWPE3DModelButtonPressed() ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
CProjectControllerLoaderDialog::~CProjectControllerLoaderDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CProjectControllerLoaderDialog::languageChange()
{
    setCaption( tr( "Load settings" ) );
    ArealImageFilenameButton->setText( tr( "Areal image" ) );
    WPETypesFilenameButton->setText( tr( "WPE types" ) );
    WPEModelFilenameButton->setText( tr( "WPE 3D model" ) );
    TerrainFilenameButton->setText( tr( "Terrain file" ) );
    SetCurrentDirectoryButton->setText( tr( "Current directory" ) );
    LoadAllButton->setText( tr( "Load all" ) );
    CloseButton->setText( tr( "close" ) );
}

