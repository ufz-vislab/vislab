#include "FlystickWalkNavigationTranslateTool.h"

CFlystickWalkNavigationTranslateTool::CFlystickWalkNavigationTranslateTool(void):
	CFlystickWalkNavigationTool(), CFlystickTranslateTool(){
}

CFlystickWalkNavigationTranslateTool::~CFlystickWalkNavigationTranslateTool(void){
}

//Inherited Interface for setting variables as string
bool CFlystickWalkNavigationTranslateTool::SetAttr(std::string attr, std::string value){
	if (CFlystickWalkNavigationTool::SetAttr(attr, value)){
		return true;
	} else {
		return CFlystickTranslateTool::SetAttr(attr, value);
	}
}