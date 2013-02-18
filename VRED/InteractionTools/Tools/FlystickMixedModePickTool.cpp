#include "FlystickMixedModePickTool.h"

#include "ART2DWindowManager.h"

CFlystickMixedModePickTool::CFlystickMixedModePickTool(void) : CFlystickWalkNavigationPickTool(){
	m_bUseMixedMode = true;
	m_b2DWindowActive = false;
	m_iCurrentScreen = 0;
	m_ScreenIntersector.ConfigForUFZDisplay();

	this->AddAttrNameAndType(std::string("Use2D3DMixedMode"), std::string("bool"));
}

CFlystickMixedModePickTool::~CFlystickMixedModePickTool(void){
}

bool CFlystickMixedModePickTool::SetAttr(std::string attr, std::string value){
	if (attr == std::string("Use2D3DMixedMode")){
		if (value == std::string("true")){
			std::cout << "Setting mixed mode var to true" << std::endl;
			m_bUseMixedMode = true;
		} else {
			std::cout << "Setting mixed mode var to false" << std::endl;
			m_bUseMixedMode = false;
		}
		return true;
	} else {
		return CFlystickWalkNavigationPickTool::SetAttr(attr, value);
	}
}

void CFlystickMixedModePickTool::OnTrackingButtonPressed(Quaternion q, Vec3f position){
	//Getting the current pointer in the tracking systems space 
	Matrix rayTransMatrix;
	rayTransMatrix.setIdentity();
	rayTransMatrix.setRotate(q);

	Vec3f ez(0.0, 0.0, -1.0);
	Vec3f direction; 
	rayTransMatrix.mult(ez, direction);
	Pnt3f pos = position;

	if (m_bUseMixedMode){
		std::cout << "Using mixed mode" << std::endl;
		bool bIfIntersection;
		int iCurrentScreen;
		bIfIntersection = m_ScreenIntersector.TestIntersectScreens(pos, direction, iCurrentScreen);
		if (bIfIntersection){
			m_iCurrentScreen = iCurrentScreen;
			std::cout << "Intersected screen is screen number: " << iCurrentScreen << std::endl;
		} else {
			std::cout << "No screen has been intersected" << std::endl;
			m_iCurrentScreen = CScreenIntersector::UFZ_NONE;
		}
		if ((m_iCurrentScreen == CScreenIntersector::UFZ_FRONT) || (m_iCurrentScreen == CScreenIntersector::UFZ_FLOOR)){
			std::cout << "OnTrackingButtonPressed() called for front screen" << std::endl;
			CFlystickWalkNavigationPickTool::OnTrackingButtonPressed(q, position);
			if (m_bToolActive){
				if (getName(m_posgPickedObjectNode)){
					std::string nodeName = getName(m_posgPickedObjectNode);
					std::cout << "picked object with name" << nodeName;
					CART2DWindowManager::Instance()->Show(nodeName);

				}
				m_pSceneManager->Select(m_posgPickedObjectNode);
			} else {
				m_pSceneManager->Select(NullFC);
			}
		} else if (m_iCurrentScreen == CScreenIntersector::UFZ_FLOOR){
			std::cout << "OnTrackingButtonPressed() called for floor screen" << std::endl;
			CFlystickWalkNavigationPickTool::OnTrackingButtonPressed(q, position);
			if (m_bToolActive){
				if (getName(m_posgPickedObjectNode)){
					std::string nodeName = getName(m_posgPickedObjectNode);
					std::cout << "picked object with name" << nodeName;
					CART2DWindowManager::Instance()->Show(nodeName);
				}
			}
		} else if (m_iCurrentScreen == CScreenIntersector::UFZ_LEFT){
			std::cout << "OnTrackingButtonPressed() called for left screen" << std::endl;
			m_b2DWindowActive = true;
		} else if (m_iCurrentScreen == CScreenIntersector::UFZ_RIGHT){
			std::cout << "OnTrackingButtonPressed() called for right screen" << std::endl;
			m_b2DWindowActive = true;
		} else if (m_iCurrentScreen == CScreenIntersector::UFZ_NONE){
			std::cout << "OnTrackingButtonPressed() called with no screen intersection" << std::endl;
		}
	} else {
		std::cout << "Not using mixed mode" << std::endl;
		CFlystickWalkNavigationPickTool::OnTrackingButtonPressed(q, pos);
	}
}

