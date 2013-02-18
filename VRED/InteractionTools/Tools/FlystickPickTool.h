#pragma once
#include "Vr3DButtonToolBase.h"

#include "NodeNamesSet.h"

#include <OpenSG/OSGSimpleAttachments.h>

//Handled variables:
//MaxPickDistance double
//PickOnlyNodesInList bool
//AddNodeNameToPickableList	std::string
//RemoveNodeNameFromPickableList std::string

class CFlystickPickTool : public virtual CVr3DButtonToolBase {
protected:
	IntersectAction			*m_pIntersectAction;
	NodePtr					m_posgPickedObjectNode;
	
	bool					m_bToolActive;

	Vec3f					m_osgPickTimeWorldSpaceTrackingPos;
	double					m_dPickDistance;

	//Attributes
	double					m_dMaxPickDistance;
	bool					m_bPickOnlyNodesInList;
	CNodeNamesSet			m_PickOnlyNodeNamesSet;


public:
	CFlystickPickTool(void);
	~CFlystickPickTool(void);

	virtual std::string		GetToolName(){return std::string("CFlystickPickTool");}
	virtual std::string		GetToolDescription(){return std::string("Use the flystick to pick objects");}

	//Inherited Interface for setting variables as string
	virtual bool			SetAttr(std::string attr, std::string value);

	virtual void			OnTrackingButtonPressed(Quaternion q, Vec3f position);
	virtual void			OnTrackingButtonReleased(Quaternion q, Vec3f position);
	void					SetMaxPickDistance(double distance){m_dMaxPickDistance = distance;}

	void					SetPickingUnrestricted();
	void					SetPickingRestrictedToSet(CNodeNamesSet set);

	virtual void			SetIntersectActionTravMask(UInt32 travMask);
};
