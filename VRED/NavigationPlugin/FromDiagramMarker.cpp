#include "FromDiagramMarker.h"

#include <qpainter.h>

CFromDiagramMarker::CFromDiagramMarker(QCanvas *canvas) : QCanvasRectangle(canvas){
	m_iSize = 50;
	m_iHalfSize = 10;
	m_iBorder = 2;

	setSize(m_iSize, m_iSize);
	QPen pen = green;
	pen.setWidth(2);
	setPen(pen);
	setBrush(Qt::NoBrush);
}

CFromDiagramMarker::~CFromDiagramMarker(void){
}

int CFromDiagramMarker::rtti() const {
	return RTTI;
}

QRect CFromDiagramMarker::boundingRect() const{
	return QRect((int) x() - m_iBorder - 4, (int) y() - m_iBorder - 4,
		width() + 2 * m_iBorder + 8, height() + 2 * m_iBorder + 8);
}

void CFromDiagramMarker::drawShape(QPainter &painter){
	QCanvasRectangle::drawShape(painter);
	int x1, y1, x2, y2;
	x1 = (int)(x() + 0.25*width());
	y1 = (int) (y() + 0.5 * height());
	x2 = (int)(x() + 0.75*width());
	y2 = (int) (y() + 0.5 * height());
	painter.drawLine(x1, y1, x2, y2);
	x1 = (int) (x() + 0.5 * width());
	y1 = (int) (y() + 0.25 * height());
	x2 = (int) (x() + 0.5 * width());
	y2 = (int) (y() + 0.75 * height());
	painter.drawLine(x1, y1, x2, y2);
	if (isActive()){
		drawActiveHandle(painter, rect().topLeft());
		drawActiveHandle(painter, rect().topRight());
		drawActiveHandle(painter, rect().bottomLeft());
		drawActiveHandle(painter, rect().bottomRight());
	}
}

void CFromDiagramMarker::drawActiveHandle(QPainter &painter, const QPoint &center){
	painter.setPen(Qt::black);
	painter.setBrush(Qt::gray);
	painter.drawRect(center.x() - m_iBorder, center.y() - m_iBorder,
		2 * m_iBorder + 1, 2 * m_iBorder + 1);
}

double	CFromDiagramMarker::GetCenterX(){
	return (x() + 0.5 * width());
}

double	CFromDiagramMarker::GetCenterY(){
	return (y() + 0.5 * height());
}