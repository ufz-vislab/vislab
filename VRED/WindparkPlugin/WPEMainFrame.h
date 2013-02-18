/****************************************************************************
** Form interface generated from reading ui file '.\WPEMainFrame.ui'
**
** Created: Mo 23. Jun 18:07:09 2008
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CWPEMAINFRAME_H
#define CWPEMAINFRAME_H

#include <qvariant.h>
#include <qmainwindow.h>
#include "ProjectControllerLoaderDialog.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QAction;
class QActionGroup;
class QToolBar;
class QPopupMenu;
class QSplitter;
class CWPEAttrPanel;
class CWPEDiagramView;
class CWPEProjectController;

class CWPEMainFrame : public QMainWindow
{
    Q_OBJECT

public:
    CWPEMainFrame( QWidget* parent = 0, const char* name = 0, WFlags fl = WType_TopLevel );
    ~CWPEMainFrame();

    QMenuBar *MenuBar;
    QPopupMenu *fileMenu;
    QPopupMenu *WPE;
    QAction* fileNewAction;
    QAction* fileOpenAction;
    QAction* fileSaveAction;
    QAction* fileSaveAsAction;
    QAction* filePrintAction;
    QAction* fileExitAction;
    QAction* ResetAllAction;
    QAction* LoadAllSettings;
    QAction* SaveWindparkAction;

public slots:
    virtual void OnWPEResetAllAction();
    virtual void OnLoadAllSettings();
    virtual void OnSaveWindparkAction();

protected:
    CWPEAttrPanel *m_pAttrPanel;
    CWPEDiagramView *m_pDiagramView;
    QSplitter *m_pqSplitter;
    CProjectControllerLoaderDialog *m_pProjectControllerLoaderDialog;


protected slots:
    virtual void languageChange();

private:
    void init();

};

#endif // CWPEMAINFRAME_H
