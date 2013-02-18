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


void CWPEAttrPanel::OnAddItemButtonPressed()
{
    CWPEProjectController::Instance()->InitAddWPE();
}


void CWPEAttrPanel::OnRemoveActiveItemButton()
{
    CWPEProjectController::Instance()->RemoveActiveWPE();
}


void CWPEAttrPanel::OnLoadArealImageButton()
{
    QString fileFilters = tr("png dateien (*.png)\n"
			   "jpg dateien (*.jpg)\n");
    
    QString fileName = QFileDialog::getOpenFileName(".", fileFilters, this);
    CWPEProjectController::Instance()->LoadArealImage(fileName.ascii());
}


void CWPEAttrPanel::OnScaleDownButtonPressed()
{
    CWPEProjectController::Instance()->ScaleDown();
}


void CWPEAttrPanel::OnScaleUpButtonPressed()
{
    CWPEProjectController::Instance()->ScaleUp();
}


void CWPEAttrPanel::AddTypeNameToTypeNamesCombo( const char * typeName )
{
    QString qTypeName(typeName);
    WPETypeNamesCombo->insertItem(qTypeName);
}


const char* CWPEAttrPanel::GetCurrentTypeNameFromTypeNamesCombo()
{
    return WPETypeNamesCombo->currentText().ascii();
}


void CWPEAttrPanel::OnTypeNamesComboTextChanged()
{
    const char *typeName = WPETypeNamesCombo->currentText().ascii();
    CWPEProjectController::Instance()->ChangeTypeOfActiveWPE(typeName);
    CWPETypeInfo typeInfo = CWPEProjectController::Instance()->GetTypeInfoByName(typeName);
    SetWPETypeAttributes(typeInfo);
}


void CWPEAttrPanel::OnLoadWPETypesButtonPressed()
{
     QString fileFilters = tr("txt dateien (*.txt)\n");
    
    QString fileName = QFileDialog::getOpenFileName(".", fileFilters, this);
    CWPEProjectController::Instance()->LoadTypes(fileName.ascii());
}


void CWPEAttrPanel::OnLoadTerrainButtonPressed()
{
    QString fileFilters = tr("OpenSG dateien (*.osb)\n");
    
    QString fileName = QFileDialog::getOpenFileName(".", fileFilters, this);
    if (! fileName.isEmpty()){
	CWPEProjectController::Instance()->LoadTerrainModel(fileName.ascii());
    }
}


void CWPEAttrPanel::OnSet3DModelFileNameButtonPressed()
{
     QString fileFilters = tr("OpenSG dateien (*.osb)\n");
    
    QString fileName = QFileDialog::getOpenFileName(".", fileFilters, this);
    if (! fileName.isEmpty()){
	CWPEProjectController::Instance()->Set3DModelFilename(fileName.ascii());
    }
}


void CWPEAttrPanel::SetCurrentWPEType( CWPETypeInfo wpeType )
{
    WPETypeNamesCombo->setCurrentText(QString(wpeType.m_stdstrWPETypeName.c_str()));
    SetWPETypeAttributes(wpeType);
}


void CWPEAttrPanel::SetWPETypeAttributes( CWPETypeInfo typeInfo )
{
     QString genHeightText = QString("%1").arg(typeInfo.m_fPilarHeight, 0, 'f', 3);
    GeneratorHeightLineEdit->setText(genHeightText);
    
    QString rotLengthText = QString("%1").arg(typeInfo.m_fRotorLength, 0, 'f', 3);
    RotorLengthLineEdit->setText(rotLengthText);
    
    QString mwhText = QString("%1").arg(typeInfo.m_fMWh, 0, 'f', 3);
    WPEMWHLineEdit->setText(mwhText);
}


void CWPEAttrPanel::SetEnergyProduction( float production )
{
    QString allMWhText = QString("%1").arg(production, 0, 'f', 3);
    WindparkProductionLineEdit->setText(allMWhText);
}


void CWPEAttrPanel::OnSaveWindparkButtonPressed()
{
    QString fileFilters = tr("Text dateien (*.txt)\n");
    
    QString fileName = QFileDialog::getSaveFileName(".", fileFilters, this);
    if (! fileName.isEmpty()){
	CWPEProjectController::Instance()->SaveWindparkInfo(fileName.ascii());
    }
}


void CWPEAttrPanel::OnWinddirectionDialValueChanged()
{
    float value = WinddirectionDial->value();
    CWPEProjectController::Instance()->SetNewWindDirection(value);
}


float CWPEAttrPanel::GetWinddirection()
{
    return (float) WinddirectionDial->value();
}


void CWPEAttrPanel::OnUseWPERotorsCheckBoxToggled()
{
    bool useRotors = UseWPERotorsCheckBox->isChecked();
    CWPEProjectController::Instance()->UseRotorRotation(useRotors);
}


void CWPEAttrPanel::OnLoadLandscapeButtonPressed()
{
    QString fileFilters = tr("OpenSG dateien (*.osb)\n");
    
    QString fileName = QFileDialog::getOpenFileName(".", fileFilters, this);
    if (! fileName.isEmpty()){
	CWPEProjectController::Instance()->LoadLandscape(fileName.ascii());
    }
}
