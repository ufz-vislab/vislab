#pragma once

#include <string>
#include <map>

#include <qmainwindow.h>
#include "ART2DWindowInterface.h"
#include "IQtART2DViewWidget.h"
#include "ART2DImageViewer.h"

class CQtVRED2DViewMainWindow : public QMainWindow, public CART2DWindowInterface {
	Q_OBJECT
public:
	CQtVRED2DViewMainWindow(QWidget *parent = 0, const char *name = 0);

	~CQtVRED2DViewMainWindow(void);

	virtual void Move(int x, int y);
	virtual void Zoom(double factor);
	virtual void Show2DViewFor(std::string filename);

private:
	IQtART2DViewWidget* m_pCurrent2DViewWidget;
	CART2DImageViewer* m_pImageViewer;
};
