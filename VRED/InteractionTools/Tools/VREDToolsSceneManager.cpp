#include "VREDToolsSceneManager.h"

#include "vrOSGWidget.h"
#include "vrScenegraph.h"

CVREDToolsSceneManager::CVREDToolsSceneManager(void){
}

CVREDToolsSceneManager::~CVREDToolsSceneManager(void){
}

NodePtr CVREDToolsSceneManager::GetRoot(){
	vrOSGWidget *gl = vrOSGWidget::getMGLW(-1);
	if (gl != NULL){
		return gl->getRoot();
	} else {
		return NullFC;
	}
}

PerspectiveCameraPtr CVREDToolsSceneManager::GetPerspectiveCamera(){
	vrOSGWidget *gl = vrOSGWidget::getMGLW(-1);
	if (gl != NULL){
		return gl->getCamera();
	} else {
		return NullFC;
	}
}

WindowPtr CVREDToolsSceneManager::GetWindow(){
	vrOSGWidget *gl = vrOSGWidget::getMGLW(-1);
	if (gl != NULL){
		return gl->getWindow();
	} else {
		return NullFC;
	}
}

Pnt3f CVREDToolsSceneManager::GetFrom(){
	vrOSGWidget *gl = vrOSGWidget::getMGLW(-1);
	if (gl != NULL){
		return gl->getFrom(vrOSGWidget::getMGLW(gl));
	} else {
		return Pnt3f(0.0, 0.0, 0.0);
	}
}

Pnt3f CVREDToolsSceneManager::GetAt(){
	vrOSGWidget *gl = vrOSGWidget::getMGLW(-1);
	if (gl != NULL){
		return gl->getAt(vrOSGWidget::getMGLW(gl));
	} else {
		return Pnt3f(0.0, 0.0, 0.0);
	}
}

Vec3f CVREDToolsSceneManager::GetUp(){
	vrOSGWidget *gl = vrOSGWidget::getMGLW(-1);
	if (gl != NULL){
		return gl->getUp(vrOSGWidget::getMGLW(gl));
	} else {
		return Vec3f(0.0, 0.0, 0.0);
	}
}

Matrix CVREDToolsSceneManager::GetNavigatorTransformation(){
	vrOSGWidget *gl = vrOSGWidget::getMGLW(-1);
	if (gl != NULL){
		return gl->getNavigator().getMatrix();
	} else {
		return Matrix();
	}
}

void CVREDToolsSceneManager::SetFromAtUp(Pnt3f from, Pnt3f at, Vec3f up){
	vrOSGWidget *gl = vrOSGWidget::getMGLW(-1);
	if (gl != NULL){
		gl->setFromAtUp(vrOSGWidget::getMGLW(gl), from, at, up);
	}

	if (m_pNavigation2DObserver != NULL){
		m_pNavigation2DObserver->SetFromAtWithZUp(from[0], from[1], at[0], at[1], from[2]);
	}
}

bool CVREDToolsSceneManager::IsInTrackballMode(){
	vrOSGWidget *gl = vrOSGWidget::getMGLW(-1);
	if (gl != NULL){
		return (gl->getNavigator().getMode() == vrNavigator::TRACKBALL);
	} else {
		return false;
	}
}

void CVREDToolsSceneManager::SetToTrackballMode(){
	vrOSGWidget *gl = vrOSGWidget::getMGLW(-1);
	if (gl != NULL){
		gl->getNavigator().setMode(vrNavigator::TRACKBALL);
	}
}

bool CVREDToolsSceneManager::IsInWalkMode(){
	vrOSGWidget *gl = vrOSGWidget::getMGLW(-1);
	if (gl != NULL){
		return (gl->getNavigator().getMode() == vrNavigator::WALK);
	} else {
		return false;
	}
}

void CVREDToolsSceneManager::SetToWalkMode(){
	vrOSGWidget *gl = vrOSGWidget::getMGLW(-1);
	if (gl != NULL){
		gl->getNavigator().setMode(vrNavigator::WALK);
	}
}

