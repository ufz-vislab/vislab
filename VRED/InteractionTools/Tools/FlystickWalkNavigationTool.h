#pragma once
#include "Vr3DAnalogToolBase.h"
#include <time.h>

//Handled attributes:
//FlySpeed double
//RotationSpeed double

class CFlystickWalkNavigationTool : public virtual CVr3DAnalogToolBase
{
public:
	CFlystickWalkNavigationTool(void);
	~CFlystickWalkNavigationTool(void);

	virtual std::string		GetToolName(){return std::string("CFlystickWalkNavigationTool");}
	virtual std::string		GetToolDescription(){return std::string("Use the flystick to navigate (uses OpenSG WalkNavigator)");}

	//Inherited Interface for setting variables as string
	virtual bool			SetAttr(std::string attr, std::string value);

	virtual void	OnAnalog0(Quaternion q, Vec3f position, double analogValue);
	virtual void	OnAnalog1(Quaternion q, Vec3f position, double analogValue);

protected:
	clock_t			m_clocktLastTimeAnalog0;
	clock_t			m_clocktLastTimeAnalog1;
	double			m_dFlySpeed;
	double			m_dRotationSpeed;
};
