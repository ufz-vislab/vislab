#pragma once
#include "Vr3DAnalogToolBase.h"
#include "time.h"

//Attributes handled:
//All attributes are inherited from CToolBase

class CFlystickNavigationTool : public CVr3DAnalogToolBase
{
protected:
	clock_t			m_clocktLastTimeAnalog0;
	clock_t			m_clocktLastTimeAnalog1;
	double			m_dFlySpeed;
	double			m_dRotationSpeed;
public:
	CFlystickNavigationTool(void);
	~CFlystickNavigationTool(void);

	//Inherited Interface for setting variables as string
	virtual bool			SetAttr(std::string attr, std::string value);

	virtual std::string		GetToolName(){return std::string("CFlystickNavigationTool");}
	virtual std::string		GetToolDescription(){return std::string("Use the flystick to navigate");}

	virtual void	OnAnalog0(Quaternion q, Vec3f position, double analogValue);
	virtual void	OnAnalog1(Quaternion q, Vec3f position, double analogValue);
};
