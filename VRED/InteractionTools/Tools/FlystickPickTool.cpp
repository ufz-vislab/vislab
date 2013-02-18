#include "FlystickPickTool.h"

#include <OpenSG/OSGTriangleIterator.h>

#include <cstdlib>

CFlystickPickTool::CFlystickPickTool(void){
	m_pIntersectAction = IntersectAction::create();
	m_pIntersectAction->setTravMask(this->m_uint32IntersectActionTraverseMask);
	m_posgPickedObjectNode = NullFC;
	m_dMaxPickDistance = 1.0;
	m_bPickOnlyNodesInList = false;
	m_bToolActive = false;

	AddAttrNameAndType(std::string("MaxPickDistance"), std::string("double"));
	AddAttrNameAndType(std::string("PickOnlyNodesInList"), std::string("bool"));
	AddAttrNameAndType(std::string("AddNodeNameToPickableList"), std::string("std::string"));
	AddAttrNameAndType(std::string("RemoveNodeNameFromPickableList"), std::string("std::string"));
}

CFlystickPickTool::~CFlystickPickTool(void){
}

bool CFlystickPickTool::SetAttr(std::string attr, std::string value){
	if  (attr == std::string("MaxPickDistance")){
		double dval = atof(value.c_str());
		if (dval != 0.0){
			SetMaxPickDistance(dval);
		} else {
			std::cout << "CFlystickPickTool::SetAttr(MaxPickDistance, value=" << value << ") value must be of type double!" << std::endl;
		}
		return true;
	} else if (attr == std::string("PickOnlyNodesInList")){
		if (value == std::string("false")){
			SetPickingUnrestricted();
		} else {
			SetPickingRestrictedToSet(this->m_PickOnlyNodeNamesSet);
		}
		return true;
	} else if (attr == std::string("AddNodeNameToPickableList")){
		this->m_PickOnlyNodeNamesSet.insert(value);
		return true;
	} else if (attr == std::string("RemoveNodeNameFromPickableList")){
		this->m_PickOnlyNodeNamesSet.erase(value);
		return true;
	} else {
		return CToolBase::SetAttr(attr, value);
	}
}

void CFlystickPickTool::OnTrackingButtonPressed(Quaternion q, Vec3f position){
	//std::cout << "CFlystickPickTool::ButtonPressed()" << std::endl;
	//std::cout << "Intersect action trav mask: " << this->m_uint32IntersectActionTraverseMask << std::endl;
	//std::cout << "cursor trav mask: " << this->m_uint32CursorTraverseMask << std::endl;

	if (m_pSceneManager != NULL){
		NodePtr cameraBeacon = m_pSceneManager->GetPerspectiveCamera()->getBeacon();
		Matrix cameraToWorld = cameraBeacon->getToWorld();

		Matrix rayTransMatrix;
		rayTransMatrix.setIdentity();
		rayTransMatrix.setRotate(q);

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
			m_pIntersectAction->setLine(ray, m_dMaxPickDistance);
			m_pIntersectAction->apply(node);

			if (m_pIntersectAction->didHit()){

				//some test if I can obtain the colour .....
				//std::cout << "obtaining the node pointer " << std::endl;
				//NodePtr pickedNode = m_pIntersectAction->getHitObject();
				//std::cout << "obtaining the triangle number" << std::endl;
				//Int32 triangleNumber = m_pIntersectAction->getHitTriangle();
				//if (pickedNode != NullFC){
				//	std::cout << "obtaining the geometry" << std::endl;
				//	GeometryPtr geometry = GeometryPtr::dcast(pickedNode->getCore());
				//	if (geometry != NullFC){
				//		std::cout << "found a geometry to get the color" << std::endl;
				//		Color3f color = geometry->beginTriangles().getColor(triangleNumber);
				//		std::cout << "picked color: " << color[0] << "  " << color[1] << " " << color[2] << std::endl;
				//	}
				//}

				m_posgPickedObjectNode = m_pIntersectAction->getHitObject();
				if (m_bPickOnlyNodesInList){
					if (getName(m_posgPickedObjectNode)){
						// Check if the name is in the list:
						std::string nodeName = getName(m_posgPickedObjectNode);
						if (! m_PickOnlyNodeNamesSet.ContainsName(nodeName)){
							std::cout << "Node is not in pick list" << std::endl;
							return;
						}
						m_dPickDistance = m_pIntersectAction->getHitT();
						m_osgPickTimeWorldSpaceTrackingPos = pos + direction * m_dPickDistance;
						std::cout << "pick distance: " << m_dPickDistance << std::endl;
						m_bToolActive = true;
					} else {
						// There was not even a name
						return;
					}
				} else {
					m_dPickDistance = m_pIntersectAction->getHitT();
					m_osgPickTimeWorldSpaceTrackingPos = pos + direction * m_dPickDistance;
					std::cout << "pick distance: " << m_dPickDistance << std::endl;
					m_bToolActive = true;
				}
			} else {
				std::cout << "CFlystickPickTool::ButtonPressed(): could not pick anything" << std::endl;
			}
		}
	}
}

void CFlystickPickTool::OnTrackingButtonReleased(Quaternion q, Vec3f position){
	//std::cout << "CFlystickPickTool::OnTrackingButtonReleased()" << std::endl;
	m_bToolActive = false;
}

void CFlystickPickTool::SetPickingUnrestricted(){
	m_bPickOnlyNodesInList = false;
}

void CFlystickPickTool::SetPickingRestrictedToSet(CNodeNamesSet set){
	m_bPickOnlyNodesInList = true;
	m_PickOnlyNodeNamesSet = set;
}

void CFlystickPickTool::SetIntersectActionTravMask(UInt32 travMask){
	this->m_uint32IntersectActionTraverseMask = travMask;
	this->m_pIntersectAction->setTravMask(this->m_uint32IntersectActionTraverseMask);
}
