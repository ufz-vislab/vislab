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

void CQtVREDToolsPanel::init(){
    m_pSceneManager = new CVREDToolsSceneManager();
    CToolHolder::Instance()->SetSceneManager(m_pSceneManager);
    
    VRPNServerLineEdit->setText(QString("DTrack@141.65.34.28"));
    
    this->setMinimumSize(300, 600);
    
    m_pLeftWindow = new CQtVRED2DViewMainWindow(this, "Left window");
    m_pRightWindow = new CQtVRED2DViewMainWindow(this, "Right window");
    m_pNavWindow = new CQtVRED2DNavMainWindow(this, "Navigation information");
    
    LoadDefaultTools();
}


void CQtVREDToolsPanel::Loop(){
    if (UseTrackingCheckBox->isChecked()){
	CARTToolsManager::Instance()->VRPNLoop();
    }
}

void CQtVREDToolsPanel::LoadDefaultTools(){
    CToolHolder *toolHolder = CToolHolder::Instance();
    
    CFlystickWalkNavigationTranslateTool *walk_translate_tool;
    walk_translate_tool = new CFlystickWalkNavigationTranslateTool();
    walk_translate_tool->SetMaxPickDistance(0.2);
    walk_translate_tool->Load3DCursor("C:\\libs32\\Geometry\\translate_cursor.osb");
	
    CFlystickWalkNavigationTranslationResetTool *walk_translation_reset_tool;
    walk_translation_reset_tool = new CFlystickWalkNavigationTranslationResetTool();
    walk_translation_reset_tool->SetMaxPickDistance(1.0);
    walk_translation_reset_tool->Load3DCursor("C:\\libs32\\Geometry\\cone_cursor.osb");

    CFlystickWalkCageFilterTool *cage_filter_bbox_tool;
    cage_filter_bbox_tool = new CFlystickWalkCageFilterTool();
    cage_filter_bbox_tool->SetMaxPickDistance(0.5);
    cage_filter_bbox_tool->Load3DCursor("C:\\libs32\\Geometry\\cage_filter_bbox_cursor.osb");
	
    CFlystickWalkCageFilterTool	*cage_filter_clip_tool;
    cage_filter_clip_tool = new CFlystickWalkCageFilterTool();
    cage_filter_clip_tool->SetMaxPickDistance(0.5);
    cage_filter_clip_tool->Load3DCursor("C:\\libs32\\Geometry\\cage_filter_clip_cursor.osb");
    cage_filter_clip_tool->SetInteractionType(CFlystickCageFilterBBoxTool::SET_CLIP_PLANES);
    
    CMouseTrackballNavigationTool *mouse_default_tool;
    mouse_default_tool = new CMouseTrackballNavigationTool();
    
    CToolHolder::Instance()->AddTool("default", mouse_default_tool);
    CToolHolder::Instance()->AddTool("Tool1", walk_translate_tool);
    CToolHolder::Instance()->AddTool("Tool2", walk_translation_reset_tool);
    CToolHolder::Instance()->AddTool("Tool3", cage_filter_bbox_tool);
    CToolHolder::Instance()->AddTool("Tool4", cage_filter_clip_tool);
    
    CToolHolder::Instance()->SetIntersectActionTravMask(2);
    CToolHolder::Instance()->Set3DCursorTravMask(1);

    CARTToolsManager::Instance()->SetDefaultTool("default");
    CARTToolsManager::Instance()->SetArtButton1ToolName("Tool1");
    CARTToolsManager::Instance()->SetArtButton2ToolName("Tool2");
    CARTToolsManager::Instance()->SetArtButton3ToolName("Tool3");
    CARTToolsManager::Instance()->SetArtButton4ToolName("Tool4");
    
    //CART2DWindowManager::Instance()->SetLeftScreenInterface(new CART2DWindowInterface());
    //CART2DWindowManager::Instance()->SetRightScreenInterface(new CART2DWindowInterface());
}


