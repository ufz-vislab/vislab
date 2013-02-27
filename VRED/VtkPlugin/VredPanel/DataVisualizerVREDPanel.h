/****************************************************************************
** Form interface generated from reading ui file '.\DataVisualizerVREDPanel.ui'
**
** Created: Mo 24. Nov 17:05:24 2008
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CDATAVISUALIZERVREDPANEL_H
#define CDATAVISUALIZERVREDPANEL_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QTabWidget;
class QPushButton;
class QLabel;
class QCheckBox;
class QFrame;
class QSplitter;
class CQtAttrPanelSelector;
class CQtAttrPanelHolder;
class CDataVisApp;

class CDataVisualizerVREDPanel : public QWidget
{
    Q_OBJECT

public:
    CDataVisualizerVREDPanel( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~CDataVisualizerVREDPanel();

    QTabWidget* tabWidget2;
    QWidget* tab;
    QPushButton* LoadVtlVectorDataButton;
    QLabel* textLabel2;
    QCheckBox* ShowSciVizDataCheckBox;
    QPushButton* LoadScalarStructuredGridButton;
    QPushButton* LoadScalarUnstructuredGridButton;
    QLabel* textLabel1;
    QFrame* VTKDataPanel;
    QWidget* tab_2;

public slots:
    virtual void OnLoadScalarStructuredGridButton();
    virtual void OnLoadScalarUnstructuredGridButton();
    virtual void OnLoadVtkVectorDataButton();
    virtual void OnLoadVectorUnstructuredGridButton();
    virtual void OnShowSciVizDataCheckBoxClicked();

protected:
    CQtAttrPanelHolder *m_pAttrPanelHolder;
    CQtAttrPanelSelector *m_pAttrPanelSelector;
    QSplitter *m_pqVTKDataPanelSplitter;
    CDataVisApp *m_pDataVisApp;

    QVBoxLayout* layout18;
    QGridLayout* layout3;
    QSpacerItem* spacer2;
    QSpacerItem* spacer1;

protected slots:
    virtual void languageChange();

private:
    void init();

};

#endif // CDATAVISUALIZERVREDPANEL_H
