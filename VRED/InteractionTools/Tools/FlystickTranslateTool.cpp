#include "FlystickTranslateTool.h"

CFlystickTranslateTool::CFlystickTranslateTool(void){
	m_posgPickedObjectTransform = NullFC;
	m_bToolActive = false;
	m_dMaxPickDistance = 1.0;
	m_bPickOnlyNodesInList = false;
}

CFlystickTranslateTool::~CFlystickTranslateTool(void){
}

bool CFlystickTranslateTool::SetAttr(std::string attr, std::string value){
	return CFlystickPickTool::SetAttr(attr, value);
}

void CFlystickTranslateTool::OnTrackingButtonPressed(Quaternion q, Vec3f position){
	std::cout << "CFlystickTranslateTool::ButtonPressed()" << std::endl;

	m_bToolActive = false;

	CFlystickPickTool::OnTrackingButtonPressed(q, position);

	if (m_bToolActive){
		std::cout << "getting parent transform node ..";
		NodePtr parentTransformNode = GetParentTransformNode(m_posgPickedObjectNode);
		if (parentTransformNode != NULL){
			std::cout << "... found one" << std::endl;
			m_posgPickedObjectTransform = TransformPtr::dcast(parentTransformNode->getCore());
			m_osgPickedObjectTransformToWorldMatrix = parentTransformNode->getToWorld();
			m_osgOldPickedObjectTransformMatrix = m_posgPickedObjectTransform->getMatrix();
			m_bToolActive = true;
		} else {
			std::cout << "... could not find one" << std::endl;
			m_bToolActive = false;
		}
	}else{
		std::cout << "tool is not active" << std::endl;
	}
}

void CFlystickTranslateTool::OnTrackingButtonReleased(Quaternion q, Vec3f position){
	std::cout << "CFlystickPickTool::ButtonReleased()" << std::endl;
	m_bToolActive = false;
}

void CFlystickTranslateTool::OnTrackingMove(Quaternion q, Vec3f position){
	if (!m_bToolActive) return;
	
	// the new position in tracking space should be the new position of the flystick
	// plus the properly rotated vector with the length of the pick distance
	Vec3f eNegZPickVec; 
	eNegZPickVec = Vec3f(0.0, 0.0, -1.0) * m_dPickDistance;
	Vec3f pickVec;
	q.multVec(eNegZPickVec, pickVec);
	Vec3f trackingSpacePos = position + pickVec;

	// evaluate the transformation in world space since pick event
	NodePtr cameraBeacon = m_pSceneManager->GetPerspectiveCamera()->getBeacon();
	Matrix cameraToWorld = cameraBeacon->getToWorld();
	Vec3f trackingWorldSpacePos = trackingSpacePos;
	cameraToWorld.multFullMatrixPnt(trackingWorldSpacePos);
	Vec3f worldSpaceDiffVector = trackingWorldSpacePos - m_osgPickTimeWorldSpaceTrackingPos;

	// transform the "transformation since pick event" (worldSpaceDiffVector) into object space
	Matrix worldToPickedObjectTransformMatrix;
	Vec3f pickedObjectSpaceDiffVector;
	m_osgPickedObjectTransformToWorldMatrix.inverse3(worldToPickedObjectTransformMatrix);
	worldToPickedObjectTransformMatrix.mult(worldSpaceDiffVector, pickedObjectSpaceDiffVector);
	

	// transform the object appropriately
	Matrix m = m_osgOldPickedObjectTransformMatrix;
	Vec3f oldTranslation, vecDummy;
	Quaternion qDummy1, qDummy2;
	m.getTransform(oldTranslation, qDummy1, vecDummy, qDummy2);
	Vec3f newTranslation = oldTranslation + pickedObjectSpaceDiffVector;
	m.setTranslate(newTranslation);

	beginEditCP(m_posgPickedObjectTransform, Transform::MatrixFieldMask);{
		m_posgPickedObjectTransform->setMatrix(m);
	};endEditCP(m_posgPickedObjectTransform, Transform::MatrixFieldMask);
}