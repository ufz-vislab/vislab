/****************************************************************************
** Form implementation generated from reading ui file '.\WPEMainFrame.ui'
**
** Created: Mo 23. Jun 18:07:10 2008
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "WPEMainFrame.h"

#include <qvariant.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qaction.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qtoolbar.h>
#include <qimage.h>
#include <qpixmap.h>

#include "qsplitter.h"
#include "WPEAttrPanel.h"
#include "WPEDiagramView.h"
#include "WPEProjectController.h"
#include "qfiledialog.h"
#include "WPEMainFrame.ui.h"
/*
 *  Constructs a CWPEMainFrame as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
CWPEMainFrame::CWPEMainFrame( QWidget* parent, const char* name, WFlags fl )
    : QMainWindow( parent, name, fl )
{
    (void)statusBar();
    if ( !name )
	setName( "CWPEMainFrame" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)3, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setMinimumSize( QSize( 21, 138 ) );

    // actions
    fileNewAction = new QAction( this, "fileNewAction" );
    fileNewAction->setIconSet( QIconSet( QPixmap::fromMimeSource( "" ) ) );
    fileOpenAction = new QAction( this, "fileOpenAction" );
    fileOpenAction->setIconSet( QIconSet( QPixmap::fromMimeSource( "" ) ) );
    fileSaveAction = new QAction( this, "fileSaveAction" );
    fileSaveAction->setIconSet( QIconSet( QPixmap::fromMimeSource( "" ) ) );
    fileSaveAsAction = new QAction( this, "fileSaveAsAction" );
    filePrintAction = new QAction( this, "filePrintAction" );
    filePrintAction->setIconSet( QIconSet( QPixmap::fromMimeSource( "" ) ) );
    fileExitAction = new QAction( this, "fileExitAction" );
    ResetAllAction = new QAction( this, "ResetAllAction" );
    LoadAllSettings = new QAction( this, "LoadAllSettings" );
    SaveWindparkAction = new QAction( this, "SaveWindparkAction" );


    // toolbars


    // menubar
    MenuBar = new QMenuBar( this, "MenuBar" );


    fileMenu = new QPopupMenu( this );
    fileNewAction->addTo( fileMenu );
    fileOpenAction->addTo( fileMenu );
    fileSaveAction->addTo( fileMenu );
    fileSaveAsAction->addTo( fileMenu );
    fileMenu->insertSeparator();
    filePrintAction->addTo( fileMenu );
    fileMenu->insertSeparator();
    fileExitAction->addTo( fileMenu );
    MenuBar->insertItem( QString(""), fileMenu, 1 );

    WPE = new QPopupMenu( this );
    ResetAllAction->addTo( WPE );
    SaveWindparkAction->addTo( WPE );
    LoadAllSettings->addTo( WPE );
    MenuBar->insertItem( QString(""), WPE, 2 );

    languageChange();
    resize( QSize(800, 600).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( ResetAllAction, SIGNAL( activated() ), this, SLOT( OnWPEResetAllAction() ) );
    connect( LoadAllSettings, SIGNAL( activated() ), this, SLOT( OnLoadAllSettings() ) );
    connect( SaveWindparkAction, SIGNAL( activated() ), this, SLOT( OnSaveWindparkAction() ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
CWPEMainFrame::~CWPEMainFrame()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CWPEMainFrame::languageChange()
{
    setCaption( tr( "Mainframe" ) );
    fileNewAction->setText( tr( "New" ) );
    fileNewAction->setMenuText( tr( "&New" ) );
    fileNewAction->setAccel( tr( "Ctrl+N" ) );
    fileOpenAction->setText( tr( "Open" ) );
    fileOpenAction->setMenuText( tr( "&Open..." ) );
    fileOpenAction->setAccel( tr( "Ctrl+O" ) );
    fileSaveAction->setText( tr( "Save" ) );
    fileSaveAction->setMenuText( tr( "&Save" ) );
    fileSaveAction->setAccel( tr( "Ctrl+S" ) );
    fileSaveAsAction->setText( tr( "Save As" ) );
    fileSaveAsAction->setMenuText( tr( "Save &As..." ) );
    fileSaveAsAction->setAccel( QString::null );
    filePrintAction->setText( tr( "Print" ) );
    filePrintAction->setMenuText( tr( "&Print..." ) );
    filePrintAction->setAccel( tr( "Ctrl+P" ) );
    fileExitAction->setText( tr( "Exit" ) );
    fileExitAction->setMenuText( tr( "E&xit" ) );
    fileExitAction->setAccel( QString::null );
    ResetAllAction->setText( tr( "Reset all" ) );
    LoadAllSettings->setText( tr( "Load settings" ) );
    SaveWindparkAction->setText( tr( "Save windpark" ) );
    if (MenuBar->findItem(1))
        MenuBar->findItem(1)->setText( tr( "&File" ) );
    if (MenuBar->findItem(2))
        MenuBar->findItem(2)->setText( tr( "WPE" ) );
}

