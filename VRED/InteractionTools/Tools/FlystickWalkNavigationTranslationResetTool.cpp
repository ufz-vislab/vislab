#include "FlystickWalkNavigationTranslationResetTool.h"

CFlystickWalkNavigationTranslationResetTool::CFlystickWalkNavigationTranslationResetTool(void):
	CFlystickWalkNavigationTool(), CFlystickTranslationResetTool(){
}

CFlystickWalkNavigationTranslationResetTool::~CFlystickWalkNavigationTranslationResetTool(void)
{
}

bool CFlystickWalkNavigationTranslationResetTool::SetAttr(std::string attr, std::string value){
	if (CFlystickWalkNavigationTool::SetAttr(attr, value)){
		return true;
	} else {
		return CFlystickTranslationResetTool::SetAttr(attr, value);
	}
}