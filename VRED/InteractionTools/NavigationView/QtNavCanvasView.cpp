#include "QtNavCanvasView.h"

#include <qimage.h>
#include <qpixmap.h>

#include <map>
#include <iostream>

#include "tinyxml.h"

#include "QtUserMarker.h"


CQtNavCanvasView::CQtNavCanvasView(QCanvas *canvas, QWidget *parent, const char *name) : QCanvasView(canvas, parent, name){
	canvas->resize(600, 400);
	setCanvas(canvas);

	m_pUserMarker = new CQtUserMarker(canvas);

	m_fBorder = 20;
	m_pfClipMinVec[0] = 20.0;
	m_pfClipMinVec[1] = 20.0;
	m_pfClipMaxVec[0] = 780.0;
	m_pfClipMaxVec[1] = 580.0;

	m_fXTranslation = 0.0;
	m_fYTranslation = 0.0;
	m_fXScaling = 1.0;
	m_fYScaling = 1.0;
}

CQtNavCanvasView::CQtNavCanvasView(QWidget *parent, const char *name) : QCanvasView(parent, name){
	QCanvas *myCanvas = new QCanvas(this);
	myCanvas->resize(600, 400);
	setCanvas(myCanvas);

	m_pUserMarker = new CQtUserMarker(myCanvas);

	m_fBorder = 20;
	m_pfClipMinVec[0] = 20.0;
	m_pfClipMinVec[1] = 20.0;
	m_pfClipMaxVec[0] = 780.0;
	m_pfClipMaxVec[1] = 580.0;

	m_fXTranslation = 0.0;
	m_fYTranslation = 0.0;
	m_fXScaling = 1.0;
	m_fYScaling = 1.0;
}

CQtNavCanvasView::~CQtNavCanvasView(void){
}

bool CQtNavCanvasView::LoadBackgroundImageAndResize(const char *filename){
	QPixmap pixmap;
	QImage image;
	if (image.load(QString(filename))){
		int width = image.width();
		int height = image.height();
		QCanvas *canvas = this->canvas();
		canvas->resize(width, height);

		pixmap.convertFromImage(image);
		canvas->setBackgroundPixmap(pixmap);

		m_pUserMarker->move(0.2 * canvas->width(), 0.2 * canvas->height());
		//m_pUserMarker->SetRotation(45);
		m_pUserMarker->show();
		canvas->update();

		this->Adjust();
		return true;
	}else{
		return false;
	}
}

void CQtNavCanvasView::SetBorder(int pixels){
	m_fBorder = (float) pixels;
	Adjust();
}

void CQtNavCanvasView::SetFromAtWorldSpace(float fromX, float fromY, float atX, float atY){
	fromX += m_fXTranslation;
	fromY += m_fYTranslation;
	atX += m_fXTranslation;
	atY += m_fYTranslation;

	fromX *= m_fXScaling;
	fromY *= m_fYScaling;
	atX *= m_fXScaling;
	atY *= m_fYScaling;

	SetFromAtImageSpace(fromX, fromY, atX, atY);
}

void CQtNavCanvasView::SetFromAtImageSpace(float fromX, float fromY, float atX, float atY){
	float dirX = atX - fromX;
	float dirY = atY - fromY;
	float dirLength = sqrt(dirX * dirX + dirY * dirY);
	dirX /= dirLength;
	dirY /= dirLength;

	float rotation;

	float  acosvalue = acos(dirY);

	if (dirX < 0.0){
		rotation = 360.0 - (acosvalue * 180.0 / M_PI);
	} else {
		rotation = acosvalue * 180.0 / M_PI;
	}

	float posX, posY;
	int color = this->ClipUserMarker(fromX, fromY, dirX, dirY, posX, posY);
	posY = this->height() - posY;

	m_pUserMarker->SetRotation(rotation);
	m_pUserMarker->move(posX, posY);
	if (color == 1){
		QPen pen = m_pUserMarker->pen();
		QBrush brush = m_pUserMarker->brush();
		pen.setColor(green);
		brush.setColor(green);
		m_pUserMarker->setPen(pen);
		m_pUserMarker->setBrush(brush);
	} else if (color == 2){
		QPen pen = m_pUserMarker->pen();
		QBrush brush = m_pUserMarker->brush();
		pen.setColor(blue);
		brush.setColor(blue);
		m_pUserMarker->setPen(pen);
		m_pUserMarker->setBrush(brush);
	} else if (color == 3){
		QPen pen = m_pUserMarker->pen();
		QBrush brush = m_pUserMarker->brush();
		pen.setColor(red);
		brush.setColor(red);
		m_pUserMarker->setPen(pen);
		m_pUserMarker->setBrush(brush);
	}
	this->canvas()->update();
}

