#include "ART2DImageViewer.h"

#include <qpainter.h>
#include <qstyle.h>
#include <qtoolbutton.h>

#include <cmath>
#include <iostream>

#include <assert.h>

CART2DImageViewer::CART2DImageViewer(QWidget *parent, const char *name, WFlags flags) : IQtART2DViewWidget(parent, name, flags | WNoAutoErase){
	setBackgroundMode(Qt::PaletteDark);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setFocusPolicy(QWidget::StrongFocus);

	m_iImageWidth = 0;
	m_iImageHeight = 0;

	m_dCurrentZoomFactor = 1.0;
	m_iScrollX = 0;
	m_iScrollY = 0;
	m_iScrollStep = 20;
}

CART2DImageViewer::~CART2DImageViewer(void){
}

void CART2DImageViewer::LoadImage(const char *fileName){
	bool bImageCouldBeLoaded = m_qImageAsImage.load(QString(fileName));
	assert(bImageCouldBeLoaded);

	m_iImageWidth = m_qImageAsImage.width();
	m_iImageHeight = m_qImageAsImage.height();

	m_qImageAsPixmap.convertFromImage(m_qImageAsImage);
	this->RefreshPixmap();
}

QSize CART2DImageViewer::minimumSizeHint() const {
	return QSize(300, 200);
}

QSize CART2DImageViewer::sizeHint() const {
	return QSize(600, 400);
}

void CART2DImageViewer::ZoomIn(){
	Zoom(1.1);
	RefreshPixmap();
}

void CART2DImageViewer::ZoomOut(){
	Zoom(0.9);
	RefreshPixmap();
}

void CART2DImageViewer::Scroll(int x, int y){
	m_iScrollX += x;
	m_iScrollY += y;
	RefreshPixmap();
}

void CART2DImageViewer::ScrollUp(){
	Scroll(0, -m_iScrollStep);
	RefreshPixmap();
}

void CART2DImageViewer::ScrollDown(){
	Scroll(0, m_iScrollStep);
	RefreshPixmap();
}

void CART2DImageViewer::ScrollLeft(){
	Scroll(-m_iScrollStep, 0);
	RefreshPixmap();
}

void CART2DImageViewer::ScrollRight(){
	Scroll(m_iScrollStep, 0);
	RefreshPixmap();
}

void CART2DImageViewer::paintEvent(QPaintEvent *event){
	QMemArray<QRect> rects = event->region().rects();
	for (int i=0; i<(int)rects.size(); ++i){
		bitBlt(this, rects[i].topLeft(), &m_qPixmap, rects[i]);
	}

	QPainter painter(this);

	if (hasFocus()){
		style().drawPrimitive(QStyle::PE_FocusRect,
			&painter,
			rect(),
			colorGroup(),
			QStyle::Style_FocusAtBorder,
			colorGroup().dark());
	}
}

void CART2DImageViewer::resizeEvent(QResizeEvent *event){
	RefreshPixmap();
}

void CART2DImageViewer::keyPressEvent(QKeyEvent *event){
	switch (event->key()) {
		case Qt::Key_Plus:
		{
			ZoomIn();
			break;
		}
		case Qt::Key_Minus:
		{
			ZoomOut();
			break;
		}
		case Qt::Key_Left:
		{
			ScrollLeft();
			break;
		}
		case Qt::Key_Right:
		{
			ScrollRight();
			break;
		}
		case Qt::Key_Down:
		{
			ScrollDown();
			break;
		}
		case Qt::Key_Up:
		{
			ScrollUp();
			break;
		}
		default:
			QWidget::keyPressEvent(event);
	}
}

void CART2DImageViewer::wheelEvent(QWheelEvent *event){
	int numDegrees = event->delta() / 8;
	int numTicks = numDegrees / 15;

	if (event->orientation() == Qt::Horizontal){
		Scroll(1, 0);
	} else {
		Scroll(0, 1);
	}

	RefreshPixmap();
}

void CART2DImageViewer::RefreshPixmap(){
	m_qPixmap.resize(size());
	m_qPixmap.fill(this, 0, 0);
	QPainter painter(&m_qPixmap, this);
	DrawImage(&painter);
	update();
}

void CART2DImageViewer::DrawImage(QPainter *painter){
	bitBlt(&m_qPixmap, 0, 0, &m_qImageAsPixmap, m_iScrollX, m_iScrollY, this->width(), this->height());
}

void CART2DImageViewer::Zoom(double factor){
	std::cout << "CART2DImageViewer::Zoom(double factor): " << factor << std::endl; 
	double dOldZoomFactor = m_dCurrentZoomFactor;
	m_dCurrentZoomFactor *= factor;

	int iNewWidth = (int) (m_dCurrentZoomFactor * (double) m_iImageWidth);
	int iNewHeight = (int) (m_dCurrentZoomFactor * (double) m_iImageHeight);

	if ((iNewWidth > m_iImageWidth || iNewHeight > m_iImageHeight)){
		iNewWidth = m_iImageWidth;
		iNewHeight = m_iImageHeight;
		m_dCurrentZoomFactor = 1.0;
	}

	double dCenterXInSourceImage = (((double) m_iScrollX) + 0.5 * ((double) this->width())) / dOldZoomFactor;
	double dCenterYInSourceImage = (((double) m_iScrollY) + 0.5 * ((double) this->height())) / dOldZoomFactor;

	double dNewCenterXInSourceImage = dCenterXInSourceImage * m_dCurrentZoomFactor;
	double dNewCenterYInSourceImage = dCenterYInSourceImage * m_dCurrentZoomFactor;

	m_iScrollX = (int) (dNewCenterXInSourceImage - ((double)this->width()) / 2.0);
	m_iScrollY = (int) (dNewCenterYInSourceImage - ((double)this->height()) / 2.0);

	QImage resultImage = m_qImageAsImage.scale(iNewWidth, iNewHeight);
	m_qImageAsPixmap.convertFromImage(resultImage);
	this->RefreshPixmap();
}

