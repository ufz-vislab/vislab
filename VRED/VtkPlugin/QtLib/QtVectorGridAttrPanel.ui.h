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

void CQtVectorGridAttrPanel::init(){
    m_pSubject = NULL;
    
    VectorGridTabWidget->setPalette(this->palette());
    QWidget *infoTab = VectorGridTabWidget->page(0);
    infoTab->setPalette(this->palette());
    
    QWidget *glyphTab = VectorGridTabWidget->page(1);
    glyphTab->setPalette(this->palette());
    
    QWidget *streamlinesTab = VectorGridTabWidget->page(2);
    streamlinesTab->setPalette(this->palette());
    
    MinXLabel->setPalette(this->palette());
    MinYLabel->setPalette(this->palette());
    MinZLabel->setPalette(this->palette());
    MaxXLabel->setPalette(this->palette());
    MaxYLabel->setPalette(this->palette());
    MaxZLabel->setPalette(this->palette());
    MaxVectorLengthLabel->setPalette(this->palette());
    MaxVecLengthLabel->setPalette(this->palette());
    ActiveVectorNameTextLabel->setPalette(this->palette());
    ActiveVectorNameLabel->setPalette(this->palette());
    ActiveScalarNameTextLabel->setPalette(this->palette());
    ActiveScalarNameLabel->setPalette(this->palette());
}


void CQtVectorGridAttrPanel::Update(){
    if (m_pSubject == NULL) return;
    CVectorGrid *grid = (CVectorGrid*) m_pSubject;
    CVectorGridState state = grid->GetState();
}


bool CQtVectorGridAttrPanel::SetSubject( CSubject *sub ){
    CVectorGrid *grid = (CVectorGrid*) sub;
    if (sub != NULL){
	m_pSubject = sub;
	return true;
    } else {
	return false;
    }
}


void CQtVectorGridAttrPanel::SetInitialValues()
{
    if (m_pSubject == NULL) return;
    CVectorGrid *grid = (CVectorGrid*) m_pSubject;
    CVectorGridState state = grid->GetState();
    
    QIntValidator *intValidator = new QIntValidator(this);
    QDoubleValidator *doubleValidator = new QDoubleValidator(this);
    
    QString qtext;
    
    // On the info page
    qtext = QString("%1").arg(state.m_dXMin, 0, 'f', 3);
    MinXLabel->setText(qtext);
    
    qtext = QString("%1").arg(state.m_dYMin, 0, 'f', 3);
    MinYLabel->setText(qtext);
    
    qtext = QString("%1").arg(state.m_dZMin, 0, 'f', 3);
    MinZLabel->setText(qtext);
    
    qtext = QString("%1").arg(state.m_dXMax, 0, 'f', 3);
    MaxXLabel->setText(qtext);
    
    qtext = QString("%1").arg(state.m_dYMax, 0, 'f', 3);
    MaxYLabel->setText(qtext);
    
    qtext = QString("%1").arg(state.m_dZMax, 0, 'f', 3);
    MaxZLabel->setText(qtext);
    
    qtext = QString("%1").arg(state.m_dMaxVecLength, 0, 'f', 6);
    MaxVecLengthLabel->setText(qtext);
    
    // On the glyphs page
    if (state.m_bShowGlyphs){
	ShowGlyphsCheckBox->setChecked(true);
    } else {
	ShowGlyphsCheckBox->setChecked(false);
    }
    
    MaskRatioLineEdit->setValidator(intValidator);
    qtext = QString("%1").arg(state.m_iGlyphMaskRatio);
    MaskRatioLineEdit->setText(qtext);
    
    GlyphScaleFactorLineEdit->setValidator(doubleValidator);
    qtext = QString("%1").arg(state.m_dGlyphScaleFactor, 0,  'f', 6);
    GlyphScaleFactorLineEdit->setText(qtext);
    
    // On the streamlines page
    if (state.m_bShowStreamlines){
	ShowStreamlinesCheckBox->setChecked(true);
    } else {
	ShowStreamlinesCheckBox->setChecked(false);
    }
    
    StepLengthLineEdit->setValidator(doubleValidator);
    qtext = QString("%1").arg(state.m_dStepLength, 0, 'f', 6);
    StepLengthLineEdit->setText(qtext);
    
    StreamlineLengthLineEdit->setValidator(doubleValidator);
    qtext = QString("%1").arg(state.m_dMaxPropagationLength, 0, 'f', 6);
    StreamlineLengthLineEdit->setText(qtext);
    
    TubeRadiusLineEdit->setValidator(doubleValidator);
    qtext = QString("%1").arg(state.m_dTubeRadius, 0, 'f', 6);
    TubeRadiusLineEdit->setText(qtext);
    
    StreamlinesDirectionComboBox->insertItem(QString("Forward"));
    StreamlinesDirectionComboBox->insertItem(QString("Backward"));
    StreamlinesDirectionComboBox->insertItem(QString("Both"));
    
    if (state.m_bUseGridAsSourceForStreamlines){
	UseGridAsSourceCheckBox->setChecked(true);
    } else {
	UseGridAsSourceCheckBox->setChecked(false);
    }
    
    NumStreamlineSourcesXDirSpinBox->setValue(state.m_iNumStreamlineSourcesXDir);
    NumStreamlineSourcesXDirSpinBox->setMinValue (2);
    NumStreamlineSourcesYDirSpinBox->setValue(state.m_iNumStreamlineSourcesYDir);
    NumStreamlineSourcesYDirSpinBox->setMinValue(2);
    NumStreamlineSourcesZDirSpinBox->setValue(state.m_iNumStreamlineSourcesZDir);
    NumStreamlineSourcesZDirSpinBox->setMinValue(2);
    
    if (state.m_bUseGeomObjectAsSourceForStreamlines){
	UseGeometryAsSourceCheckBox->setChecked(true);
    } else {
	UseGeometryAsSourceCheckBox->setChecked(false);
    }
    
    StreamlinesGeomObjectNameLineEdit->setText((QString(state.m_stdstrStreamlinesGeomObjectName.c_str())));
    
    ActiveVectorNameLabel->setText(QString(state.m_stdstrActiveVectorName.c_str()));
    ActiveScalarNameLabel->setText(QString(state.m_stdstrActiveScalarName.c_str()));
}

