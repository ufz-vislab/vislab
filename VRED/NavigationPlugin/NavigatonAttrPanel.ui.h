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
void CNavigationAttrPanel::init(){
    QDoubleValidator *upperHeightValidator = new QDoubleValidator(UpperHeightLineEdit);
    upperHeightValidator->setDecimals(2);
    UpperHeightLineEdit->setValidator(upperHeightValidator);
    UpperHeightLineEdit->setText(QString("2.0"));
    
    QDoubleValidator  *lowerHeightValidator = new QDoubleValidator(LowerHeightLineEdit);
    lowerHeightValidator->setDecimals(2);
    LowerHeightLineEdit->setValidator(lowerHeightValidator);
    LowerHeightLineEdit->setText(QString("0.0"));
    
    m_dLowerHeight = 0.0;
    m_dUpperHeight = 2.0;
}

void CNavigationAttrPanel::OnLoadArealImageButtonPressed()
{
    QString fileFilters = tr("png dateien (*.png)\n"
			   "jpg dateien (*.jpg)\n");
    
    QString fileName = QFileDialog::getOpenFileName(".", fileFilters, this);
    CNavigationProjectController::Instance()->LoadArealImage(fileName.ascii());
}


void CNavigationAttrPanel::OnScaleUpButtonPressed()
{
    CNavigationProjectController::Instance()->ScaleUp();
}


void CNavigationAttrPanel::OnScaleDownButtonPressed()
{
    CNavigationProjectController::Instance()->ScaleDown();
}


void CNavigationAttrPanel::SetDistanceText( const char *text )
{
    QString distText(text);
    ViewingDistanceEdit->setText(distText);
}


void CNavigationAttrPanel::OnLoadTerrainButtonPressed()
{
    QString fileFilters = tr("OpenSG dateien (*.osb)\n");
    
    QString fileName = QFileDialog::getOpenFileName(".", fileFilters, this);
    if (! fileName.isEmpty()){
	CNavigationProjectController::Instance()->LoadTerrainModel(fileName.ascii());
    }
}


void CNavigationAttrPanel::OnHeightSliderChanged()
{
    double min =  (double) HeightSlider->minValue();
    double max = (double) HeightSlider->maxValue();
    double val = (double) HeightSlider->value();
    
    m_dHeight = 0.0;
    m_dHeight += ((val - min) / (max-min)) * m_dLowerHeight;
    m_dHeight += ((max - val) / (max-min)) * m_dUpperHeight;
    
    QString text = QString("%1").arg(m_dHeight, 0, 'f', 3);
    HeightEdit->setText(text);
    CNavigationProjectController::Instance()->SetHeightAboutGround(m_dHeight);
}


void CNavigationAttrPanel::OnUpperHeightLineEditChanged()
{
    QString text = UpperHeightLineEdit->text();
    if (! text.isEmpty()){
	m_dUpperHeight = text.toDouble();
    }
}


void CNavigationAttrPanel::OnLowerHeightLineEditChanged()
{
    QString text = LowerHeightLineEdit->text();
    if (! text.isEmpty()){
	m_dLowerHeight = text.toDouble();
    }
}
