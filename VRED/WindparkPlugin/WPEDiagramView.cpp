#include "WPEDiagramView.h"

#include <qpixmap.h>
#include <qimage.h>
#include <qwmatrix.h>

#include "WPEDiagramModel.h"
#include "WPEProjectController.h"

#include <iostream>

CWPEDiagramView::CWPEDiagramView(QCanvas *canvas, QWidget *parent, const char *name): QCanvasView(canvas, parent, name){
	m_pqPendingItem = NULL;
	m_pqActiveItem = NULL;
	m_iMinZ = 0;
	m_iMaxZ = 0;

	setFocusPolicy(QWidget::StrongFocus);
}

CWPEDiagramView::CWPEDiagramView(QWidget *parent, const char *name):QCanvasView(parent, name){
	m_pqPendingItem = NULL;
	m_pqActiveItem = NULL;
	m_iMinZ = 0;
	m_iMaxZ = 0;
	QCanvas *myCanvas = new QCanvas(this);
	myCanvas->resize(800, 600);
	setCanvas(myCanvas);

	setFocusPolicy(QWidget::StrongFocus);
}

CWPEDiagramView::~CWPEDiagramView(void){
}

void CWPEDiagramView::AddItem(QCanvasItem *item){
	delete m_pqPendingItem;
	m_pqPendingItem = item;
	setActiveItem(0);
	setCursor(crossCursor);
}

int CWPEDiagramView::RemoveActiveItem(){
	int returnvalue = 0;
	if (m_pqActiveItem){
		QCanvasItem *item = m_pqActiveItem;
		setActiveItem(0);
		if (item->rtti() == CWPEDiagramModel::RTTI){
			CWPEDiagramModel *model = (CWPEDiagramModel*) item;
			returnvalue = model->GetId();
		}
		delete item;
		canvas()->update();
	}
	return returnvalue;
}

int CWPEDiagramView::GetIdOfActiveItem(){
	int returnvalue = 0;
	if (m_pqActiveItem){
		QCanvasItem *item = m_pqActiveItem;
		if (item->rtti() == CWPEDiagramModel::RTTI){
			CWPEDiagramModel *model = (CWPEDiagramModel*) item;
			returnvalue = model->GetId();
		}
	}
	return returnvalue;
}

void CWPEDiagramView::AdaptActiveItemToNewState(CWPEState state){
	if (m_pqActiveItem){
		if (m_pqActiveItem->rtti() == CWPEDiagramModel::RTTI){
			CWPEDiagramModel *wpe = (CWPEDiagramModel*) m_pqActiveItem;
			if (wpe->GetId() == state.m_iId){
				wpe->SetState(state);
			}
		}
		m_pqActiveItem->show();
		canvas()->update();
	}
}

void CWPEDiagramView::LoadBackgroundImageAndResize(const char *filename){
	QPixmap pixmap;
	QImage image;
	if (image.load(QString(filename))){
		int width = image.width();
		int height = image.height();
		QCanvas *canvas = this->canvas();
		canvas->resize(width, height);

		pixmap.convertFromImage(image);
		canvas->setBackgroundPixmap(pixmap);
		canvas->update();
	}else{
		exit(0);
	}
}

void CWPEDiagramView::GetCanvasSize(float &xsize, float &ysize){
	QCanvas *canvas = this->canvas();
	QSize size = canvas->size();
	xsize = (float) size.width();
	ysize = (float) size.height();
}

void CWPEDiagramView::ScaleDown(){
	QWMatrix matrix = worldMatrix();
	matrix.scale(0.75, 0.75);
	setWorldMatrix(matrix);
}

void CWPEDiagramView::ScaleUp(){
	QWMatrix matrix = worldMatrix();
	matrix.scale((4.0 / 3.0), (4.0 / 3.0));
	setWorldMatrix(matrix);
}

void CWPEDiagramView::contentsMousePressEvent(QMouseEvent * e){
	this->setFocus();
	if (e->button() == LeftButton && m_pqPendingItem){
		QWMatrix matrix = inverseWorldMatrix();
		int xPos = matrix.m11() * e->pos().x() + matrix.m12() * e->pos().y();
		int yPos = matrix.m21() * e->pos().x() + matrix.m22() * e->pos().y();

		//m_pqPendingItem->move(e->pos().x(), e->pos().y());

		m_pqPendingItem->move(xPos, yPos);

		showNewItem(m_pqPendingItem);
		m_pqPendingItem = 0;
		unsetCursor();

		if (m_pqActiveItem->rtti() == CWPEDiagramModel::RTTI){
			CWPEDiagramModel *wpe = (CWPEDiagramModel*) m_pqActiveItem;
			int id = wpe->GetId();
			float fXPos = (float) m_pqActiveItem->x();
			float fYPos = (float) m_pqActiveItem->y();
			CWPEState state = wpe->GetState();
			//CWPEProjectController::Instance()->SetNew3DModelPosition(id, fXPos, fYPos);
			CWPEProjectController::Instance()->FinishAddWPE(state, fXPos, fYPos);
		}
	}else{
		QWMatrix matrix = inverseWorldMatrix();
		int xPos = matrix.m11() * e->pos().x() + matrix.m12() * e->pos().y();
		int yPos = matrix.m21() * e->pos().x() + matrix.m22() * e->pos().y();
		QPoint p(xPos, yPos);

		//QCanvasItemList items = canvas()->collisions(e->pos());
		QCanvasItemList items = canvas()->collisions(p);
		if (items.empty()){
			setActiveItem(0);
		}else{
			setActiveItem(*items.begin());
		}
	}
	m_qLastPos = e->pos();
}

