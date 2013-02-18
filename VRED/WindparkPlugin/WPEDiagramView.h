#pragma once

#include <qcanvas.h>
#include <stdlib.h>

class CWPEState;

class CWPEDiagramView : public QCanvasView
{
	Q_OBJECT
public:
	CWPEDiagramView(QCanvas *canvas, QWidget *parent = 0, const char *name = 0);
	CWPEDiagramView(QWidget *parent = 0, const char *name = 0);
	~CWPEDiagramView(void);

	void			AddItem(QCanvasItem *item);
	int				RemoveActiveItem(); //returns the id of the removed item
	int				GetIdOfActiveItem();
	void			AdaptActiveItemToNewState(CWPEState state);

	void			LoadBackgroundImageAndResize(const char *filename);
	void			GetCanvasSize(float &xsize, float &ysize);

	void			ScaleDown();
	void			ScaleUp();

	void			RemoveAllItems();

protected:
	virtual void	contentsMousePressEvent(QMouseEvent * e);
	virtual void	contentsMouseReleaseEvent(QMouseEvent * e);
	virtual void	contentsMouseDoubleClickEvent(QMouseEvent * e);
	virtual void	contentsMouseMoveEvent(QMouseEvent * e);

	virtual void	keyPressEvent(QKeyEvent *e);

	void			setActiveItem(QCanvasItem *item);
	void			showNewItem(QCanvasItem *item);
	void			bringToFront();
	void			sendToBack();

	QCanvasItem		*m_pqPendingItem;
	QCanvasItem		*m_pqActiveItem;
	QPoint			m_qLastPos;
	
	int				m_iMinZ;
	int				m_iMaxZ;
};