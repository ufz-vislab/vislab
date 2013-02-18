#pragma once

#include <qcanvas.h>
class CQtUserMarker;

class CQtNavCanvasView : public QCanvasView
{
	Q_OBJECT

public:
	CQtNavCanvasView(QCanvas *canvas, QWidget *parent = 0, const char *name = 0);
	CQtNavCanvasView(QWidget *parent = 0, const char *name = 0);
	~CQtNavCanvasView(void);

	bool	LoadBackgroundImageAndResize(const char *filename);
	void	SetBorder(int pixels);

	void	SetFromAtWorldSpace(float fromX, float fromY, float atX, float atY);
	void	SetFromAtImageSpace(float fromX, float fromY, float atX, float atY);

	void	LoadSettingsFromXMLFile(const char *filename);
	int		GetWidth(){return this->canvas()->width();}
	int		GetHeight(){return this->canvas()->height();}
	static void WriteExampleXMLFile(const char *filename);


protected:
	CQtUserMarker	*m_pUserMarker;

	float			m_fBorder;
	float			m_pfClipMinVec[2];
	float			m_pfClipMaxVec[2];

	float			m_fXTranslation;
	float			m_fYTranslation;

	float			m_fXScaling;
	float			m_fYScaling;

	void Adjust();
	int ClipUserMarker(float fXPos, float fYPos, float fXDir, float fYDir, float &fClippedXPos, float &fClippedYPos);

protected:
	class MyPoint{
	public:
		MyPoint(){};
		MyPoint(float inX, float inY){x = inX; y = inY;}
		float x;
		float y;
	};
};
