#include ".\toolbase.h"

#include <OpenSG/OSGNavigator.h>
#include <OpenSG/OSGTransform.h>

CToolBase::CToolBase(void){
	m_pSceneManager = NULL;
	m_posg3DCursor = NullFC;

	m_uint32IntersectActionTraverseMask = 4294967295;
	m_uint32CursorTraverseMask = 4294967295;

	AddAttrNameAndType(std::string("CursorFile"),  std::string("std::string"));
}

CToolBase::~CToolBase(void){
	if (m_posg3DCursor != NullFC){
		subRefCP(m_posg3DCursor);
	}
}

std::vector<std::string> CToolBase::GetAttrNames(){
	std::vector<std::string> returnvec;
	std::map<std::string, std::string>::iterator iter;
	for (iter = m_stdmapVarsAndTypes.begin(); iter != m_stdmapVarsAndTypes.end(); iter++){
		returnvec.push_back((*iter).first);
	}
	return returnvec;
}

std::map<std::string, std::string> CToolBase::GetAttrNamesAndTypes(){
	return m_stdmapVarsAndTypes;
}

bool CToolBase::SetAttr(std::string attr, std::string value){
	if (attr == std::string("CursorFile")){
		Load3DCursor(value.c_str());
		return true;
	} else {
		return false;
	}
}

bool CToolBase::SetAttr(std::string attr, int value){
	return false;
}

bool CToolBase::SetAttr(std::string attr, double value){
	return false;
}

void CToolBase::Load3DCursor(const char* filename){
	if (m_posg3DCursor != NullFC) subRefCP(m_posg3DCursor);

	m_posg3DCursor = SceneFileHandler::the().read(filename);
	if (m_posg3DCursor != NullFC){
		addRefCP(m_posg3DCursor);
		beginEditCP(m_posg3DCursor, Node::TravMaskFieldMask);{
			m_posg3DCursor->setTravMask(m_uint32CursorTraverseMask);
		};endEditCP(m_posg3DCursor, Node::TravMaskFieldMask);
	}
}

void CToolBase::SetIntersectActionTravMask(UInt32 travMask){
	m_uint32IntersectActionTraverseMask = travMask;
}

void CToolBase::Set3DCursorTravMask(UInt32 travMask){
	m_uint32CursorTraverseMask = travMask;
	if (m_posg3DCursor != NullFC){
		beginEditCP(m_posg3DCursor, Node::TravMaskFieldMask);{
			m_posg3DCursor->setTravMask(m_uint32CursorTraverseMask);
		};endEditCP(m_posg3DCursor, Node::TravMaskFieldMask);
	}
}

void CToolBase::SetSceneManager(COpenSGToolsSceneManager *mgr){
	m_pSceneManager = mgr;
};

bool CToolBase::UseWalkNavigatorMode(){
	if (m_pSceneManager == NULL) return false;

	// Navigator *navigator = m_pSceneManager->GetNavigator();

	// Ensuring we are in WALK modus
	if (! m_pSceneManager->IsInWalkMode()){
		Pnt3f fromPnt = m_pSceneManager->GetFrom();
		Pnt3f atPnt = m_pSceneManager->GetAt();
		Vec3f upVec = m_pSceneManager->GetUp();
		m_pSceneManager->SetToWalkMode();
		m_pSceneManager->SetFromAtUp(fromPnt, atPnt, upVec);
	}

	/*
	if (navigator->getMode() != Navigator::WALK){
		Pnt3f fromPnt = navigator->getFrom();
		Pnt3f atPnt = navigator->getAt();
		Vec3f upVec = navigator->getUp();
		navigator->setMode(Navigator::WALK);
		navigator->set(fromPnt, atPnt, upVec);
	}
	*/

	return true;
}

bool CToolBase::UseTrackballNavigatorMode(){
	if (m_pSceneManager == NULL) return false;
	// Navigator *navigator = m_pSceneManager->GetNavigator();

	// Ensuring we are in TRACKBALL modus
	if (! m_pSceneManager->IsInTrackballMode()){
		Pnt3f fromPnt = m_pSceneManager->GetFrom();
		Pnt3f atPnt = m_pSceneManager->GetAt();
		Vec3f upVec = m_pSceneManager->GetUp();
		m_pSceneManager->SetToTrackballMode();
		m_pSceneManager->SetFromAtUp(fromPnt, atPnt, upVec);
	}

	/*
	if (navigator->getMode() != Navigator::TRACKBALL){
		Pnt3f fromPnt = navigator->getFrom();
		Pnt3f atPnt = navigator->getAt();
		Vec3f upVec = navigator->getUp();
		navigator->setMode(Navigator::TRACKBALL);
		navigator->set(fromPnt, atPnt, upVec);
	}
	*/
	return true;
}
                 

