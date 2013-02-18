#include "FlystickWalkNavigationPickTool.h"

CFlystickWalkNavigationPickTool::CFlystickWalkNavigationPickTool(void):
	CFlystickWalkNavigationTool(), CFlystickPickTool(){
}

CFlystickWalkNavigationPickTool::~CFlystickWalkNavigationPickTool(void)
{
}

bool CFlystickWalkNavigationPickTool::SetAttr(std::string attr, std::string value){
	if (CFlystickWalkNavigationTool::SetAttr(attr, value)){
		return true;
	} else {
		return CFlystickPickTool::SetAttr(attr, value);
	}
}
