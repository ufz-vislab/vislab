/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/

void CWPEMainFrame::init(){
    m_pqSplitter = new QSplitter(this);
    this->setCentralWidget(m_pqSplitter);
    
    m_pDiagramView = new CWPEDiagramView(m_pqSplitter);
    m_pAttrPanel = new CWPEAttrPanel(m_pqSplitter);
    
    CWPEProjectController *controller = CWPEProjectController::Instance();
    controller->SetAttrPanel(m_pAttrPanel);
    controller->SetDiagramView(m_pDiagramView);
    
    m_pProjectControllerLoaderDialog = new CProjectControllerLoaderDialog(this);
}


void CWPEMainFrame::OnWPEResetAllAction()
{
    CWPEProjectController::Instance()->RemoveAllWPEs();
}


void CWPEMainFrame::OnLoadAllSettings()
{
    m_pProjectControllerLoaderDialog->show();
}


void CWPEMainFrame::OnSaveWindparkAction()
{
    QString fileFilters = tr("Text dateien (*.txt)\n");
    
    QString fileName = QFileDialog::getSaveFileName(".", fileFilters, this);
    if (! fileName.isEmpty()){
	CWPEProjectController::Instance()->SaveWindparkInfo(fileName.ascii());
    }
}
