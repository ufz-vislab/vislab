#pragma once

#include "OpenSGToolsSceneManager.h"

class CVREDToolsSceneManager : public COpenSGToolsSceneManager {
public:
	CVREDToolsSceneManager(void);
	~CVREDToolsSceneManager(void);

	virtual NodePtr					GetRoot();

	virtual PerspectiveCameraPtr	GetPerspectiveCamera();
	virtual WindowPtr				GetWindow();

	virtual Pnt3f					GetFrom();
	virtual Pnt3f					GetAt();
	virtual Vec3f					GetUp();
	virtual Matrix					GetNavigatorTransformation();
	virtual void					SetFromAtUp(Pnt3f from, Pnt3f at, Vec3f up);
	virtual bool					IsInTrackballMode();
	virtual void					SetToTrackballMode();
	virtual bool					IsInWalkMode();
	virtual void					SetToWalkMode();
	virtual void					WalkNavRotate(double deltaX, double deltaY);

	virtual Line					CalcViewRay(int x, int y);

	virtual void					Select(NodePtr node);

	virtual void					UpdateScenegraph();

	//m_pSimpleSceneManager->mouseButtonPress(SimpleSceneManager::MouseLeft, x, y);
	virtual void OnLeftMouseButtonDown(int x, int y);

	//m_pSimpleSceneManager->mouseButtonPress(SimpleSceneManager::MouseMiddle, x, y);
	virtual void OnMiddleMouseButtonDown(int x, int y);

	//m_pSimpleSceneManager->mouseButtonPress(SimpleSceneManager::MouseRight, x, y);
	virtual void OnRightMouseButtonDown(int x, int y);

	//m_pSimpleSceneManager->mouseButtonRelease(SimpleSceneManager::MouseLeft, x, y);
	virtual void OnLeftMouseButtonUp(int x, int y);

	//m_pSimpleSceneManager->mouseButtonRelease(SimpleSceneManager::MouseMiddle, x, y);
	virtual void OnMiddleMouseButtonUp(int x, int y);

	//m_pSimpleSceneManager->mouseButtonRelease(SimpleSceneManager::MouseRight, x, y);
	virtual void OnRightMouseButtonUp(int x, int y);

	//m_pSimpleSceneManager->mouseMove(x, y);
	virtual void OnMouseMove(int x, int y);

	//Print some VRED related information
	void PrintInformation();
};
