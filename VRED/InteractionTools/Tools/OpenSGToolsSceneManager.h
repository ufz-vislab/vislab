#pragma once

#include <OpenSG/OSGNavigator.h>
#include <OpenSG/OSGPerspectiveCamera.h>
#include <OpenSG/OSGWindow.h>

#include "ARTNavWindowInterface.h"

//OSG_USING_NAMESPACE

using namespace osg;

class COpenSGToolsSceneManager
{
protected:
	CARTNavWindowInterface *m_pNavigation2DObserver;
public:
	COpenSGToolsSceneManager(void);
	~COpenSGToolsSceneManager(void);

	virtual void SetNavigation2DObserver(CARTNavWindowInterface *obs){
		m_pNavigation2DObserver = obs;
	}

	virtual NodePtr					GetRoot() = 0;

	virtual PerspectiveCameraPtr	GetPerspectiveCamera() = 0;
	virtual WindowPtr				GetWindow() = 0;

	//This was not available from VRED, so .... see below:
	//virtual Navigator*				GetNavigator() = 0;
	virtual Pnt3f					GetFrom() = 0;
	virtual Pnt3f					GetAt() = 0;
	virtual Vec3f					GetUp() = 0;
	virtual Matrix					GetNavigatorTransformation() = 0;
	virtual void					SetFromAtUp(Pnt3f from, Pnt3f at, Vec3f up) = 0;
	virtual bool					IsInTrackballMode() = 0;
	virtual void					SetToTrackballMode() = 0;
	virtual bool					IsInWalkMode() = 0;
	virtual void					SetToWalkMode() = 0;
	virtual void					WalkNavRotate(double deltax, double deltay) = 0;

	virtual Line					CalcViewRay(int x, int y) = 0;

	virtual void					Select(NodePtr node) = 0;

	//m_pSimpleSceneManager->mouseButtonPress(SimpleSceneManager::MouseLeft, x, y);
	virtual void OnLeftMouseButtonDown(int x, int y) = 0;

	//m_pSimpleSceneManager->mouseButtonPress(SimpleSceneManager::MouseMiddle, x, y);
	virtual void OnMiddleMouseButtonDown(int x, int y) = 0;

	//m_pSimpleSceneManager->mouseButtonPress(SimpleSceneManager::MouseRight, x, y);
	virtual void OnRightMouseButtonDown(int x, int y) = 0;

	//m_pSimpleSceneManager->mouseButtonRelease(SimpleSceneManager::MouseLeft, x, y);
	virtual void OnLeftMouseButtonUp(int x, int y) = 0;

	//m_pSimpleSceneManager->mouseButtonRelease(SimpleSceneManager::MouseMiddle, x, y);
	virtual void OnMiddleMouseButtonUp(int x, int y) = 0;

	//m_pSimpleSceneManager->mouseButtonRelease(SimpleSceneManager::MouseRight, x, y);
	virtual void OnRightMouseButtonUp(int x, int y) = 0;

	//m_pSimpleSceneManager->mouseMove(x, y);
	virtual void OnMouseMove(int x, int y) = 0;
};
