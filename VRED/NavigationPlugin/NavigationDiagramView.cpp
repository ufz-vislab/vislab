#include "NavigationDiagramView.h"

#include "FromDiagramMarker.h"
#include "AtDiagramMarker.h"
#include "qpixmap.h"
#include "qimage.h"
#include "qwmatrix.h"

#include "NavigationProjectController.h"

CNavigationDiagramView::CNavigationDiagramView(QCanvas *canvas, QWidget *parent, const char *name): QCanvasView(canvas, parent, name){
	m_pFromDiagramMarker = new CFromDiagramMarker(canvas);
	m_pFromDiagramMarker->move(0.4 * canvas->width(), 0.5 * canvas->height()); 
	m_pAtDiagramMarker = new CAtDiagramMarker(canvas);
	m_pAtDiagramMarker->move(0.6 * canvas->width(), 0.5 * canvas->height());
	canvas->update();
}

CNavigationDiagramView::CNavigationDiagramView(QWidget *parent, const char *name):QCanvasView(parent, name){
	m_pqActiveItem = NULL;
	QCanvas *myCanvas = new QCanvas(this);
	myCanvas->resize(800, 600);
	setCanvas(myCanvas);

	m_pFromDiagramMarker = new CFromDiagramMarker(myCanvas);
	m_pFromDiagramMarker->move(0.4 * myCanvas->width(), 0.5 * myCanvas->height());
	m_pFromDiagramMarker->show();

	m_pAtDiagramMarker = new CAtDiagramMarker(myCanvas);
	m_pAtDiagramMarker->move(0.6 * myCanvas->width(), 0.5 * myCanvas->height());
	m_pAtDiagramMarker->show();

	myCanvas->update();
}

CNavigationDiagramView::~CNavigationDiagramView(void){
}

void CNavigationDiagramView::AddItem(QCanvasItem *item, int xPos, int yPos){
	item->move((double) xPos, (double) yPos);
	canvas()->update();
}

void CNavigationDiagramView::LoadBackgroundImageAndResize(const char *filename){
	QPixmap pixmap;
	QImage image;
	if (image.load(QString(filename))){
		int width = image.width();
		int height = image.height();
		QCanvas *canvas = this->canvas();
		canvas->resize(width, height);

		pixmap.convertFromImage(image);
		canvas->setBackgroundPixmap(pixmap);

		double x, y;

		m_pFromDiagramMarker->move(0.4 * canvas->width(), 0.5 * canvas->height());
		x = m_pFromDiagramMarker->GetCenterX();
		y = m_pFromDiagramMarker->GetCenterY();
		CNavigationProjectController::Instance()->SetNewFromPosition(x, y);

		m_pAtDiagramMarker->move(0.6 * canvas->width(), 0.5 * canvas->height());
		x = m_pAtDiagramMarker->GetCenterX();
		y = m_pAtDiagramMarker->GetCenterY();
		CNavigationProjectController::Instance()->SetNewAtPosition(x, y);
		canvas->update();
	}else{
	}
}

void CNavigationDiagramView::GetCanvasSize(double &xsize, double &ysize){
	QCanvas *canvas = this->canvas();
	QSize size = canvas->size();
	xsize = size.width();
	ysize = size.height();
}

void CNavigationDiagramView::ScaleDown(){
	QWMatrix matrix = worldMatrix();
	matrix.scale(0.75, 0.75);
	setWorldMatrix(matrix);
}

void CNavigationDiagramView::ScaleUp(){
	QWMatrix matrix = worldMatrix();
	matrix.scale((4.0/3.0), (4.0/3.0));
	setWorldMatrix(matrix);
}

void CNavigationDiagramView::contentsMousePressEvent(QMouseEvent * e){
	QWMatrix matrix = inverseWorldMatrix();
	int xPos = matrix.m11() * e->pos().x() + matrix.m12() * e->pos().y();
	int yPos = matrix.m21() * e->pos().x() + matrix.m22() * e->pos().y();
	QPoint p(xPos, yPos);

	QCanvasItemList items = canvas()->collisions(p);
	if (items.empty()){
		setActiveItem(0);
	}else{
		setActiveItem(*items.begin());
	}
	m_qLastPos = e->pos();
}

void CNavigationDiagramView::contentsMouseReleaseEvent(QMouseEvent * e){
}
void CNavigationDiagramView::contentsMouseDoubleClickEvent(QMouseEvent * e){
}

void CNavigationDiagramView::contentsMouseMoveEvent(QMouseEvent * e){
	QWMatrix matrix = inverseWorldMatrix();
	double x = matrix.m11() * e->x() + matrix.m12() * e->y();
	double y = matrix.m21() * e->x() + matrix.m22() * e->y();

	if (! canvas()->onCanvas(x, y)) return;

	if (m_pqActiveItem){
		

		double dxPosLokal = e->pos().x() - m_qLastPos.x();
		double dyPosLokal = e->pos().y() - m_qLastPos.y();

		double dxPos = matrix.m11() * dxPosLokal + matrix.m12() * dyPosLokal;
		double dyPos = matrix.m21() * dxPosLokal + matrix.m22() * dyPosLokal;

		//int newIntX = (int) (x() + dxPos);
		//int newIntY = (int) (y() + dyPos);
		//if (! canvas()->onCanvas(newIntX, newIntY)) return;

		m_pqActiveItem->moveBy(dxPos, dyPos);

		m_qLastPos = e->pos();
		canvas()->update();

		if (m_pqActiveItem->rtti() == CFromDiagramMarker::RTTI){
			CFromDiagramMarker *fromMarker = (CFromDiagramMarker*) m_pqActiveItem;
			float fXPos = (float) fromMarker->GetCenterX();
			float fYPos = (float) fromMarker->GetCenterY();
			CNavigationProjectController::Instance()->SetNewFromPosition(fXPos, fYPos);
		}else if (m_pqActiveItem->rtti() == CAtDiagramMarker::RTTI){
			CAtDiagramMarker *atMarker = (CAtDiagramMarker*) m_pqActiveItem;
			float fXPos = (float) atMarker->GetCenterX();
			float fYPos = (float) atMarker->GetCenterY();
			CNavigationProjectController::Instance()->SetNewAtPosition(fXPos, fYPos);
		}
	}
}

void CNavigationDiagramView::setActiveItem(QCanvasItem *item){
	if (item != m_pqActiveItem){
		if (m_pqActiveItem){
			m_pqActiveItem->setActive(false);
		}
		m_pqActiveItem = item;
		if (m_pqActiveItem){
			m_pqActiveItem->setActive(true);
		}
		canvas()->update();
	}
}