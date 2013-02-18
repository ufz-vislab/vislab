#include "FlystickCageFilterBBoxTool.h"

#include <cstdlib>

CFlystickCageFilterBBoxTool::CFlystickCageFilterBBoxTool(void){
	m_posgPickedObjectTransform = NullFC;
	m_bToolActive = false;
	m_dMaxPickDistance = 1.0;
	m_stdstrPickedObjectName = std::string("");
	m_iInteractionType = SET_BBOX;

	this->AddAttrNameAndType(std::string("MaxPickDistance"), std::string("double"));
	this->AddAttrNameAndType(std::string("InteractionType"), std::string("std::string  = BoundingBox or ClipPlanes"));
}

CFlystickCageFilterBBoxTool::~CFlystickCageFilterBBoxTool(void){
}

bool CFlystickCageFilterBBoxTool::SetAttr(std::string attr, std::string value){
	if (attr == std::string("MaxPickDistance")){
		double dval = atof(value.c_str());
		if (dval != 0.0){
			SetMaxPickDistance(dval);
		} else {
			std::cout << "CFlystickCageFilterBBoxTool::SetAttr(MaxPickDistance, " << value << ")";
			std::cout << "could not convert value!" << std::endl;			
		}
		return true;
	} else if (attr == std::string("InteractionType")){
		if (value == std::string("BoundingBox")){
			this->SetInteractionType(SET_BBOX);
		} else if (value == std::string("ClipPlanes")){
			this->SetInteractionType(SET_CLIP_PLANES);
		} else {
			std::cout << "CFlystickCageFilterBBoxTool::SetAttr(InteractionType, " << value;
			std::cout << ". Given value is not supported -> choose either BoundingBox or ClipPlanes!" << std::endl;
		}
		return true;
	} else {
		return CToolBase::SetAttr(attr, value);
	}
}

void CFlystickCageFilterBBoxTool::SetInteractionType(int type){
	m_iInteractionType = type;
}

int CFlystickCageFilterBBoxTool::GetInteractionType(){
	return m_iInteractionType;
}

void CFlystickCageFilterBBoxTool::OnTrackingButtonPressed(Quaternion q, Vec3f position){
	std::cout << "CFlystickCageFilterBBoxTool::ButtonPressed()" << std::endl;

	if (m_pSceneManager != NULL){
		NodePtr cameraBeacon = m_pSceneManager->GetPerspectiveCamera()->getBeacon();
		Matrix cameraToWorld = cameraBeacon->getToWorld();

		Matrix rayTransMatrix;
		rayTransMatrix.setIdentity();
		rayTransMatrix.setRotate(q);

		// transforming the position and direction (negative ez vector) from tracking space into
		// world space to use it for shooting a ray through the OpenSG scenery
		Vec3f ez(0.0, 0.0, -1.0);
		Vec3f direction; 
		rayTransMatrix.mult(ez, direction);
		Pnt3f pos = position;
		cameraToWorld.mult(direction);
		cameraToWorld.multFullMatrixPnt(pos);
		//m_osgPickTimeWorldSpaceTrackingPos = pos;

		NodePtr node = m_pSceneManager->GetRoot();
		if (node != NullFC){
			Line ray = Line(pos, direction);
			IntersectAction *iAct = IntersectAction::create();
			iAct->setTravMask(this->m_uint32IntersectActionTraverseMask);
			iAct->setLine(ray, m_dMaxPickDistance);
			//iAct->apply(node);
			iAct->apply(CCageFilter::Instance()->GetMarkerRoot());
			if (iAct->didHit()){
				m_dPickDistance = iAct->getHitT();
				m_osgPickTimeWorldSpaceTrackingPos = pos + direction * m_dPickDistance;
				NodePtr hitNode = iAct->getHitObject();

				if (getName(hitNode)){
					std::string nodeName = getName(hitNode);
					std::cout << "Hitted node is called: " << nodeName << std::endl;
					if (m_iInteractionType == SET_BBOX){
						if (CCageFilter::Instance()->IsBBoxMarker(nodeName)){
							m_stdstrPickedObjectName = nodeName;
							NodePtr transformNode = hitNode->getParent()->getParent();
							m_posgPickedObjectTransform = TransformPtr::dcast(transformNode->getCore());
							if (m_posgPickedObjectTransform != NullFC){
								m_osgPickedObjectTransformToWorldMatrix = transformNode->getToWorld();
								m_osgOldPickedObjectTransformMatrix = m_posgPickedObjectTransform->getMatrix();
								CCageFilter::Instance()->SetOldMarkerPositions();
								m_bToolActive = true;
							}
						}
					} else if (m_iInteractionType == SET_CLIP_PLANES){
						if (CCageFilter::Instance()->IsBBoxMarker(nodeName)){
							CCageFilter::Instance()->SetClippedOctant(nodeName);
						} else if (CCageFilter::Instance()->IsClipPlanesMarker(nodeName)){
							m_stdstrPickedObjectName = nodeName;
							NodePtr transformNode = hitNode->getParent()->getParent();
							m_posgPickedObjectTransform = TransformPtr::dcast(transformNode->getCore());
							if (m_posgPickedObjectTransform != NullFC){
								m_osgPickedObjectTransformToWorldMatrix = transformNode->getToWorld();
								m_osgOldPickedObjectTransformMatrix = m_posgPickedObjectTransform->getMatrix();
								CCageFilter::Instance()->SetOldMarkerPositions();
								m_bToolActive = true;
							}
						}
					}
				}
			}
		}
	}
}

void CFlystickCageFilterBBoxTool::OnTrackingButtonReleased(Quaternion q, Vec3f position){
	std::cout << "CFlystickCageFilterBBoxTool::ButtonReleased()" << std::endl;
	m_bToolActive = false;
}

void CFlystickCageFilterBBoxTool::OnTrackingMove(Quaternion q, Vec3f position){
	if (!m_bToolActive) return;

	//Evaluate the new position in tracking space
	Vec3f ezNegInTrackingSpace = Vec3f(0.0, 0.0, -1.0) * m_dPickDistance;
	Vec3f pickVecTrackingSpace;
	q.multVec(ezNegInTrackingSpace, pickVecTrackingSpace);
	Vec3f trackingWorldSpacePos = position + pickVecTrackingSpace;

	// evaluate the transformation in world space since pick event
	NodePtr cameraBeacon = m_pSceneManager->GetPerspectiveCamera()->getBeacon();
	Matrix cameraToWorld = cameraBeacon->getToWorld();
	cameraToWorld.multFullMatrixPnt(trackingWorldSpacePos);
	Vec3f worldSpaceDiffVector = trackingWorldSpacePos - m_osgPickTimeWorldSpaceTrackingPos;

	// transform the "transformation since pick event" (worldSpaceDiffVector) into object space
	Matrix worldToPickedObjectTransformMatrix;
	Vec3f pickedObjectSpaceDiffVector;
	m_osgPickedObjectTransformToWorldMatrix.inverse3(worldToPickedObjectTransformMatrix);
	worldToPickedObjectTransformMatrix.mult(worldSpaceDiffVector, pickedObjectSpaceDiffVector);

	// transform the object appropriately
	if (m_iInteractionType == SET_BBOX){
		CCageFilter::Instance()->TransformMarker(m_stdstrPickedObjectName, pickedObjectSpaceDiffVector);
	} else if (m_iInteractionType == SET_CLIP_PLANES){
		CCageFilter::Instance()->TransformClipPlanesMarker(m_stdstrPickedObjectName, pickedObjectSpaceDiffVector);
	}
}
