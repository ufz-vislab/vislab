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

void CNavigationMainFrame::init(){
    m_pqSplitter = new QSplitter(this);
    this->setCentralWidget(m_pqSplitter);
    
    m_pDiagramView = new CNavigationDiagramView(m_pqSplitter);
    m_pAttrPanel = new CNavigationAttrPanel(m_pqSplitter);
    
    CNavigationProjectController *controller = CNavigationProjectController::Instance();
    controller->SetAttrPanel(m_pAttrPanel);
    controller->SetDiagramView(m_pDiagramView);
}
