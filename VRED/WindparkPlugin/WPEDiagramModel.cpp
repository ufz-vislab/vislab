#include "WPEDiagramModel.h"

int CWPEDiagramModel::Margin = 2;

CWPEDiagramModel::CWPEDiagramModel(CWPEState state, QCanvas *canvas): QCanvasEllipse(canvas){
//CWPEDiagramModel::CWPEDiagramModel(QCanvas *canvas): QCanvasEllipse(canvas){
	m_state = state;

	m_iId = state.m_iId;
	m_stdstrWPETypeName = state.m_stdstrWPETypeName;
	m_iInfluenceRadius = (int) state.m_fInfluenceRadius;

	setSize(m_iInfluenceRadius * 2, m_iInfluenceRadius * 2);
	setPen(black);
	setBrush(gray);
}

CWPEDiagramModel::~CWPEDiagramModel(void){
	hide();
}

void CWPEDiagramModel::drawShape(QPainter &painter){
	QCanvasEllipse::drawShape(painter);
	//painter.drawText(boundingRect(), AlignCenter, QString(m_stdstrWPETypeName.c_str()));
	painter.drawText(x() - m_iInfluenceRadius + 2, y()+ 2, QString(m_stdstrWPETypeName.c_str()), 10);
	if (isActive()){
		drawActiveHandle(painter, shrinkedBoundingRect().topLeft());
		drawActiveHandle(painter, shrinkedBoundingRect().topRight());
		drawActiveHandle(painter, shrinkedBoundingRect().bottomLeft());
		drawActiveHandle(painter, shrinkedBoundingRect().bottomRight());
	}
}

QRect CWPEDiagramModel::boundingRect() const {
	int halfsize = Margin + (int) m_iInfluenceRadius;

	return QRect((int) x() - halfsize, (int) y() - halfsize,
		width() + 2 * Margin, height() + 2 * Margin);
}

QRect CWPEDiagramModel::shrinkedBoundingRect() const {
	int quartersize = (int) (m_iInfluenceRadius / 2);
	return QRect((int) x() - quartersize, (int) y() - quartersize,
		2 * quartersize, 2*quartersize);
}

void CWPEDiagramModel::SetState(CWPEState state){
	m_state = state;
	m_iId = state.m_iId;
	m_stdstrWPETypeName = state.m_stdstrWPETypeName;
	m_iInfluenceRadius = (int) state.m_fInfluenceRadius;
	setSize(m_iInfluenceRadius * 2, m_iInfluenceRadius * 2);
}

void CWPEDiagramModel::drawActiveHandle(QPainter &painter, const QPoint &center){
	painter.setPen(Qt::black);
	painter.setBrush(Qt::gray);
	painter.drawRect(center.x() - Margin, center.y() - Margin, 2 * Margin + 1, 2 * Margin + 1);
}