#pragma once
#include "flystickwalknavigationtool.h"
#include "FlystickTranslateTool.h"

//Handled attributes:
//All attributes are inherited

class CFlystickWalkNavigationTranslateTool : public CFlystickWalkNavigationTool, public CFlystickTranslateTool
{
public:
	CFlystickWalkNavigationTranslateTool(void);
	~CFlystickWalkNavigationTranslateTool(void);

	virtual std::string		GetToolName(){return std::string("CFlystickWalkNavigationTranslateTool");}
	virtual std::string		GetToolDescription(){return std::string("Use the flystick for navigation (walk), pick and translate objects");}

	//Inherited Interface for setting variables as string
	virtual bool			SetAttr(std::string attr, std::string value);
};