void CQtVectorGridAttrPanel::OnShowGlyphsCheckBoxStateChanged(){
    if (m_pSubject == NULL) return;
    CVectorGrid *grid = (CVectorGrid*) m_pSubject;
    CVectorGridState state = grid->GetState();
    
    if (ShowGlyphsCheckBox->isChecked()){
	state.m_bShowGlyphs = true;
    } else {
	state.m_bShowGlyphs = false;
    }
    
    grid->SetState(state);
    grid->Notify(this);
}

void CQtVectorGridAttrPanel::OnShowStreamlinesCheckBoxStateChanged(){
    if (m_pSubject == NULL) return;
    CVectorGrid *grid = (CVectorGrid*) m_pSubject;
    CVectorGridState state = grid->GetState();
    
    if (ShowStreamlinesCheckBox->isChecked()){
	state.m_bShowStreamlines = true;
    } else {
	state.m_bShowStreamlines = false;
    }
    
    grid->SetState(state);
    grid->Notify(this);
}


void CQtVectorGridAttrPanel::OnComputeGlyphsButtonClicked(){
    if (m_pSubject == NULL) return;
    CVectorGrid *grid = (CVectorGrid*) m_pSubject;
    CVectorGridState state = grid->GetState();
    
    QString qtext;
    
    qtext = MaskRatioLineEdit->text();
    state.m_iGlyphMaskRatio = qtext.toInt();
    
    qtext = GlyphScaleFactorLineEdit->text();
    state.m_dGlyphScaleFactor = qtext.toDouble();
    
    grid->SetState(state);
    grid->Notify(this);
}


void CQtVectorGridAttrPanel::OnComputeStreamlinesButtonClicked(){
    if (m_pSubject == NULL) return;
    CVectorGrid *grid = (CVectorGrid*) m_pSubject;
    CVectorGridState state = grid->GetState();
    
    QString qtext;
    
    qtext = StepLengthLineEdit->text();
    state.m_dStepLength = qtext.toDouble();
    
    qtext = StreamlineLengthLineEdit->text();
    state.m_dMaxPropagationLength = qtext.toDouble();
    
    qtext = TubeRadiusLineEdit->text();
    state.m_dTubeRadius = qtext.toDouble();
    
    qtext = StreamlinesDirectionComboBox->currentText();
    if (qtext == QString("Forward")){
	state.m_iIntegrationDirection = CVectorGridState::FORWARD;
    } else if (qtext == QString("Backward")){
	state.m_iIntegrationDirection = CVectorGridState::BACKWARD;
    } else if (qtext == QString("Both")){
	state.m_iIntegrationDirection = CVectorGridState::BOTH;
    } else {
	state.m_iIntegrationDirection = CVectorGridState::BOTH;
    }
    
    if (UseGridAsSourceCheckBox->isChecked()){
	state.m_bUseGridAsSourceForStreamlines = true;
    } else {
	state.m_bUseGridAsSourceForStreamlines = false;
    }
    
    state.m_iNumStreamlineSourcesXDir = NumStreamlineSourcesXDirSpinBox->value();
    state.m_iNumStreamlineSourcesYDir = NumStreamlineSourcesYDirSpinBox->value();
    state.m_iNumStreamlineSourcesZDir = NumStreamlineSourcesZDirSpinBox->value();
    
    if (UseGeometryAsSourceCheckBox->isChecked()){
	state.m_bUseGeomObjectAsSourceForStreamlines = true;
    } else {
	state.m_bUseGeomObjectAsSourceForStreamlines = false;
    }
    
    qtext = StreamlinesGeomObjectNameLineEdit->text();
    state.m_stdstrStreamlinesGeomObjectName = std::string(qtext.ascii());
    
    grid->SetState(state);
    grid->Notify(this);
}
