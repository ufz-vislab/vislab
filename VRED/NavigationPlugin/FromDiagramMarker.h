#pragma once

#include <qcanvas.h>

class QPainter;

class CFromDiagramMarker : public QCanvasRectangle {
protected:
	int m_iHalfSize;
	int m_iSize;
	int m_iBorder;

public:
	CFromDiagramMarker(QCanvas *canvas);
	~CFromDiagramMarker(void);

	enum {RTTI = 1003};
	
	int		rtti() const;
	QRect	boundingRect() const;
	void	drawShape(QPainter &painter);
	
	void	drawActiveHandle(QPainter &painter, const QPoint &center);
	double	GetCenterX();
	double	GetCenterY();
};
