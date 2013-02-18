#pragma once
#include "FlystickWalknavigationPickTool.h"
#include "ScreenIntersector.h"

//Handled attributes
//Use2D3DMixedMode bool

class CFlystickMixedModePickTool : public virtual CFlystickWalkNavigationPickTool
{
protected:
	bool					m_bUseMixedMode;
	bool					m_b2DWindowActive;
	int						m_iCurrentScreen;
	CScreenIntersector		m_ScreenIntersector;

public:
	CFlystickMixedModePickTool(void);
	~CFlystickMixedModePickTool(void);

	virtual std::string		GetToolName(){return std::string("CFlystickMixedModePickTool");}
	virtual std::string		GetToolDescription(){return std::string("Use the flystick to pick or interact with 2D windows");}
	
	//Inherited Interface for setting variables as string
	virtual bool			SetAttr(std::string attr, std::string value);

	virtual void			OnTrackingButtonPressed(Quaternion q, Vec3f position);
	virtual void			OnTrackingButtonReleased(Quaternion q, Vec3f position);
	virtual void			OnTrackingMove(Quaternion q, Vec3f position);

	virtual void			OnAnalog0(Quaternion q, Vec3f position, double analogValue);
	virtual void			OnAnalog1(Quaternion q, Vec3f position, double analogValue);
};
