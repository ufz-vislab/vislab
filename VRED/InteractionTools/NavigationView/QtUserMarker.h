#pragma once

#include <qcanvas.h>

#define _USE_MATH_DEFINES 
#include <cmath>

class QPainter;

class CQtUserMarker : public QCanvasRectangle {
protected:
	int m_iHalfSize;
	int m_iSize;
	int m_iBorder;
	float m_fRotation;

	float m_pdTransform[2][2];

public:
	CQtUserMarker(QCanvas *canvas);
	~CQtUserMarker(void);

	enum {RTTI = 1003};

	void	SetSize(int size){m_iSize = size;}
	void	SetRotation(float rotation){
		rotation -= 90.0;
		m_pdTransform[0][0] = cos(rotation * M_PI / 180.0);
		m_pdTransform[0][1] = -sin(rotation * M_PI / 180.0);
		m_pdTransform[1][0] = sin(rotation * M_PI / 180.0);
		m_pdTransform[1][1] = cos(rotation * M_PI / 180.0);
		this->update();
	}

	int		rtti() const;
	QRect	boundingRect() const;
	void	drawShape(QPainter &painter);
	
	void	drawActiveHandle(QPainter &painter, const QPoint &center);
	double	GetCenterX();
	double	GetCenterY();
};
