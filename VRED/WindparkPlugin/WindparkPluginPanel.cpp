/****************************************************************************
** Form implementation generated from reading ui file '.\WindparkPluginPanel.ui'
**
** Created: Mi 4. Jun 09:38:50 2008
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "WindparkPluginPanel.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

/*
 *  Constructs a CWindparkPluginPanel as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
CWindparkPluginPanel::CWindparkPluginPanel( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "CWindparkPluginPanel" );

    QWidget* privateLayoutWidget = new QWidget( this, "layout1" );
    privateLayoutWidget->setGeometry( QRect( 40, 20, 82, 83 ) );
    layout1 = new QVBoxLayout( privateLayoutWidget, 11, 6, "layout1"); 

    pushButton1 = new QPushButton( privateLayoutWidget, "pushButton1" );
    layout1->addWidget( pushButton1 );

    pushButton2 = new QPushButton( privateLayoutWidget, "pushButton2" );
    layout1->addWidget( pushButton2 );

    pushButton3 = new QPushButton( privateLayoutWidget, "pushButton3" );
    layout1->addWidget( pushButton3 );
    languageChange();
    resize( QSize(158, 261).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
CWindparkPluginPanel::~CWindparkPluginPanel()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CWindparkPluginPanel::languageChange()
{
    setCaption( tr( "WindparkPluginPanel" ) );
    pushButton1->setText( tr( "pushButton1" ) );
    pushButton2->setText( tr( "pushButton2" ) );
    pushButton3->setText( tr( "pushButton3" ) );
}