void CQtNavCanvasView::LoadSettingsFromXMLFile(const char *filename){
	TiXmlDocument *doc = new TiXmlDocument(filename);
	if (doc->LoadFile()){
		std::cout << "CQtNavCanvasView::LoadSettingsFromXMLFile(), could load file: " << filename << std::endl;
	} else {
		std::cout << "CQtNavCanvasView::LoadSettingsFromXMLFile, could not load file: " << filename << std::endl;
		return;
	}

	TiXmlElement *root = doc->FirstChildElement();
	TiXmlElement *navWindow = root->FirstChildElement("NavigationWindow");

	if (navWindow != NULL){
		TiXmlElement *baseImage = navWindow->FirstChildElement("BaseImage");
		if (baseImage != NULL){
			const char *filename = baseImage->Attribute("file");
			if (filename != NULL){
				this->LoadBackgroundImageAndResize(filename);
			} else {
				std::cout << "    name for file with the base image is required! Stopping load process!" << std::endl;
				return;
			}
		}

		TiXmlElement *translation = navWindow->FirstChildElement("Translation");
		if (translation != NULL){
			const char *xtAsChar = translation->Attribute("xt");
			const char *ytAsChar = translation->Attribute("yt");
			if (xtAsChar != NULL){
				m_fXTranslation = atof(xtAsChar);
			} else {
				m_fXTranslation = 0.0;
			}
			if (ytAsChar != NULL){
				m_fYTranslation = atof(ytAsChar);
			} else {
				m_fYTranslation = 0.0;
			}
		}

		TiXmlElement *scaling = navWindow->FirstChildElement("Scaling");
		if (scaling != NULL){
			const char *xsAsChar = scaling->Attribute("xs");
			const char *ysAsChar = scaling->Attribute("ys");
			if (xsAsChar != NULL){
				m_fXScaling = atof(xsAsChar);
			} else {
				m_fXScaling = 1.0;
			}
			if (ysAsChar != NULL){
				m_fYScaling = atof(ysAsChar);
			} else {
				m_fYScaling = 1.0;
			}
		}
	}
}
	
void CQtNavCanvasView::WriteExampleXMLFile(const char *filename){
	TiXmlDocument doc;
	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	TiXmlElement *root = new TiXmlElement("Root");
	doc.LinkEndChild(root);

	TiXmlElement *navWindow = new TiXmlElement("NavigationWindow");
	root->LinkEndChild(navWindow);

	TiXmlElement *baseImage = new TiXmlElement("BaseImage");
	baseImage->SetAttribute("file", "some/path/file.xml");
	navWindow->LinkEndChild(baseImage);

	TiXmlElement *translation = new TiXmlElement("Translation");
	translation->SetAttribute("xt", "0.0");
	translation->SetAttribute("yt", "0.0");
	navWindow->LinkEndChild(translation);

	TiXmlElement *scaling = new TiXmlElement("Scaling");
	scaling->SetAttribute("xs", "1.0");
	scaling->SetAttribute("ys", "1.0");
	navWindow->LinkEndChild(scaling);

	doc.SaveFile(filename);
}

void CQtNavCanvasView::Adjust(){
	QCanvas *canvas = this->canvas();
	float fWidth = canvas->width();
	float fHeight = canvas->height();

	m_pfClipMinVec[0] = m_fBorder;
	m_pfClipMinVec[1] = m_fBorder;
	m_pfClipMaxVec[0] = fWidth - m_fBorder;
	m_pfClipMaxVec[1] = fHeight - m_fBorder;
}

