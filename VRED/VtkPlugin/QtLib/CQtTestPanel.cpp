/****************************************************************************
** Form implementation generated from reading ui file '.\CQtTestPanel.ui'
**
** Created: Mo 24. Nov 14:38:07 2008
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "CQtTestPanel.h"

#include <qvariant.h>
#include "CQtAttrPanelBase.h"
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "CQtTestPanel.ui.h"
/*
 *  Constructs a CQtTestPanel which is a child of 'parent', with the
 *  name 'name'.' 
 */
CQtTestPanel::CQtTestPanel( QWidget* parent,  const char* name )
    : CQtAttrPanelBase( parent, name )
{
    if ( !name )
	setName( "CQtTestPanel" );

    textLabel1 = new QLabel( this, "textLabel1" );
    textLabel1->setGeometry( QRect( 90, 20, 80, 20 ) );

    pushButton1 = new QPushButton( this, "pushButton1" );
    pushButton1->setGeometry( QRect( 70, 50, 120, 24 ) );

    lineEdit1 = new QLineEdit( this, "lineEdit1" );
    lineEdit1->setGeometry( QRect( 70, 90, 120, 20 ) );
    languageChange();
    resize( QSize(300, 133).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
CQtTestPanel::~CQtTestPanel()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CQtTestPanel::languageChange()
{
    setCaption( tr( "qt test panel" ) );
    textLabel1->setText( tr( "I am a test panel" ) );
    pushButton1->setText( tr( "pushButton1" ) );
}