void CQtVREDToolsPanel::OnUseTrackingCheckBoxToggled(){
    if (UseTrackingCheckBox->isChecked()){
	vrOSGWidget *gl = vrOSGWidget::getMGLW(-1);
	if (gl != NULL){  
	    std::cout << "Switching tracking on" << std::endl;
	    OSG::NodePtr node = gl->getUserNode();
	    OSG::TransformPtr userTransform = TransformPtr::dcast(node->getCore());
	    if (userTransform != OSG::NullFC){
		CARTToolsManager::Instance()->SetProjectionCameraUserTransformPtr(userTransform);
		CARTToolsManager::Instance()->StartVRPNTracking(VRPNServerLineEdit->text().ascii());
	    }
	}
    } else {
	CARTToolsManager::Instance()->StopVRPNTracking();
    }
}

void CQtVREDToolsPanel::OnPrintVREDInfoButton(){
    m_pSceneManager->PrintInformation();
    
    Pnt3f p(0.0, 0.0, -0.5);
    Vec3f v(1.0, 0.0, 0.0);
    int foundId;
    CScreenIntersector s;
    s.ConfigForUFZDisplay();
    s.TestIntersectScreens(p, v, foundId);
}

void CQtVREDToolsPanel::OnArtDefaultToolComboBoxClicked(){
    std::string newToolName(ArtDefaultToolComboBox->currentText().ascii());
    CARTToolsManager::Instance()->SetDefaultTool(newToolName);
    CARTToolsManager::Instance()->UseTool(0);
}

void CQtVREDToolsPanel::OnArtTool1ComboBoxChanged(){
    std::string newToolName(ArtTool1ComboBox->currentText().ascii());
    CARTToolsManager::Instance()->SetArtButton1ToolName(newToolName);
}

void CQtVREDToolsPanel::OnArtTool2ComboBoxChanged(){
    std::string newToolName(ArtTool2ComboBox->currentText().ascii());
    CARTToolsManager::Instance()->SetArtButton2ToolName(newToolName);
}

void CQtVREDToolsPanel::OnArtTool3ComboBoxChanged(){
    std::string newToolName(ArtTool3ComboBox->currentText().ascii());
    CARTToolsManager::Instance()->SetArtButton3ToolName(newToolName);
}

void CQtVREDToolsPanel::OnArtTool4ComboBoxChanged(){
    std::string newToolName(ArtTool4ComboBox->currentText().ascii());
    CARTToolsManager::Instance()->SetArtButton4ToolName(newToolName);
}


void CQtVREDToolsPanel::OnSaveToolsDefaultConfigurationButtonClicked(){
    QString fileFilters = tr("xml dateien (*.xml)\n");
    
    QString fileName = QFileDialog::getSaveFileName(".", fileFilters, this);
    if (! fileName.isEmpty()){
	CXmlToolLoader::GenerateDefaultFile(fileName.ascii());
    }
}


void CQtVREDToolsPanel::OnLoadToolsConfigButtonClicked(){
    QString fileFilters = tr("xml dateien (*.xml)\n");
    QString fileName = QFileDialog::getOpenFileName(".", fileFilters, this);
    if (! fileName.isEmpty()){
	CXmlToolLoader::LoadFile(fileName.ascii());
    }
    OnUpdateComboBoxes();
}


void CQtVREDToolsPanel::OnUpdateComboBoxes(){
    ArtDefaultToolComboBox->clear();
    ArtTool1ComboBox->clear();
    ArtTool2ComboBox->clear();
    ArtTool3ComboBox->clear();
    ArtTool4ComboBox->clear();
    
    std::vector<std::string> toolNames = CToolHolder::Instance()->GetAllToolNames();
    std::vector<std::string>::iterator iter;
    for (iter = toolNames.begin(); iter != toolNames.end(); iter++){
	ArtDefaultToolComboBox->insertItem(QString((*iter).c_str()));
	ArtTool1ComboBox->insertItem(QString((*iter).c_str()));
	ArtTool2ComboBox->insertItem(QString((*iter).c_str()));
	ArtTool3ComboBox->insertItem(QString((*iter).c_str()));
	ArtTool4ComboBox->insertItem(QString((*iter).c_str()));
    }
    
    ArtDefaultToolComboBox->setCurrentText(QString(CARTToolsManager::Instance()->GetDefaultToolName().c_str()));
    ArtTool1ComboBox->setCurrentText(QString(CARTToolsManager::Instance()->GetArtButton1ToolName().c_str()));
    ArtTool2ComboBox->setCurrentText(QString(CARTToolsManager::Instance()->GetArtButton2ToolName().c_str()));
    ArtTool3ComboBox->setCurrentText(QString(CARTToolsManager::Instance()->GetArtButton3ToolName().c_str()));
    ArtTool4ComboBox->setCurrentText(QString(CARTToolsManager::Instance()->GetArtButton4ToolName().c_str()));
}


