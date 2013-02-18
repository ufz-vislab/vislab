#pragma once
#include "qcanvas.h"

class CFromDiagramMarker;
class CAtDiagramMarker;

class CNavigationDiagramView : public QCanvasView
{
Q_OBJECT

public:
	CNavigationDiagramView(QCanvas *canvas, QWidget *parent = 0, const char *name = 0);
	CNavigationDiagramView(QWidget *parent = 0, const char *name = 0);
	~CNavigationDiagramView(void);

	void				AddItem(QCanvasItem *item, int xPos, int yPos);
	void				LoadBackgroundImageAndResize(const char *filename);
	void				GetCanvasSize(double &xsize, double &ysize);
	void				ScaleDown();
	void				ScaleUp();

protected:
	virtual void		contentsMousePressEvent(QMouseEvent * e);
	virtual void		contentsMouseReleaseEvent(QMouseEvent * e);
	virtual void		contentsMouseDoubleClickEvent(QMouseEvent * e);
	virtual void		contentsMouseMoveEvent(QMouseEvent * e);
	void				setActiveItem(QCanvasItem *item);

	QCanvasItem			*m_pqActiveItem;
	QPoint				m_qLastPos;

	CFromDiagramMarker	*m_pFromDiagramMarker;
	CAtDiagramMarker	*m_pAtDiagramMarker;
};
