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

void CDataVisualizerVREDPanel::init(){
    m_pqVTKDataPanelSplitter = new QSplitter(VTKDataPanel);
    m_pqVTKDataPanelSplitter->setOrientation(QSplitter::Vertical);
    m_pqVTKDataPanelSplitter->setMinimumSize(320, 700);
    
    m_pAttrPanelSelector = new CQtAttrPanelSelector(m_pqVTKDataPanelSplitter); 
    m_pAttrPanelSelector->setMinimumSize(320, 100);
    
    m_pAttrPanelHolder = new CQtAttrPanelHolder(m_pqVTKDataPanelSplitter);
    m_pAttrPanelHolder->setMinimumSize(320, 450);
    
    m_pAttrPanelSelector->AddRootNode(std::string("root 1"));
    m_pAttrPanelSelector->AddRootNode(std::string("root 2"));
    
    m_pDataVisApp = CDataVisApp::Instance();
    m_pDataVisApp->SetPanelSelector(m_pAttrPanelSelector);
    m_pDataVisApp->SetPanelHolder(m_pAttrPanelHolder);
}


void CDataVisualizerVREDPanel::OnLoadScalarStructuredGridButton()
{

}


void CDataVisualizerVREDPanel::OnLoadScalarUnstructuredGridButton(){
    QString fileFilter("VTK files (*.vtk)\n"
		       "VTK files (*.vtu)\n"
		       "VTK files (*.vts)\n");
    QString fileName = QFileDialog::getOpenFileName(".", fileFilter, this);
    if (fileName.isEmpty()) return;
    m_pDataVisApp->LoadScalarUnstructuredGrid(fileName.ascii());
}


void CDataVisualizerVREDPanel::OnLoadVtkVectorDataButton(){
    QString fileFilter("VTK files (*.vtk)\n"
		       "VTK files (*.vtu)\n"
		       "VTK files (*.vts)\n");
    QString fileName = QFileDialog::getOpenFileName(".", fileFilter, this);
    if (fileName.isEmpty()) return;
    m_pDataVisApp->LoadVtkVectorGrid(fileName.ascii());
}


void CDataVisualizerVREDPanel::OnLoadVectorUnstructuredGridButton()
{

}


void CDataVisualizerVREDPanel::OnShowSciVizDataCheckBoxClicked(){
    if (ShowSciVizDataCheckBox->isChecked()){
	osg::NodePtr sceneRoot = vrScenegraph::getRoot();
	osg::NodePtr vtkRoot = m_pDataVisApp->GetOsgRoot();
	beginEditCP(sceneRoot, osg::Node::ChildrenFieldMask);{
	    sceneRoot->addChild(vtkRoot);
	};endEditCP(sceneRoot, osg::Node::ChildrenFieldMask);
	vrScenegraph::update();
    }else{
	osg::NodePtr vtkRoot = m_pDataVisApp->GetOsgRoot();
	vrScenegraph::deleteNode(vtkRoot);
	vrScenegraph::update();
    }
}
