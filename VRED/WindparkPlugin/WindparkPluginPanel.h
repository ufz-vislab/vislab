/****************************************************************************
** Form interface generated from reading ui file '.\WindparkPluginPanel.ui'
**
** Created: Mi 4. Jun 09:38:49 2008
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CWINDPARKPLUGINPANEL_H
#define CWINDPARKPLUGINPANEL_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;

class CWindparkPluginPanel : public QWidget
{
    Q_OBJECT

public:
    CWindparkPluginPanel( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~CWindparkPluginPanel();

    QPushButton* pushButton1;
    QPushButton* pushButton2;
    QPushButton* pushButton3;

protected:
    QVBoxLayout* layout1;

protected slots:
    virtual void languageChange();

};

#endif // CWINDPARKPLUGINPANEL_H
