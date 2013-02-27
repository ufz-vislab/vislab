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
void CQtScalarGridAttrPanel::init(){
    m_pSubject = NULL;
}

void CQtScalarGridAttrPanel::Update(){
    if (m_pSubject == NULL) return;
    
    CScalarUnstructuredGrid *uGrid = (CScalarUnstructuredGrid*) m_pSubject;
    CScalarGridState state = uGrid->GetState();
}


bool CQtScalarGridAttrPanel::SetSubject( CSubject *sub ){
    CScalarUnstructuredGrid *uGrid = (CScalarUnstructuredGrid*) sub;
    if (uGrid != NULL){
	m_pSubject = sub;
	return true;
    } else {
	return false;
    }
}


void CQtScalarGridAttrPanel::SetInitialValues(){
    if (m_pSubject == NULL) return;
    
    CScalarUnstructuredGrid *uGrid = (CScalarUnstructuredGrid*) m_pSubject;
    CScalarGridState state = uGrid->GetState();
    
    ContourAttrNameComboBox->clear();
    ColorAttrNameComboBox->clear();
    
     QString qtext;
    
    std::list<std::string> attributes = uGrid->GetAvailableScalars();
    std::list<std::string>::iterator iter;
    
    ColorAttrNameComboBox->insertItem(QString("NONE"));
    for (iter = attributes.begin(); iter != attributes.end(); iter++){
	qtext = QString((*iter).c_str());
	ContourAttrNameComboBox->insertItem(qtext);
	ColorAttrNameComboBox->insertItem(qtext);
    }
    
    ContourAttrNameComboBox->setCurrentText(QString(state.m_stdstrAttributeName.c_str()));
    //ColorAttrNameComboBox->setCurrentText(QString(state.m_stdstrColorAttributeName.c_str()));
    ColorAttrNameComboBox->setCurrentText(QString("NONE"));
    
    qtext = QString("%1").arg(state.m_dValMin, 0, 'f', 3);
    DataValMinLabel->setText(qtext);
    
    qtext = QString("%1").arg(state.m_dValMax, 0, 'f', 3);
    DataValMaxLabel->setText(qtext);
    
    qtext = QString("%1").arg(state.m_dXMin, 0, 'f', 3);
    XMinLabel->setText(qtext);
    
    qtext = QString("%1").arg(state.m_dXMax, 0, 'f', 3);
    XMaxLabel->setText(qtext);
    
    qtext = QString("%1").arg(state.m_dYMin, 0, 'f', 3);
    YMinLabel->setText(qtext);
    
    qtext = QString("%1").arg(state.m_dYMax, 0, 'f', 3);
    YMaxLabel->setText(qtext);
    
    qtext = QString("%1").arg(state.m_dZMin, 0, 'f', 3);
    ZMinLabel->setText(qtext);
    
    qtext = QString("%1").arg(state.m_dZMax, 0, 'f', 3);
    ZMaxLabel->setText(qtext);
    
    if (state.m_bShowContours) ShowContoursCheckBox->setChecked(true);
    if (state.m_bShowXSection) ShowXSectionCheckBox->setChecked(true);
    if (state.m_bShowYSection) ShowYSectionCheckBox->setChecked(true);
    if (state.m_bShowZSection) ShowZSectionCheckBox->setChecked(true);
}


void CQtScalarGridAttrPanel::OnShowContoursCheckBoxToggled(){
     if (m_pSubject == NULL) return;
    
    CScalarUnstructuredGrid *uGrid = (CScalarUnstructuredGrid*) m_pSubject;
    CScalarGridState state = uGrid->GetState();
    state.m_bShowContours = ShowContoursCheckBox->isChecked();
    uGrid->SetState(state);
    uGrid->Notify(this);
}


void CQtScalarGridAttrPanel::OnShowXSectionCheckBoxToggled(){
     if (m_pSubject == NULL) return;
    
    CScalarUnstructuredGrid *uGrid = (CScalarUnstructuredGrid*) m_pSubject;
    CScalarGridState state = uGrid->GetState();
    state.m_bShowXSection = ShowXSectionCheckBox->isChecked();
    uGrid->SetState(state);
    uGrid->Notify(this);
}


void CQtScalarGridAttrPanel::OnShowYSectionCheckBoxToggled(){
     if (m_pSubject == NULL) return;
    
    CScalarUnstructuredGrid *uGrid = (CScalarUnstructuredGrid*) m_pSubject;
    CScalarGridState state = uGrid->GetState();
    state.m_bShowYSection = ShowYSectionCheckBox->isChecked();
    uGrid->SetState(state);
    uGrid->Notify(this);
}


void CQtScalarGridAttrPanel::OnShowZSectionCheckBoxToggled(){
     if (m_pSubject == NULL) return;
    
    CScalarUnstructuredGrid *uGrid = (CScalarUnstructuredGrid*) m_pSubject;
    CScalarGridState state = uGrid->GetState();
    state.m_bShowZSection = ShowZSectionCheckBox->isChecked();
    uGrid->SetState(state);
    uGrid->Notify(this);
}


void CQtScalarGridAttrPanel::OnNumContoursSpinboxValueChanged()
{

}


