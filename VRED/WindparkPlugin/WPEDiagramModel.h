#pragma once

#include <qcanvas.h>
#include <qpainter.h>
#include <string>
#include <stdlib.h>

#include "WPEState.h"

class CWPEDiagramModel : public QCanvasEllipse
{
protected:
	static int		Margin;
	int				m_iId;					// Unique id
	std::string		m_stdstrWPETypeName;	//e.g. VESTAS123
	int				m_iInfluenceRadius;
	CWPEState		m_state;

	void drawActiveHandle(QPainter &painter, const QPoint &center);

public:
	enum {RTTI = 1001};
	CWPEDiagramModel(CWPEState state, QCanvas *canvas);
	//CWPEDiagramModel(QCanvas *canvas);
	~CWPEDiagramModel(void);

	//Qt required drawing functions
	void			drawShape(QPainter &painter);
	QRect			boundingRect() const;
	QRect			shrinkedBoundingRect() const;
	int				rtti() const {return RTTI;}
	void			SetState(CWPEState state);
	CWPEState		GetState(){return m_state;}

	//Get and Set functions
	void			SetId(int id){m_iId = id;}
	int				GetId(){return m_iId;}

	void			SetInfluenceRadius(int radius){m_iInfluenceRadius = radius;}
	void			SetInfluenceRadius(float radius){m_iInfluenceRadius = (int) radius;}
	int				GetInfluenceRadius(){return m_iInfluenceRadius;}

	void			SetWPETypeName(std::string name){m_stdstrWPETypeName = name;}
	std::string		GetWPETypeName(){return m_stdstrWPETypeName;}
};
