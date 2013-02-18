#pragma once
#include "mousetrackballnavigationtool.h"
#include "FlystickWalkNavigationTool.h"

//Handled attributes:
//Only the inherited ones

class CMouseTrackballFlystickWalkNavigationTool : public CMouseTrackballNavigationTool, public CFlystickWalkNavigationTool
{
public:
	CMouseTrackballFlystickWalkNavigationTool(void);
	~CMouseTrackballFlystickWalkNavigationTool(void);

	virtual std::string		GetToolName(){return std::string("CMouseTrackballFlystickWalkNavigationTool");}
	virtual std::string		GetToolDescription(){return std::string("Use the flystick or the mouse to navigat (fly/trackball)");}

	//Inherited Interface for setting variables as string
	virtual bool			SetAttr(std::string attr, std::string value);
};
