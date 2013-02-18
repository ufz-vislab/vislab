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
    
    VRPNServerLineEdit->setText(QString("DTrack@viswork06.intern.ufz.de"));
    
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
}


void CQtVREDToolsPanel::OnLoadToolsConfigButtonClicked(){
    QString fileFilters = tr("xml dateien (*.xml)\n";
    QString fileName = QFileDialog::getOpenFileName(QString("."), fileFilters, this);
    if (! fileName.isEmpty()){
	CXmlToolLoader::LoadFile(fileName.ascii());
    }
}


void CQtVREDToolsPanel::OnSaveToolsConfigButtonClicked(){
    QString fileFilters = tr("xml dateien (*.xml)\n");
    QString fileName = QFileDialog::getSaveFileName(".", fileFilters, this);
    if (! fileName.isEmpty()){
	CXmlToolLoader::GenerateDefaultFile(fileName.ascii());
    }
}


void CQtVREDToolsPanel::OnUpdateComboBoxes(){
}