void CToolBase::CalcScreenProjection(Pnt3f point, Pnt2f &projPoint){
	Pnt3f newPoint;

	if (m_pSceneManager == NULL) return;
	PerspectiveCameraPtr camera = m_pSceneManager->GetPerspectiveCamera();
	ViewportPtr port = m_pSceneManager->GetWindow()->getPort(0);
	int iWidth = port->getPixelWidth();
	int iHeight = port->getPixelHeight();

	Matrix projection;
	projection.setIdentity();
	camera->getProjection(projection, iWidth, iHeight);

	Matrix projectionTranslation;
	projectionTranslation.setIdentity();
	camera->getProjectionTranslation(projectionTranslation, iWidth, iHeight);

	Matrix viewingMatrix;
	viewingMatrix.setIdentity();
	camera->getViewing(viewingMatrix, iWidth, iHeight);

	Matrix wctocc = projection;
	wctocc.mult(projectionTranslation);
	wctocc.mult(viewingMatrix);

	wctocc.multFullMatrixPnt(point, newPoint);

	projPoint[0] = 0.5 * (newPoint[0] + 1.0) * (double) iWidth;
	projPoint[1] = iHeight - 0.5 * (newPoint[1] + 1.0) * (double) iHeight;
};

Line CToolBase::CalcViewRay(int x, int y){
	return m_pSceneManager->CalcViewRay(x, y);
}

void CToolBase::GetLocalUnityVectorsScreenProj(Matrix toWorld, Pnt3f pickedVolCenter, Vec2f &screenE1, Vec2f &screenE2, Vec2f &screenE3){
	Matrix lctowc = toWorld;

	//Local Coordinates
	Pnt3f lcE1(1.0, 0.0, 0.0);
	Pnt3f lcE2(0.0, 1.0, 0.0);
	Pnt3f lcE3(0.0, 0.0, 1.0);
	Pnt3f lcCenter = pickedVolCenter;

	Pnt3f lcCenterE1 = lcCenter + lcE1;
	Pnt3f lcCenterE2 = lcCenter + lcE2;
	Pnt3f lcCenterE3 = lcCenter + lcE3;

	//In world coordinates
	Pnt3f wcCenter, wcCenterE1, wcCenterE2, wcCenterE3;
	lctowc.multFullMatrixPnt(lcCenter, wcCenter);
	lctowc.multFullMatrixPnt(lcCenterE1, wcCenterE1);
	lctowc.multFullMatrixPnt(lcCenterE2, wcCenterE2);
	lctowc.multFullMatrixPnt(lcCenterE3, wcCenterE3);

	//In screen coordinates
	Pnt2f scCenter, scCenterE1, scCenterE2, scCenterE3;
	CalcScreenProjection(wcCenter, scCenter);
	CalcScreenProjection(wcCenterE1, scCenterE1);
	CalcScreenProjection(wcCenterE2, scCenterE2);
	CalcScreenProjection(wcCenterE3, scCenterE3);

	//Unity vectors on the screen
	screenE1 = scCenterE1 - scCenter;
	screenE2 = scCenterE2 - scCenter;
	screenE3 = scCenterE3 - scCenter;
}

NodePtr	CToolBase::GetParentTransformNode(NodePtr start){
	if (start == NullFC) return NullFC;
	if (NullFC != TransformPtr::dcast(start->getCore())) return start;
	NodePtr node = start;
	do {
		node = node->getParent();
		if (node == NullFC) return NullFC;
		if (NullFC != TransformPtr::dcast(node->getCore())){
			std::cout << "found a transform node" << std::endl;
			return node;
		}
	} while (node != NullFC);
	return NullFC;
}

void CToolBase::SetNear(double near){
	if (m_pSceneManager != NULL){
		CameraPtr cam = m_pSceneManager->GetPerspectiveCamera();
		if (cam == NullFC) return;
		beginEditCP(cam, Camera::NearFieldMask);{
			cam->setNear(0.1);
		};endEditCP(cam, Camera::NearFieldMask);
	}
}

bool CToolBase::AddAttrNameAndType(std::string attr, std::string type){
	std::pair<std::map<std::string, std::string>::iterator, bool> returnval;
	returnval = m_stdmapVarsAndTypes.insert(std::map<std::string, std::string>::value_type(attr, type));
	return returnval.second;
}