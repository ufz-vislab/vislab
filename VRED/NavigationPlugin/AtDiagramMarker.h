#pragma once

#include <qcanvas.h>

class CAtDiagramMarker : public QCanvasRectangle{
protected:
	int m_iHalfSize;
	int m_iSize;
	int m_iBorder;

public:
	CAtDiagramMarker(QCanvas *canvas);
	~CAtDiagramMarker(void);

	enum {RTTI = 1002};
	
	int		rtti() const;
	QRect	boundingRect() const;
	void	drawShape(QPainter &painter);
	
	void	drawActiveHandle(QPainter &painter, const QPoint &center);
	double	GetCenterX();
	double	GetCenterY();
};
