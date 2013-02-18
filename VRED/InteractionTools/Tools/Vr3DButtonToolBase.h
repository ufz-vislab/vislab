#pragma once
#include "toolbase.h"

class CVr3DButtonToolBase : public virtual CToolBase
{
public:
	CVr3DButtonToolBase(void);
	~CVr3DButtonToolBase(void);

	virtual void			OnTrackingButtonPressed(Quaternion q, Vec3f position);
	virtual void			OnTrackingButtonReleased(Quaternion q, Vec3f position);
	virtual void			OnTrackingMove(Quaternion q, Vec3f position);
};