void CQtScalarGridAttrPanel::OnMinContourSliderValueChanged()
{

}


void CQtScalarGridAttrPanel::OnMaxContourSliderValueChanged()
{

}


void CQtScalarGridAttrPanel::OnXSectionSliderValueChanged(){
    if (m_pSubject == NULL) return;
    CScalarUnstructuredGrid *uGrid = (CScalarUnstructuredGrid*) m_pSubject;
    CScalarGridState state = uGrid->GetState();
    
    double dVal = (double) XSectionSlider->value();
    double dFraction = dVal / 500.0;
    state.m_dXSectionPosition = state.m_dXMin + dFraction * (state.m_dXMax - state.m_dXMin);
    
    QString qtext = QString("%1").arg(state.m_dXSectionPosition, 0, 'f', 3);
    XSectionPositionLineEdit->setText(qtext);
    
    uGrid->SetState(state);
    uGrid->Notify(this);
}


void CQtScalarGridAttrPanel::OnYSectionSliderValueChanged(){
     if (m_pSubject == NULL) return;
    CScalarUnstructuredGrid *uGrid = (CScalarUnstructuredGrid*) m_pSubject;
    CScalarGridState state = uGrid->GetState();
    
    double dVal = (double) YSectionSlider->value();
    double dFraction = dVal / 500.0;
    state.m_dYSectionPosition = state.m_dYMin + dFraction * (state.m_dYMax - state.m_dYMin);
    
    QString qtext = QString("%1").arg(state.m_dYSectionPosition, 0, 'f', 3);
    YSectionPositionLineEdit->setText(qtext);
    
    uGrid->SetState(state);
    uGrid->Notify(this);
}


void CQtScalarGridAttrPanel::OnZSectionSliderValueChanged(){
     if (m_pSubject == NULL) return;
    CScalarUnstructuredGrid *uGrid = (CScalarUnstructuredGrid*) m_pSubject;
    CScalarGridState state = uGrid->GetState();
    
    double dVal = (double) ZSectionSlider->value();
    double dFraction = dVal / 500.0;
    state.m_dZSectionPosition = state.m_dZMin + dFraction * (state.m_dZMax - state.m_dZMin);
    
    QString qtext = QString("%1").arg(state.m_dZSectionPosition, 0, 'f', 3);
    ZSectionPositionLineEdit->setText(qtext);
    
    uGrid->SetState(state);
    uGrid->Notify(this);
}


void CQtScalarGridAttrPanel::OnContourDataValueChanged()
{
     if (m_pSubject == NULL) return;
    CScalarUnstructuredGrid *uGrid = (CScalarUnstructuredGrid*) m_pSubject;
    CScalarGridState state = uGrid->GetState();
    
    int iNumContours = NumContoursSpinBox->value();
    
    double dVal1 = (double) MinContourSlider->value();
    double dVal2 = (double) MaxContourSlider->value();
    double dMinVal = (dVal1 < dVal2) ? dVal1 : dVal2;
    double dMaxVal = (dVal1 >= dVal2) ? dVal1 : dVal2;
    
    double dMinValFrac = dMinVal / 500.0;
    double dMaxValFrac = dMaxVal / 500.0;
    
    state.m_dMinContour = state.m_dValMin + dMinValFrac * (state.m_dValMax - state.m_dValMin);
    state.m_dMaxContour = state.m_dValMin + dMaxValFrac * (state.m_dValMax - state.m_dValMin);
    state.m_iNumContours = iNumContours;
    
    QString qtext;
    qtext = QString("%1").arg(state.m_dMinContour, 0, 'f', 3);
    MinContourLineEdit->setText(qtext);
    qtext = QString("%1").arg(state.m_dMaxContour, 0, 'f', 3);
    MaxContourLineEdit->setText(qtext);
    
    uGrid->SetState(state);
    uGrid->Notify(this);
}


void CQtScalarGridAttrPanel::OnColorAttrValueComboBoxTextChanged(){
     if (m_pSubject == NULL) return;
    CScalarUnstructuredGrid *uGrid = (CScalarUnstructuredGrid*) m_pSubject;
    CScalarGridState state = uGrid->GetState();
    
    QString newColorAttributeName = ColorAttrNameComboBox->currentText();
    state.m_stdstrColorAttributeName = std::string(newColorAttributeName.ascii());
    
    std::cout << "state set to new attribute for color" << state.m_stdstrColorAttributeName << std::endl;
    
    uGrid->SetState(state);
    uGrid->Notify(this);
}


void CQtScalarGridAttrPanel::OnContourAttrNameComboBoxActivated()
{
     if (m_pSubject == NULL) return;
     
    CScalarUnstructuredGrid *uGrid = (CScalarUnstructuredGrid*) m_pSubject;
    
    QString newContourAttributeName = ContourAttrNameComboBox->currentText();
    uGrid->SetNewContourAttribute(std::string(newContourAttributeName.ascii()));
    CScalarGridState state = uGrid->GetState();
    SetInitialValues();
    
    std::cout << "state set to new attribute for contours" << state.m_stdstrColorAttributeName << std::endl;
    
    uGrid->SetState(state);
    uGrid->Notify(this);
}
