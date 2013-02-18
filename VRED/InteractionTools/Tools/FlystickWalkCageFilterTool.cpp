#include "FlystickWalkCageFilterTool.h"

CFlystickWalkCageFilterTool::CFlystickWalkCageFilterTool(void):
	CFlystickWalkNavigationTool(), CFlystickCageFilterBBoxTool(){
}

CFlystickWalkCageFilterTool::~CFlystickWalkCageFilterTool(void)
{
}

bool CFlystickWalkCageFilterTool::SetAttr(std::string attr, std::string value){
	if (CFlystickWalkNavigationTool::SetAttr(attr, value)){
		return true;
	} else {
		return CFlystickCageFilterBBoxTool::SetAttr(attr, value);
	}
}