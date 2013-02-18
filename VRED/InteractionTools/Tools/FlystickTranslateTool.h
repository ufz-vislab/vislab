#pragma once
#include "FlystickPickTool.h"
#include "NodeNamesSet.h"

#include <OpenSG/OSGSimpleAttachments.h>
#include <OpenSG/OSGTransform.h>

//Handled attributes:
//All attributes are inherited from CFlystickPickTool

class CFlystickTranslateTool : public virtual CFlystickPickTool
{
public:
	CFlystickTranslateTool(void);
	~CFlystickTranslateTool(void);

	virtual std::string		GetToolName(){return std::string("CFlystickTranslateTool");}
	virtual std::string		GetToolDescription(){return std::string("Use the flystick to translate objects with parenting transform");}
	
	//Inherited Interface for setting variables as string
	virtual bool			SetAttr(std::string attr, std::string value);

	virtual void			OnTrackingButtonPressed(Quaternion q, Vec3f position);
	virtual void			OnTrackingButtonReleased(Quaternion q, Vec3f position);
	virtual void			OnTrackingMove(Quaternion q, Vec3f position);

	void					SetMaxPickDistance(double distance){m_dMaxPickDistance = distance;}

protected:
	TransformPtr			m_posgPickedObjectTransform;
	Matrix					m_osgPickedObjectTransformToWorldMatrix;
	Matrix					m_osgOldPickedObjectTransformMatrix;	

	//bool					m_bToolActive;

	double					m_dMaxPickDistance;

	//bool					m_bPickOnlyNodesInList;
	//CNodeNamesSet			m_PickOnlyNodeNamesSet;
};