int CQtNavCanvasView::ClipUserMarker(float fXPos, float fYPos, float fXDir, float fYDir, float &fClippedXPos, float &fClippedYPos){
	std::multimap<float, MyPoint> stdmapABSTToPoints;
	std::multimap<float, MyPoint>::iterator pointsIter;

	//If the point is in the clipping rectangle it keeps its values
	if ((fXPos >= m_pfClipMinVec[0]) && (fXPos <= m_pfClipMaxVec[0]) &&
		(fYPos >= m_pfClipMinVec[1]) && (fYPos <= m_pfClipMaxVec[1])){
		fClippedXPos = fXPos;
		fClippedYPos = fYPos;
		return 1;
	}

	//The point is not in the clipping rectangle, so we have to check if the
	//view direction intersects the clipping rectangle. First we have to check if the
	//line is parallel to the edges
	float t, tabs;
	MyPoint intersectPnt;

	//Left border
	if (fXDir != 0.0){
		t = (m_pfClipMinVec[0] - fXPos) / fXDir;
		intersectPnt.x = fXPos + t * fXDir;
		intersectPnt.y = fYPos + t * fYDir;
		(t < 0.0) ? (tabs = -t) : (tabs = t);
		stdmapABSTToPoints.insert(std::map<float, MyPoint>::value_type(tabs, intersectPnt));
	}

	//Right border
	if (fXDir != 0.0){
		t = (m_pfClipMaxVec[0] - fXPos) / fXDir;
		intersectPnt.x = fXPos + t * fXDir;
		intersectPnt.y = fYPos + t * fYDir;
		(t < 0.0) ? (tabs = -t) : (tabs = t);
		stdmapABSTToPoints.insert(std::map<float, MyPoint>::value_type(tabs, intersectPnt));
	}

	//Top border
	if (fYDir != 0.0){
		t = (m_pfClipMaxVec[1] - fYPos) / fYDir;
		intersectPnt.x = fXPos + t * fXDir;
		intersectPnt.y = fYPos + t * fYDir;
		(t < 0.0) ? (tabs = -t) : (tabs = t);
		stdmapABSTToPoints.insert(std::map<float, MyPoint>::value_type(tabs, intersectPnt));
	}

	//bottom border
	if (fYDir != 0.0){
		t = (m_pfClipMinVec[1] - fYPos) / fYDir;
		intersectPnt.x = fXPos + t * fXDir;
		intersectPnt.y = fYPos + t * fYDir;
		(t < 0.0) ? (tabs = -t) : (tabs = t);
		stdmapABSTToPoints.insert(std::map<float, MyPoint>::value_type(tabs, intersectPnt));
	}

	float eps = 0.001 * width();

	for (pointsIter = stdmapABSTToPoints.begin(); pointsIter != stdmapABSTToPoints.end(); pointsIter++){
		MyPoint pnt = (*pointsIter).second;
		if ((pnt.x >= (m_pfClipMinVec[0] - eps)) && (pnt.x <= (m_pfClipMaxVec[0] + eps)) &&
			(pnt.y >= (m_pfClipMinVec[1] - eps)) && (pnt.y <= (m_pfClipMaxVec[1] + eps))){
			fClippedXPos = pnt.x;
			fClippedYPos = pnt.y;
			return 2;
		}
	}

	//The view ray does not intersect the clip rectangle, so we want to relate the marker
	//to the corner point that comes nearest to the clipped rectangle
	float dist = 0.0;
	float fNX = fYDir;
	float fNY = -fXDir;
	float fNLength = sqrt(fNX * fNX + fNY * fNY);
	fNX /= fNLength;
	fNY /= fNLength;
	stdmapABSTToPoints.clear();

	//Upper left (dist = <CornerPnt - P, N>)
	dist = (m_pfClipMinVec[0] - fXPos) * fNX + (m_pfClipMaxVec[1] - fYPos) * fNY;
	if (dist < 0.0) dist *= - 1.0;
	stdmapABSTToPoints.insert(std::map<float, MyPoint>::value_type(dist, MyPoint(m_pfClipMinVec[0], m_pfClipMaxVec[1])));

	//Upper right (dist = <CornerPnt - P, N>)
	dist = (m_pfClipMaxVec[0] - fXPos) * fNX + (m_pfClipMaxVec[1] - fYPos) * fNY;
	if (dist < 0.0) dist *= - 1.0;
	stdmapABSTToPoints.insert(std::map<float, MyPoint>::value_type(dist, MyPoint(m_pfClipMaxVec[0], m_pfClipMaxVec[1])));

	//Lower right (dist = <CornerPnt - P, N>)
	dist = (m_pfClipMaxVec[0] - fXPos) * fNX + (m_pfClipMinVec[1] - fYPos) * fNY;
	if (dist < 0.0) dist *= - 1.0;
	stdmapABSTToPoints.insert(std::map<float, MyPoint>::value_type(dist, MyPoint(m_pfClipMaxVec[0], m_pfClipMinVec[1])));

	//Lower left (dist = <CornerPnt - P, N>)
	dist = (m_pfClipMinVec[0] - fXPos) * fNX + (m_pfClipMinVec[1] - fYPos) * fNY;
	if (dist < 0.0) dist *= - 1.0;
	stdmapABSTToPoints.insert(std::map<float, MyPoint>::value_type(dist, MyPoint(m_pfClipMinVec[0], m_pfClipMinVec[1])));

	pointsIter = stdmapABSTToPoints.begin();
	fClippedXPos = (*pointsIter).second.x;
	fClippedYPos = (*pointsIter).second.y;

	return 3;
}
