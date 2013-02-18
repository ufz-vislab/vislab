#pragma once
//#include "flystickpicktool.h"
#include "FlystickTranslateTool.h"

//Handled attributes:
//All attributes are inherited from CFlystickPickTool

//class CFlystickTranslationResetTool : public virtual CFlystickPickTool
class CFlystickTranslationResetTool : public virtual CFlystickTranslateTool {
public:
	CFlystickTranslationResetTool(void);
	~CFlystickTranslationResetTool(void);

	virtual std::string		GetToolName(){return std::string("CFlystickTranslationResetTool");}
	virtual std::string		GetToolDescription(){return std::string("Use the flystick to pick objects and set the translation of the parenting transform to zero");}

	//Inherited Interface for setting variables as string
	virtual bool			SetAttr(std::string attr, std::string value);

	virtual void			OnTrackingButtonReleased(Quaternion q, Vec3f position);
};
