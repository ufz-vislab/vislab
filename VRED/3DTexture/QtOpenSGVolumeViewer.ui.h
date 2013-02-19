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


void QVRED3DTexturePanel::OnLoadFileButtonClicked(){
    QString filename = QFileDialog::getOpenFileName(
	    "E:/zehner_work/" , "3D Texture (*.bpf)", this, "open texture file dialog", "Choose a file");
    m_3DTexture.LoadImage(filename.ascii());
}