void CWPEDiagramView::contentsMouseReleaseEvent(QMouseEvent * e){
}

void CWPEDiagramView::contentsMouseDoubleClickEvent(QMouseEvent * e){
}

void CWPEDiagramView::contentsMouseMoveEvent(QMouseEvent * e){
	if (e->state() & LeftButton){
		if (m_pqActiveItem){
			QWMatrix matrix = inverseWorldMatrix();

			int dxPosLokal = e->pos().x() - m_qLastPos.x();
			int dyPosLokal = e->pos().y() - m_qLastPos.y();

			int dxPos = matrix.m11() * dxPosLokal + matrix.m12() * dyPosLokal;
			int dyPos = matrix.m21() * dxPosLokal + matrix.m22() * dyPosLokal;

			//m_pqActiveItem->moveBy(e->pos().x() - m_qLastPos.x(), e->pos().y() - m_qLastPos.y());
			m_pqActiveItem->moveBy(dxPos, dyPos);

			m_qLastPos = e->pos();
			canvas()->update();

			if (m_pqActiveItem->rtti() == CWPEDiagramModel::RTTI){
				CWPEDiagramModel *wpe = (CWPEDiagramModel*) m_pqActiveItem;
				int id = wpe->GetId();
				float fXPos = (float) m_pqActiveItem->x();
				float fYPos = (float) m_pqActiveItem->y();
				CWPEProjectController::Instance()->SetNew3DModelPosition(id, fXPos, fYPos);
			}
		}
	}
}

void CWPEDiagramView::keyPressEvent(QKeyEvent *e){
	int keyId = e->key();
	if (keyId == Qt::Key_Insert){
		CWPEProjectController::Instance()->InitAddWPE();
	}else if (keyId == Qt::Key_Delete){
		CWPEProjectController::Instance()->RemoveActiveWPE();
	}else{
		e->ignore();
	}
}

void CWPEDiagramView::setActiveItem(QCanvasItem *item){
	if (item != m_pqActiveItem){
		if (m_pqActiveItem){
			m_pqActiveItem->setActive(false);
			if (m_pqActiveItem->rtti() == CWPEDiagramModel::RTTI){
				CWPEDiagramModel *model = (CWPEDiagramModel*) m_pqActiveItem;
				int id = model->GetState().m_iId;
				CWPEProjectController::Instance()->UnsetActiveWPE(id);
			}
		}
		m_pqActiveItem = item;
		if (m_pqActiveItem){
			m_pqActiveItem->setActive(true);
			if (m_pqActiveItem->rtti() == CWPEDiagramModel::RTTI){
				CWPEDiagramModel *model = (CWPEDiagramModel*) m_pqActiveItem;
				int id = model->GetState().m_iId;
				CWPEProjectController::Instance()->SetActiveWPE(id);
			}
		}
		canvas()->update();
	}
}

void CWPEDiagramView::showNewItem(QCanvasItem *item){
	setActiveItem(item);
	bringToFront();
	item->show();
	canvas()->update();
}

void CWPEDiagramView::bringToFront(){
	if (m_pqActiveItem){
		++m_iMaxZ;
		m_pqActiveItem->setZ(m_iMaxZ);
		canvas()->update();
	}
}

void CWPEDiagramView::sendToBack(){
	if (m_pqActiveItem){
		--m_iMinZ;
		m_pqActiveItem->setZ(m_iMinZ);
		canvas()->update();
	}
}

void CWPEDiagramView::RemoveAllItems(){
	QCanvasItemList itemList = canvas()->allItems();
	QCanvasItemList::iterator itemIterator;
	for (itemIterator = itemList.begin(); itemIterator != itemList.end();){
		QCanvasItem *item = *itemIterator;
		itemIterator = itemList.remove(itemIterator);

		m_pqActiveItem = item;

		CWPEProjectController::Instance()->RemoveActiveWPE();
	}
}