void CVREDToolsSceneManager::WalkNavRotate(double deltaX, double deltaY){
	vrOSGWidget *gl = vrOSGWidget::getMGLW(-1);
	if (gl != NULL){
		Pnt3f from, at;
		Vec3f up;
		from = gl->getFrom(vrOSGWidget::getMGLW(gl));
		at = gl->getAt(vrOSGWidget::getMGLW(gl));
		up = gl->getUp(vrOSGWidget::getMGLW(gl));

		// rotate around the up vector
		Matrix final,temp;
		Quaternion q;

		q.setValueAsAxisRad(up,-deltaX);
		q.getValue(temp);

		final.setIdentity();
		final.setTranslate(from);
		final.mult(temp);

		temp.setIdentity();
		temp.setTranslate(-from[0],-from[1],-from[2]);

		final.mult(temp);
		final.multMatrixPnt(at);

		// rotate around the side vector
		Vec3f lv = at - from;
		lv.normalize();

		Vec3f sv = lv;
		sv.crossThis(up);
		sv.normalize();
		q.setValueAsAxisRad(sv,-deltaY);
		q.getValue(temp);

		final.setIdentity();
		final.setTranslate(from);
		final.mult(temp);

		temp.setIdentity();
		temp.setTranslate(-from[0],-from[1],-from[2]);
	
		final.mult(temp);
		final.multMatrixPnt(at);

		//gl->setFromAtUp(vrOSGWidget::getMGLW(gl), from, at, up);
		this->SetFromAtUp(from, at, up);
	}
}

Line CVREDToolsSceneManager::CalcViewRay(int x, int y){
	PerspectiveCameraPtr camera = this->GetPerspectiveCamera();
	WindowPtr window = this->GetWindow();

	if ((camera != NullFC) && (window != NullFC)){
		Line l;
		camera->calcViewRay( l, x, y, *window->getPort()[0]);
		return l;
	} else {
		return Line();
	}
}

void CVREDToolsSceneManager::Select(NodePtr node){
	vrOSGWidget *gl = vrOSGWidget::getMGLW(-1);
	if (gl != NULL){
		vrScenegraph::selectNode(node);
	}
}

void CVREDToolsSceneManager::UpdateScenegraph(){
	vrScenegraph::update();
}

//m_pSimpleSceneManager->mouseButtonPress(SimpleSceneManager::MouseLeft, x, y);
void CVREDToolsSceneManager::OnLeftMouseButtonDown(int x, int y){
}

//m_pSimpleSceneManager->mouseButtonPress(SimpleSceneManager::MouseMiddle, x, y);
void CVREDToolsSceneManager::OnMiddleMouseButtonDown(int x, int y){
}

//m_pSimpleSceneManager->mouseButtonPress(SimpleSceneManager::MouseRight, x, y);
void CVREDToolsSceneManager::OnRightMouseButtonDown(int x, int y){
}

//m_pSimpleSceneManager->mouseButtonRelease(SimpleSceneManager::MouseLeft, x, y);
void CVREDToolsSceneManager::OnLeftMouseButtonUp(int x, int y){
}

//m_pSimpleSceneManager->mouseButtonRelease(SimpleSceneManager::MouseMiddle, x, y);
void CVREDToolsSceneManager::OnMiddleMouseButtonUp(int x, int y){
}

//m_pSimpleSceneManager->mouseButtonRelease(SimpleSceneManager::MouseRight, x, y);
void CVREDToolsSceneManager::OnRightMouseButtonUp(int x, int y){
}

//m_pSimpleSceneManager->mouseMove(x, y);
void CVREDToolsSceneManager::OnMouseMove(int x, int y){
}

void CVREDToolsSceneManager::PrintInformation(){
	std::cout << "-------------------------------------" << std::endl;
	std::cout << "TraverseMask of the root: " << this->GetRoot()->getTravMask() << std::endl;
	std::cout << "TraverseMask of Vreds render action: " << vrOSGWidget::getMGLW(-1)->getRenderAction()->getTravMask() << std::endl;
	std::cout << "size of UInt32: " << sizeof(UInt32) << std::endl;
}