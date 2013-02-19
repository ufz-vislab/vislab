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

void QVRED3DTexturePanel::OnAddToVREDButtonClicked()
{
    std::cout << "Add to VRED will be called" << std::endl;
    m_3DTexture.Init();
    m_3DTexture.AddToVRED();
}


void QVRED3DTexturePanel::OnRemoveFromVREDButtonClicked()
{
    m_3DTexture.RemoveFromVRED();
}


void QVRED3DTexturePanel::OnLoadFromFileButtonClicked(){
    QString filename = QFileDialog::getOpenFileName(
	    "E:/home/zehner_work", "3D Textures (*.bpf)", this);
    if (! filename.isEmpty()){
	QString trfilename(filename);
	trfilename.append(".tra");
	m_3DTexture.LoadImage(filename.ascii());
	m_3DTexture.LoadTransformation(trfilename.ascii());
	
	//OnSliceValuesChanged();
    }
}






void QVRED3DTexturePanel::OnSliceValuesChanged()
{
    bool bX = XSliceRadioButton->isChecked();
    bool bY = YSliceRadioButton->isChecked();
    bool bZ = ZSliceRadioButton->isChecked();
    
    float fX = ((float)(XSliceSlider->value() - XSliceSlider->minValue())) / ((float)(XSliceSlider->maxValue() - XSliceSlider->minValue()));
    float fY = ((float)(YSliceSlider->value() - YSliceSlider->minValue())) / ((float)(XSliceSlider->maxValue() - XSliceSlider->minValue()));
    float fZ = ((float)(ZSliceSlider->value() - ZSliceSlider->minValue())) / ((float)(ZSliceSlider->maxValue() - ZSliceSlider->minValue()));
    
    m_3DTexture.SetSlicesRel(bX, bY, bZ, fX, fY, fZ);

}


void QVRED3DTexturePanel::OnUseColorTableRadioButtonClicked()
{
    m_3DTexture.SetUseColortable(UseColorTableRadioButton->isChecked());
}


void QVRED3DTexturePanel::OnClipValueChanged()
{
    float fLowClip = ((float)(LowClipValueSlider->value() - LowClipValueSlider->minValue())) / ((float) (LowClipValueSlider->maxValue() - LowClipValueSlider->minValue()));
    float fHighClip = ((float)(HighClipValueSlider->value() - HighClipValueSlider->minValue())) / ((float) (HighClipValueSlider->maxValue() - HighClipValueSlider->minValue()));
    m_3DTexture.SetColortable(fLowClip, fHighClip);
}