void CFlystickMixedModePickTool::OnTrackingButtonReleased(Quaternion q, Vec3f position){
	if (m_bUseMixedMode){
		if (m_iCurrentScreen == CScreenIntersector::UFZ_LEFT){
			std::cout << "OnTrackingButtonReleased() called for left screen" << std::endl;
		} else if (m_iCurrentScreen == CScreenIntersector::UFZ_FRONT){
			CFlystickWalkNavigationPickTool::OnTrackingButtonReleased(q, position);
		} else if (m_iCurrentScreen == CScreenIntersector::UFZ_RIGHT){
			std::cout << "OnTrackingButtonReleased() called for right screen" << std::endl;
		} else if (m_iCurrentScreen == CScreenIntersector::UFZ_FLOOR){
			CFlystickWalkNavigationPickTool::OnTrackingButtonReleased(q, position);
		} else if (m_iCurrentScreen == CScreenIntersector::UFZ_NONE){
			std::cout << "OnTrackingButtonReleased() called with no screen intersection" << std::endl;
		}
	} else {
		CFlystickPickTool::OnTrackingButtonReleased(q, position);
	}
	m_b2DWindowActive = false;
}

void CFlystickMixedModePickTool::OnTrackingMove(Quaternion q, Vec3f position){
	if (m_bUseMixedMode && m_b2DWindowActive){
		if (m_iCurrentScreen == CScreenIntersector::UFZ_LEFT){
			//std::cout << "OnTrackingMove() called for left screen" << std::endl;
		} else if (m_iCurrentScreen == CScreenIntersector::UFZ_RIGHT){
			//std::cout << "OnTrackingMove() called for right screen" << std::endl;
		}
	} else {
		CFlystickWalkNavigationPickTool::OnTrackingMove(q, position);
	}
}

void CFlystickMixedModePickTool::OnAnalog0(Quaternion q, Vec3f position, double analogValue){
	Matrix m;
	q.getValue(m);
	if (m_bUseMixedMode && m_b2DWindowActive){
		Matrix rayTransMatrix;
		rayTransMatrix.setIdentity();
		rayTransMatrix.setRotate(q);
		//Vec3f ez(0.0, 0.0, -1.0);
		Vec3f ey(0.0, 1.0, 0.0);
		Vec3f direction; 
		rayTransMatrix.mult(ey, direction);
		double zabs = direction[2];

		if (zabs < 0.0) zabs *= -1.0;

		std::cout << "zabs: " << zabs << std::endl;

		if (m_iCurrentScreen == CScreenIntersector::UFZ_LEFT){
			if (zabs < 0.75){
				int iHorizontalMove = (int) (-10.0 * analogValue);
				CART2DWindowManager::Instance()->Move(CScreenIntersector::UFZ_LEFT, iHorizontalMove, 0);
			}
		} else if (m_iCurrentScreen == CScreenIntersector::UFZ_RIGHT){
			if (zabs < 0.75){
				int iHorizontalMove = (int) (-10.0 * analogValue);
				CART2DWindowManager::Instance()->Move(CScreenIntersector::UFZ_RIGHT, iHorizontalMove, 0);
			}
		}
	} else {
		CFlystickWalkNavigationPickTool::OnAnalog0(q, position, analogValue);
	}
}

void CFlystickMixedModePickTool::OnAnalog1(Quaternion q, Vec3f position, double analogValue){
	if (m_bUseMixedMode && m_b2DWindowActive){
		Matrix rayTransMatrix;
		rayTransMatrix.setIdentity();
		rayTransMatrix.setRotate(q);
		//Vec3f ez(0.0, 0.0, -1.0);
		Vec3f ey(0.0, 1.0, 0.0);
		Vec3f direction; 
		rayTransMatrix.mult(ey, direction);
		double zabs = direction[2];

		if (zabs < 0.0) zabs *= -1.0;

		std::cout << "zabs: " << zabs << std::endl;
		if (m_iCurrentScreen == CScreenIntersector::UFZ_LEFT){
			if (zabs < 0.75){
				int iVerticalMove = (int) (10.0 * analogValue);
				CART2DWindowManager::Instance()->Move(CScreenIntersector::UFZ_LEFT, 0.0, iVerticalMove);
			} else if ((zabs >= 0.75) && (analogValue < -0.5)){
				CART2DWindowManager::Instance()->Zoom(CScreenIntersector::UFZ_LEFT, 0.95);
			} else if ((zabs >= 0.75) && (analogValue > 0.5)){
				CART2DWindowManager::Instance()->Zoom(CScreenIntersector::UFZ_LEFT, 1.05);
			}
		} else if (m_iCurrentScreen == CScreenIntersector::UFZ_RIGHT){
			if (zabs < 0.75){
				int iVerticalMove = (int) (10.0 * analogValue);
				CART2DWindowManager::Instance()->Move(CScreenIntersector::UFZ_RIGHT, 0.0, iVerticalMove);
			} else if ((zabs >= 0.75) && (analogValue < -0.5)){
				CART2DWindowManager::Instance()->Zoom(CScreenIntersector::UFZ_RIGHT, 0.95);
			} else if ((zabs >= 0.75) && (analogValue > 0.5)){
				CART2DWindowManager::Instance()->Zoom(CScreenIntersector::UFZ_RIGHT, 1.05);
			}
		}
	} else {
		CFlystickWalkNavigationPickTool::OnAnalog1(q, position, analogValue);
	}
}