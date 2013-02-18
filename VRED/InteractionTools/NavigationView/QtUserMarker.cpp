#include "QtUserMarker.h"
#include <qpainter.h>

CQtUserMarker::CQtUserMarker(QCanvas *canvas) : QCanvasRectangle(canvas){
	m_iSize = 50;
	//m_iHalfSize = 10;
	m_iBorder = 2;

	setSize(m_iSize, m_iSize);
	QPen pen = green;
	QBrush brush(green);
	pen.setWidth(2);
	setPen(pen);
	setBrush(brush);

	SetRotation(0.0);
}

CQtUserMarker::~CQtUserMarker(void){
}

int CQtUserMarker::rtti() const {
	return RTTI;
}

QRect CQtUserMarker::boundingRect() const{
	return QRect((int) x() - 0.5 * width(), (int) y() - 0.5 * height(), width() , height());
}

void CQtUserMarker::drawShape(QPainter &painter){
	int i;
	//ArrowHead
	float xah[4], yah[4];
	xah[0] = 0.2 * width();		yah[0] = 0.2 * height();
	xah[1] = 0.5 * width();		yah[1] = 0.0;
	xah[2] = 0.2 * width();		yah[2] = -0.2 * height();
	xah[3] = 0.3 * width();		yah[3] = 0.0;

	//the cross
	float xcross[4], ycross[4];
	xcross[0] = -0.1 * width(); ycross[0] = 0.1 * height();
	xcross[1] = 0.1 * width();	ycross[1] = -0.1 * height();
	xcross[2] = -0.1 * width();	ycross[2] = -0.1 * height();
	xcross[3] = 0.1 * width();	ycross[3] = 0.1 * height();

	//the tail
	float xtail[4], ytail[4];
	xtail[0] = -0.5 * width(); ytail[0] = 0.1 * height();
	xtail[1] = -0.4 * width(); ytail[1] = 0.1 * height();
	xtail[2] = -0.4 * width(); ytail[2] = -0.1 * height();
	xtail[3] = -0.5 * width(); ytail[3] = -0.1 * height();

	//rotation of all objects
	float fx, fy;
	for (i=0; i<4; i++){
		fx = m_pdTransform[0][0] * xah[i] + m_pdTransform[0][1] * yah[i];
		fy = m_pdTransform[1][0] * xah[i] + m_pdTransform[1][1] * yah[i];
		xah[i] = fx;
		yah[i] = fy;
	}
	for (i=0; i<4; i++){
		fx = m_pdTransform[0][0] * xcross[i] + m_pdTransform[0][1] * ycross[i];
		fy = m_pdTransform[1][0] * xcross[i] + m_pdTransform[1][1] * ycross[i];
		xcross[i] = fx;
		ycross[i] = fy;
	}
	for (i=0; i<4; i++){
		fx = m_pdTransform[0][0] * xtail[i] + m_pdTransform[0][1] * ytail[i];
		fy = m_pdTransform[1][0] * xtail[i] + m_pdTransform[1][1] * ytail[i];
		xtail[i] = fx;
		ytail[i] = fy;
	}

	//Drawing the arrow head
	QPointArray ArrowHeadPointArray(4);
	for (i=0; i<4; i++){
		ArrowHeadPointArray.setPoint(i, xah[i] + x(), yah[i] + y());
	}
	painter.drawPolygon(ArrowHeadPointArray);

	//Drawing the cross
	painter.drawLine(xcross[0] + x(), ycross[0] + y(), xcross[1] + x(), ycross[1] + y());
	painter.drawLine(xcross[2] + x(), ycross[2] + y(), xcross[3] + x(), ycross[3] + y());

	//Drawing the tail
	QPointArray TailPointArray(4);
	for (i=0; i<4; i++){
		TailPointArray.setPoint(i, xtail[i] + x(), ytail[i] + y());
	}
	painter.drawPolygon(TailPointArray);

	//int x1, y1, x2, y2;
	//x1 = (int)(x() + 0.25*width());
	//y1 = (int) (y() + 0.5 * height());
	//x2 = (int)(x() + 0.75*width());
	//y2 = (int) (y() + 0.5 * height());
	//painter.drawLine(x1, y1, x2, y2);
	//x1 = (int) (x() + 0.5 * width());
	//y1 = (int) (y() + 0.25 * height());
	//x2 = (int) (x() + 0.5 * width());
	//y2 = (int) (y() + 0.75 * height());
	//painter.drawLine(x1, y1, x2, y2);
	//if (isActive()){
	//	drawActiveHandle(painter, rect().topLeft());
	//	drawActiveHandle(painter, rect().topRight());
	//	drawActiveHandle(painter, rect().bottomLeft());
	//	drawActiveHandle(painter, rect().bottomRight());
	//}
}

void CQtUserMarker::drawActiveHandle(QPainter &painter, const QPoint &center){
	painter.setPen(Qt::black);
	painter.setBrush(Qt::gray);
	painter.drawRect(center.x() - m_iBorder, center.y() - m_iBorder,
		2 * m_iBorder + 1, 2 * m_iBorder + 1);
}

double	CQtUserMarker::GetCenterX(){
	return (x() + 0.5 * width());
}

double	CQtUserMarker::GetCenterY(){
	return (y() + 0.5 * height());
}