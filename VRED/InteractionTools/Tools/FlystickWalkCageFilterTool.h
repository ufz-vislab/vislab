#pragma once
#include "flystickwalknavigationtool.h"
#include "FlystickCageFilterBBoxTool.h"

//Handled attributes:
//All handled attributes are inherited!

class CFlystickWalkCageFilterTool : public CFlystickWalkNavigationTool, public CFlystickCageFilterBBoxTool
{
public:
	CFlystickWalkCageFilterTool(void);
	~CFlystickWalkCageFilterTool(void);

	virtual std::string		GetToolName(){return std::string("CFlystickWalkCageFilterTool");}
	virtual std::string		GetToolDescription(){return std::string("Use the flystick to walk and to interact with the cage filter");}

	//Inherited Interface for setting variables as string
	virtual bool			SetAttr(std::string attr, std::string value);

};