void CQtVREDToolsPanel::OnLoad2DViewConfigButtonClicked(){
    QString fileFilters = tr("xml dateien (*.xml)\n");
    QString fileName = QFileDialog::getOpenFileName(".", fileFilters, this);
    if (! fileName.isEmpty()){
	CART2DWindowManager::Instance()->LoadNodeToVisMap(fileName.ascii());
    }
}


void CQtVREDToolsPanel::OnSave2DViewExampleConfigButtonClicked(){
    QString fileFilters = tr("xml dateien (*.xml)\n");
    
    QString fileName = QFileDialog::getSaveFileName(".", fileFilters, this);
    if (! fileName.isEmpty()){
	CART2DWindowManager::Instance()->WriteNodeToVisMapFileExample(fileName.ascii());
    }
}

void CQtVREDToolsPanel::OnCageFilterNodeNameLineEditTextChanged(){
    if (UseCageFilterCheckBox->isChecked()){
	std::string nodeName = std::string(CageFilterNodeNameLineEdit->text().ascii());
	CCageFilter *cage = CCageFilter::Instance();
	cage->SetCageFilterAroundNode(m_pSceneManager->GetRoot(), nodeName);
	cage->SetMarkerScaling(0.1);
	m_pSceneManager->UpdateScenegraph();	
    }
}

void CQtVREDToolsPanel::OnOpenLeftWindowButtonClicked(){
    CART2DWindowManager::Instance()->SetLeftScreenInterface(m_pLeftWindow);
    m_pLeftWindow->show();
    m_pLeftWindow->Show2DViewFor("E:\\zehner_work\\Daten\\oxywall_data\\Example_for_2D_BoreholeDiagramm.jpg");
}


void CQtVREDToolsPanel::OnOpenRightWindowButtonClicked(){
    CART2DWindowManager::Instance()->SetRightScreenInterface(m_pRightWindow);
    m_pRightWindow->show();
    m_pRightWindow->Show2DViewFor("E:\\zehner_work\\Daten\\oxywall_data\\Example_for_2D_Histogram.jpg");
}


void CQtVREDToolsPanel::OnCloseLeftWindowButtonClicked(){
    CART2DWindowManager::Instance()->SetLeftScreenInterface(NULL);
    m_pLeftWindow->hide();
}


void CQtVREDToolsPanel::OnCloseRightWindowButtonClicked(){
    CART2DWindowManager::Instance()->SetRightScreenInterface(NULL);
    m_pRightWindow->hide();
}


void CQtVREDToolsPanel::OnOpenNavWindowButtonClicked(){
    m_pSceneManager->SetNavigation2DObserver(m_pNavWindow);
    m_pNavWindow->show();
}


void CQtVREDToolsPanel::OnCloseNavWindowButtonClicked(){
    m_pSceneManager->SetNavigation2DObserver(NULL);
    m_pNavWindow->hide();
}


void CQtVREDToolsPanel::OnLoad2DNavWindowConfigButtonClicked(){
    QString fileFilters = tr("xml dateien (*.xml)\n");
    QString fileName = QFileDialog::getOpenFileName(".", fileFilters, this);
    if (! fileName.isEmpty()){
	m_pNavWindow->LoadSettingsFromXMLFile(fileName.ascii());
    }
}


void CQtVREDToolsPanel::OnSave2DNavWindowExampleConfigButtonClicked(){
    QString fileFilters = tr("xml dateien (*.xml)\n");
    
    QString fileName = QFileDialog::getSaveFileName(".", fileFilters, this);
    if (! fileName.isEmpty()){
	m_pNavWindow->WriteExampleSettingsXMLFile(fileName.ascii());
    }
}
