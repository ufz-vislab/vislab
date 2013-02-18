#pragma once
#include "toolbase.h"

class CVr3DAnalogToolBase : public virtual CToolBase
{
public:
	CVr3DAnalogToolBase(void);
	~CVr3DAnalogToolBase(void);

	virtual void			OnAnalog0(Quaternion q, Vec3f position, double analogValue);
	virtual void			OnAnalog1(Quaternion q, Vec3f position, double analogValue);
};
