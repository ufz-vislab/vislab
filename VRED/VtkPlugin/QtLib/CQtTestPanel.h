/****************************************************************************
** Form interface generated from reading ui file '.\CQtTestPanel.ui'
**
** Created: Mo 24. Nov 14:38:07 2008
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CQTTESTPANEL_H
#define CQTTESTPANEL_H

#include <qvariant.h>
#include <qpixmap.h>
#include "CQtAttrPanelBase.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QPushButton;
class QLineEdit;

class CQtTestPanel : public CQtAttrPanelBase
{
    Q_OBJECT

public:
    CQtTestPanel( QWidget* parent = 0, const char* name = 0 );
    ~CQtTestPanel();

    QLabel* textLabel1;
    QPushButton* pushButton1;
    QLineEdit* lineEdit1;

    virtual void SetLineEditText( const char * text );
    virtual void Update();
    virtual bool SetSubject( CSubject * );

public slots:
    virtual void SetInitialValues();

protected:

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;

};

#endif // CQTTESTPANEL_H
