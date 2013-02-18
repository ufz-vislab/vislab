#pragma once
#include "iqtart2dviewwidget.h"

//class CART2DImageViewer : public IQtART2DViewWidget

#include "qwidget.h"

#include <qpixmap.h>
#include <qimage.h>
#include <qwidget.h>

#include <map>
#include <vector>

//class QToolButton;

//#include "HorizontalGraph2D.h"

class CART2DImageViewer : public IQtART2DViewWidget
{
	Q_OBJECT

public:
	CART2DImageViewer(QWidget *parent = 0, const char *name = 0, WFlags flags = 0);
	~CART2DImageViewer(void);

	void LoadImage(const char *fileName);

	QSize minimumSizeHint() const;
	QSize sizeHint() const;

	void ZoomIn();
	void ZoomOut();
	void Scroll(int x, int y);
	void ScrollUp();
	void ScrollDown();
	void ScrollLeft();
	void ScrollRight();
	void ShowInfoFor(std::string name){LoadImage(name.c_str());}

protected:
	// all reimplemented from QWidget (virtual functions)
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void wheelEvent(QWheelEvent *event);

private:
	void RefreshPixmap();
	void DrawImage(QPainter *painter);
	
	void Zoom(double factor);

	int m_iImageWidth;
	int m_iImageHeight;
	int m_iScrollX;
	int m_iScrollY;
	int m_iScrollStep;
	double m_dCurrentZoomFactor;

	QPixmap m_qPixmap;
	QPixmap m_qImageAsPixmap;
	QImage	m_qImageAsImage;
};