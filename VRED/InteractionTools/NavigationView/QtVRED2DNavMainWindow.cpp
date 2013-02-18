#include "QtVRED2DNavMainWindow.h"

#include "QtNavCanvasView.h"

#include <qlabel.h>
#include <qstatusbar.h>
#include <qfont.h>

CQtVRED2DNavMainWindow::CQtVRED2DNavMainWindow(QWidget *parent, const char *name) : QMainWindow(parent, name){
	m_pNavCanvasView = new CQtNavCanvasView(this);

	this->setMinimumHeight(400);
	this->setMinimumWidth(600);

	this->setCentralWidget(m_pNavCanvasView);

	QFont textFont;
	textFont.setBold(true);
	textFont.setPointSize(24);
	m_pHeightLabel = new QLabel(this);
	m_pHeightLabel->setFont(textFont);
	m_pHeightLabel->setText(QString("Depth: not determined yet!"));
	
	this->statusBar()->addWidget(m_pHeightLabel);
}

CQtVRED2DNavMainWindow::~CQtVRED2DNavMainWindow(void){
}

void CQtVRED2DNavMainWindow::SetFromAtWithZUp(float fromX, float fromY, float atX, float atY, float height){
	m_pNavCanvasView->SetFromAtWorldSpace(fromX, fromY, atX, atY);

	QString heightText = QString("Height: %1").arg(height, 0, 'f', 3);
	m_pHeightLabel->setText(heightText);
}

void CQtVRED2DNavMainWindow::LoadSettingsFromXMLFile(const char *filename){
	m_pNavCanvasView->LoadSettingsFromXMLFile(filename);

	int winMinHeight = this->minimumHeight();
	int winMinWidth = this->minimumWidth();

	int canvasHeight = this->m_pNavCanvasView->GetHeight();
	int canvasWidth = this->m_pNavCanvasView->GetWidth();

	if (canvasHeight < winMinHeight) this->setMinimumHeight(canvasHeight);
	if (canvasWidth < winMinWidth) this->setMinimumWidth(canvasWidth);
}

void CQtVRED2DNavMainWindow::WriteExampleSettingsXMLFile(const char *filename){
	m_pNavCanvasView->WriteExampleXMLFile(filename);
}
