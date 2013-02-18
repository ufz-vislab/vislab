#pragma once
#include <qmainwindow.h>

#include "ARTNavWindowInterface.h"

class CQtNavCanvasView;
class QLabel;

class CQtVRED2DNavMainWindow : public QMainWindow, public CARTNavWindowInterface
{
	Q_OBJECT

public:
	CQtVRED2DNavMainWindow(QWidget *parent = 0, const char *name = 0);
	~CQtVRED2DNavMainWindow(void);

	virtual void SetFromAtWithZUp(float fromX, float fromY, float atX, float atY, float height);
	virtual void LoadSettingsFromXMLFile(const char *filename);
	virtual void WriteExampleSettingsXMLFile(const char *filename);

protected:
	CQtNavCanvasView	*m_pNavCanvasView;
	QLabel				*m_pHeightLabel;
};
