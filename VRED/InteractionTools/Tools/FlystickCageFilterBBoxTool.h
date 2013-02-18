#pragma once
#include "Vr3DButtonToolBase.h"
#include "CageFilter.h"

//Handled attributes:
//MaxPickDistance double
//InteractionType std::string  = "BoundingBox", "ClipPlanes"

class CFlystickCageFilterBBoxTool : virtual public CVr3DButtonToolBase
{
public:
	CFlystickCageFilterBBoxTool(void);
	~CFlystickCageFilterBBoxTool(void);

	enum {SET_BBOX, SET_CLIP_PLANES};

	virtual std::string		GetToolName(){return std::string("CFlystickCageFilterBBoxTool");}
	virtual std::string		GetToolDescription(){return std::string("Use the flystick to translate the cornerstones of the cage");}
	
	//Inherited Interface for setting variables as string
	virtual bool			SetAttr(std::string attr, std::string value);

	void					SetInteractionType(int type);
	int						GetInteractionType();

	virtual void			OnTrackingButtonPressed(Quaternion q, Vec3f position);
	virtual void			OnTrackingButtonReleased(Quaternion q, Vec3f position);
	virtual void			OnTrackingMove(Quaternion q, Vec3f position);

	void					SetMaxPickDistance(double distance){m_dMaxPickDistance = distance;}

protected:
	TransformPtr			m_posgPickedObjectTransform;
	Matrix					m_osgPickedObjectTransformToWorldMatrix;
	Matrix					m_osgOldPickedObjectTransformMatrix;	
	Vec3f					m_osgPickTimeWorldSpaceTrackingPos;
	double					m_dPickDistance;

	std::string				m_stdstrPickedObjectName;

	bool					m_bToolActive;

	double					m_dMaxPickDistance;

	int						m_iInteractionType;
};
