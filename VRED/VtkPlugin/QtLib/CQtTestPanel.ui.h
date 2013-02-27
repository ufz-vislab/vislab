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


void CQtTestPanel::SetLineEditText(const char* text)
{
    lineEdit1->setText(QString(text));
}


void CQtTestPanel::Update()
{

}


bool CQtTestPanel::SetSubject( CSubject * )
{
    return false;
}


void CQtTestPanel::SetInitialValues()
{

}
