#pragma once
#include "flystickwalknavigationtool.h"
#include "FlystickPickTool.h"

//Handled attributes:
//All attributes are inherited!

class CFlystickWalkNavigationPickTool : public CFlystickWalkNavigationTool, public CFlystickPickTool
{
public:
	CFlystickWalkNavigationPickTool(void);
	~CFlystickWalkNavigationPickTool(void);

	virtual std::string		GetToolName(){return std::string("CFlystickWalkNavigationPickTool");}
	virtual std::string		GetToolDescription(){return std::string("Use the flystick to navigate (walk) and pick objects");}

	//Inherited Interface for setting variables as string
	virtual bool			SetAttr(std::string attr, std::string value);
};
