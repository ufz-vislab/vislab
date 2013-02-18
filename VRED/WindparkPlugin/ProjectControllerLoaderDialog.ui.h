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
void CProjectControllerLoaderDialog::init(){
    m_qstrWorkingDirectory = QString(".");
}

void CProjectControllerLoaderDialog::OnCloseButtonPressed()
{
    this->hide();
}


void CProjectControllerLoaderDialog::OnLoadAllButtonPressed()
{
    if (m_ProjectControllerLoader.AllDataComplete()){
	m_ProjectControllerLoader.LoadAll();
    }else{
	QMessageBox::warning(this, QString("Warning"), QString("Not all necessary information has been completed, could not load files!"), 0, 0);
    }
}


void CProjectControllerLoaderDialog::OnCurrentDirectoryButtonPressed()
{
    m_qstrWorkingDirectory = QFileDialog::getExistingDirectory(".");
    WorkingDirectoryLineEdit->setText(m_qstrWorkingDirectory);
}


void CProjectControllerLoaderDialog::OnTerrainFilenameButtonPressed()
{
    QString fileFilters = tr("OpenSG dateien (*.osb)\n");
    QString fileName = QFileDialog::getOpenFileName(m_qstrWorkingDirectory , fileFilters, this);
    if (! fileName.isEmpty()){
	m_ProjectControllerLoader.SetTerrainFilename(fileName.ascii());
	TerrainFilenameLineEdit->setText(fileName);
    }
}


void CProjectControllerLoaderDialog::OnArealImageFilenameButtonPressed()
{
    QString fileFilters = tr("png dateien (*.png)\n"
			   "jpg dateien (*.jpg)\n");
    QString fileName = QFileDialog::getOpenFileName(m_qstrWorkingDirectory , fileFilters, this);
    if (! fileName.isEmpty()){
	m_ProjectControllerLoader.SetArealImageFilename(fileName.ascii());
	ArealImageFilenameLineEdit->setText(fileName);
    }
}


void CProjectControllerLoaderDialog::OnWPETypeFilenameButtonPressed()
{
    QString fileFilters = tr("txt dateien (*.txt)\n");
    QString fileName = QFileDialog::getOpenFileName(m_qstrWorkingDirectory , fileFilters, this);
    if (! fileName.isEmpty()){
	m_ProjectControllerLoader.SetWPETypesFilename(fileName.ascii());
	WPETypesFilenameLineEdit->setText(fileName);
    }
}


void CProjectControllerLoaderDialog::OnWPE3DModelButtonPressed()
{
    QString fileFilters = tr("OpenSG dateien (*.osb)\n");
    QString fileName = QFileDialog::getOpenFileName(m_qstrWorkingDirectory , fileFilters, this);
    if (! fileName.isEmpty()){
	m_ProjectControllerLoader.SetWPE3DModelFilename(fileName);
	WPE3DModelFilenameLineEdit->setText(fileName);
    }
}
