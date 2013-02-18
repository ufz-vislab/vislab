/****************************************************************************
** Form interface generated from reading ui file '.\ProjectControllerLoaderDialog.ui'
**
** Created: So 22. Jun 14:42:10 2008
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CPROJECTCONTROLLERLOADERDIALOG_H
#define CPROJECTCONTROLLERLOADERDIALOG_H

#include <qvariant.h>
#include <qdialog.h>
#include "ProjectControllerLoader.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLineEdit;
class QPushButton;

class CProjectControllerLoaderDialog : public QDialog
{
    Q_OBJECT

public:
    CProjectControllerLoaderDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~CProjectControllerLoaderDialog();

    QLineEdit* WorkingDirectoryLineEdit;
    QPushButton* ArealImageFilenameButton;
    QLineEdit* WPETypesFilenameLineEdit;
    QPushButton* WPETypesFilenameButton;
    QLineEdit* ArealImageFilenameLineEdit;
    QPushButton* WPEModelFilenameButton;
    QLineEdit* TerrainFilenameLineEdit;
    QPushButton* TerrainFilenameButton;
    QPushButton* SetCurrentDirectoryButton;
    QLineEdit* WPE3DModelFilenameLineEdit;
    QPushButton* LoadAllButton;
    QPushButton* CloseButton;

public slots:
    virtual void OnCloseButtonPressed();
    virtual void OnLoadAllButtonPressed();
    virtual void OnCurrentDirectoryButtonPressed();
    virtual void OnTerrainFilenameButtonPressed();
    virtual void OnArealImageFilenameButtonPressed();
    virtual void OnWPETypeFilenameButtonPressed();
    virtual void OnWPE3DModelButtonPressed();

protected:
    CProjectControllerLoader m_ProjectControllerLoader;
    QString m_qstrWorkingDirectory;

    QVBoxLayout* layout7;
    QSpacerItem* spacer2;
    QGridLayout* layout1;
    QHBoxLayout* layout2;
    QSpacerItem* spacer1;

protected slots:
    virtual void languageChange();

private:
    void init();

};

#endif // CPROJECTCONTROLLERLOADERDIALOG_H
