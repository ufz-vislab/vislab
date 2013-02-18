#include "FlystickWalkNavigationTool.h"

#include <cstdlib>

CFlystickWalkNavigationTool::CFlystickWalkNavigationTool(void){
	m_clocktLastTimeAnalog0 = clock();
	m_clocktLastTimeAnalog1 = clock();
	m_dFlySpeed = 1.0;
	m_dRotationSpeed = 1.0;

	this->AddAttrNameAndType(std::string("FlySpeed"), std::string("double"));
	this->AddAttrNameAndType(std::string("RotationSpeed"), std::string("double"));
}

CFlystickWalkNavigationTool::~CFlystickWalkNavigationTool(void)
{
}

bool CFlystickWalkNavigationTool::SetAttr(std::string attr, std::string value){
	if (attr == std::string("FlySpeed")){
		double dval = atof(value.c_str());
		if (dval != 0.0){
			m_dFlySpeed = dval;
		} else {
			std::cout << "CFlystickWalkNavigationTool::SetAttr(FlySpeed, " << value;
			std::cout << "), could not convert parameter to double!" << std::endl;
		}
		return true;
	} else if (attr == std::string("RotationSpeed")){
		double dval = atof(value.c_str());
		if (dval != 0.0){
			m_dRotationSpeed = dval;
		} else {
			std::cout << "CFlystickWalkNavigationTool::SetAttr(RotationSpeed, " << value;
			std::cout << "), could not convert parameter to double!" << std::endl;
		}
		return true;
	} else {
		return CToolBase::SetAttr(attr, value);
	}
}

void CFlystickWalkNavigationTool::OnAnalog0(Quaternion q, Vec3f position, double analogValue){
	if (m_pSceneManager != NULL){
		if ((analogValue < 0.001) && (analogValue > -0.001)){
			m_clocktLastTimeAnalog0 = clock();
			return;
		}

		std::cout << "OnAnalog0: " << analogValue << std::endl;

		//Navigator *navigator = m_pSceneManager->GetNavigator();

		// Ensuring we are in WALK modus
		UseWalkNavigatorMode();
		//if (navigator->getMode() != Navigator::WALK){
		//	Pnt3f fromPnt = navigator->getFrom();
		//	Pnt3f atPnt = navigator->getAt();
		//	Vec3f upVec = navigator->getUp();
		//	navigator->setMode(Navigator::WALK);
		//	navigator->set(fromPnt, atPnt, upVec);
		//}

		// calculating the direction in which we want to fly
		// WalkNavigator *walkNav = navigator->getWalkNavigator();
		clock_t currentTime = clock();
		double difftime = double(currentTime - m_clocktLastTimeAnalog0) / CLOCKS_PER_SEC;
		double rotation = difftime * m_dRotationSpeed * analogValue;
		std::cout << "rotation angle: " << rotation << std::endl; 
		//walkNav->rotate(rotation, 0.0);
		m_pSceneManager->WalkNavRotate(rotation, 0.0);
		
		m_clocktLastTimeAnalog0 = currentTime;
	}
}

void CFlystickWalkNavigationTool::OnAnalog1(Quaternion q, Vec3f position, double analogValue){
	if (m_pSceneManager != NULL){
		if ((analogValue < 0.001) && (analogValue > -0.001)){
			m_clocktLastTimeAnalog1 = clock();
			return;
		}

		std::cout << "OnAnalog1: " << analogValue << std::endl;

		// Navigator *navigator = m_pSceneManager->GetNavigator();

		// Ensuring we are in WALK modus
		UseWalkNavigatorMode();
		//if (navigator->getMode() != Navigator::WALK){
		//	Pnt3f fromPnt = navigator->getFrom();
		//	Pnt3f atPnt = navigator->getAt();
		//	Vec3f upVec = navigator->getUp();
		//	navigator->setMode(Navigator::WALK);
		//	navigator->set(fromPnt, atPnt, upVec);
		//}

		// calculating the direction in which we want to fly
		Matrix m;
		m.setIdentity();
		m.setRotate(q);
		Vec3f ez(0.0, 0.0, -1.0);
		m.mult(ez);
		clock_t currentTime = clock();
		double difftime = double(currentTime - m_clocktLastTimeAnalog1) / CLOCKS_PER_SEC;
		Vec3f displacement = ez * (m_dFlySpeed * difftime * analogValue);

		// Matrix cameraTrans = navigator->getMatrix();
		Matrix cameraTrans = m_pSceneManager->GetNavigatorTransformation();
		cameraTrans.setTranslate(0.0, 0.0, 0.0);
		cameraTrans.mult(displacement);

		//Pnt3f from = navigator->getFrom();
		//Pnt3f at = navigator->getAt();
		//Vec3f up = navigator->getUp();

		Pnt3f from = m_pSceneManager->GetFrom();
		Pnt3f at = m_pSceneManager->GetAt();
		Vec3f up = m_pSceneManager->GetUp();

		from = from + displacement;
		at = at + displacement;
		//navigator->set(from, at, up);
		m_pSceneManager->SetFromAtUp(from, at, up);

		m_clocktLastTimeAnalog1 = currentTime;
	}
}
