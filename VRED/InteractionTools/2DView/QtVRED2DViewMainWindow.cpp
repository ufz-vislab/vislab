#include "QtVRED2DViewMainWindow.h"
#include "ART2DImageViewer.h"

#include <iostream>

CQtVRED2DViewMainWindow::CQtVRED2DViewMainWindow(QWidget *parent, const char *name) : QMainWindow(parent, name){
	this->setMinimumSize(400, 300);

	m_pCurrent2DViewWidget = NULL;
	
	m_pImageViewer = new CART2DImageViewer(this, "image viewer"); 
}

CQtVRED2DViewMainWindow::~CQtVRED2DViewMainWindow(void){
}

void CQtVRED2DViewMainWindow::Move(int x, int y){
	if (m_pCurrent2DViewWidget != NULL){
		m_pCurrent2DViewWidget->Scroll(x, y);
	}
}

void CQtVRED2DViewMainWindow::Zoom(double factor){
	if (m_pCurrent2DViewWidget != NULL){
		m_pCurrent2DViewWidget->Zoom(factor);
	}
}

void CQtVRED2DViewMainWindow::Show2DViewFor(std::string filename){
	std::cout << "CQtVRED2DViewMainWindow::Show2DViewFor(file=" << filename << ") called!" << std::endl;
	if (m_pCurrent2DViewWidget != NULL){
		m_pCurrent2DViewWidget->hide();
	}

	std::string::size_type pos = filename.find(".jpg");
	if (pos != std::string::npos){
		m_pImageViewer->ShowInfoFor(filename);
		m_pCurrent2DViewWidget = m_pImageViewer;
		this->setCentralWidget(m_pCurrent2DViewWidget);
		m_pCurrent2DViewWidget->show();
	} else {
		std::cout << "CQtVRED2DViewMainWindow::Show2DViewFor(filename=" << filename << ")" << std::endl;
		std::cout << "     file has no suffix, can not recognize type!" << std::endl;
	}
}
