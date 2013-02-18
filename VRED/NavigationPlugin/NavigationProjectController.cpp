#include <stdlib.h>
#include <math.h>

#include "NavigationProjectController.h"
#include "NavigationDiagramView.h"
#include "NavigationAttrPanel.h"

#include "ElevationGridBase.h"
#include "TriangleElevationGrid.h"

#include <vrOSGWidget.h>
#include <vrScenegraph.h>

CNavigationProjectController* CNavigationProjectController::m_pInstance = NULL;

CNavigationProjectController::CNavigationProjectController(void){
	m_pNavigationDiagramView = NULL;
	m_pNavigationAttrPanel = NULL;

	m_pElevationGrid = new CElevationGridBase;

	m_dCanvasXSize = 0.0;
	m_dCanvasYSize = 0.0;
	m_dDiagramTo3DXTranslate = 0.0;
	m_dDiagramTo3DYTranslate = 0.0;

	m_dFromX = 0.0;
	m_dFromY = 0.0;
	m_dFromZ = 0.0;
	m_dAtX = 0.0;
	m_dAtY = 0.0;
	m_dAtZ = 0.0;
	m_dHeightAboutGround = 2.0;
}

CNavigationProjectController *CNavigationProjectController::Instance(){
	if (m_pInstance == NULL){
		m_pInstance = new CNavigationProjectController;
	}
	return m_pInstance;
}

CNavigationProjectController::~CNavigationProjectController(void){
}

void CNavigationProjectController::SetDiagramView(CNavigationDiagramView *view){
	m_pNavigationDiagramView = view;
}

void CNavigationProjectController::SetAttrPanel(CNavigationAttrPanel *attrPanel){
	m_pNavigationAttrPanel = attrPanel;
}

void CNavigationProjectController::SetNewFromPosition(double dXPos, double dYPos){
	double x, y, z;
	Get3DCoordFrom2DCoords(dXPos, dYPos, x, y, z);
	m_dFromX = x;
	m_dFromY = y;
	m_dFromZ = z;
	SetShowFromAtDistance();
	SetShowFromAt3DView();
}

void CNavigationProjectController::SetNewAtPosition(double dXPos, double dYPos){
	double x, y, z;
	Get3DCoordFrom2DCoords(dXPos, dYPos, x, y, z);
	m_dAtX = x;
	m_dAtY = y;
	m_dAtZ = m_dFromZ; //we always look horizontal
	SetShowFromAtDistance();
	SetShowFromAt3DView();
}

void CNavigationProjectController::SetShowFromAt3DView(){
	double dFromZElevated = m_dFromZ + m_dHeightAboutGround;
	double dAtZElevated = m_dAtZ + m_dHeightAboutGround;
	vrOSGWidget *gl = vrOSGWidget::getMGLW(-1);
	if (gl != NULL){
		gl->setFromAtUp(-1, (float) m_dFromX, (float) m_dFromY, (float) dFromZElevated,
			(float) m_dAtX, (float) m_dAtY, (float) dAtZElevated,
			0.0, 0.0, 1.0);
	}
}

void CNavigationProjectController::SetShowFromAtDistance(){
	if (m_pNavigationAttrPanel){
		double dx = m_dFromX - m_dAtX;
		double dy = m_dFromY - m_dAtY;
		double distance = sqrt(dx * dx + dy * dy);
		char distanceText[255];
		sprintf(distanceText, "%lf", distance);
		m_pNavigationAttrPanel->SetDistanceText(distanceText);
	}
}

void CNavigationProjectController::SetHeightAboutGround(double height){
	m_dHeightAboutGround = height;
	SetShowFromAt3DView();
}

void CNavigationProjectController::LoadArealImage(const char *filename){
	if (m_pNavigationDiagramView){
		m_pNavigationDiagramView->LoadBackgroundImageAndResize(filename);
		m_pNavigationDiagramView->GetCanvasSize(m_dCanvasXSize, m_dCanvasYSize);
	}
}

void CNavigationProjectController::LoadTerrainModel(const char *filename){
	CTriangleElevationGrid *triangleGrid = new CTriangleElevationGrid();
	delete m_pElevationGrid;
	m_pElevationGrid = triangleGrid;
	triangleGrid->LoadFile(filename);
	triangleGrid->SortIntoGrid(20, 20);

	//NodePtr terrainRoot = triangleGrid->GetRoot();
	//NodePtr sceneRoot = vrScenegraph::getRoot();
	//beginEditCP(sceneRoot);{
	//	sceneRoot->addChild(terrainRoot);
	//};endEditCP(sceneRoot);
	//vrScenegraph::update();

	double xMin, xMax, yMin, yMax;
	triangleGrid->GetXYExtension(xMin, yMin, xMax, yMax);
	m_dDiagramTo3DXTranslate = xMin;
	m_dDiagramTo3DYTranslate = yMin;
}

void CNavigationProjectController::ScaleUp(){
	if (m_pNavigationDiagramView){
		m_pNavigationDiagramView->ScaleUp();
	}
}

void CNavigationProjectController::ScaleDown(){
	if (m_pNavigationDiagramView){
		m_pNavigationDiagramView->ScaleDown();
	}
}

void CNavigationProjectController::Get3DCoordFrom2DCoords(double inX, double inY, double &outX, double &outY, double &outZ){
	double x, y, z;
	x = inX;
	y = m_dCanvasYSize - inY;
	x += m_dDiagramTo3DXTranslate;
	y += m_dDiagramTo3DYTranslate;
	z = m_pElevationGrid->GetZ(x, y);
	outX = x;
	outY = y;
	outZ = z;
}