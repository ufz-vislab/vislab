#pragma once
#include "flystickwalknavigationtool.h"
#include "FlystickTranslationResetTool.h"

//Handled attributes:
//All attributes are inherited!

class CFlystickWalkNavigationTranslationResetTool : public CFlystickWalkNavigationTool, public CFlystickTranslationResetTool
{
public:
	CFlystickWalkNavigationTranslationResetTool(void);
	~CFlystickWalkNavigationTranslationResetTool(void);

	virtual std::string		GetToolName(){return std::string("CFlystickWalkNavigationTranslationResetTool");}
	virtual std::string		GetToolDescription(){return std::string("Use the flystick to navigate (walk) and pick objects whose parenting transform is set to zero");}

	//Inherited Interface for setting variables as string
	virtual bool			SetAttr(std::string attr, std::string value);
};
