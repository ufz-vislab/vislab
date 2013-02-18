#include "MouseTrackballFlystickWalkNavigationTool.h"

CMouseTrackballFlystickWalkNavigationTool::CMouseTrackballFlystickWalkNavigationTool(void) :
	CMouseTrackballNavigationTool(), CFlystickWalkNavigationTool() {
}

CMouseTrackballFlystickWalkNavigationTool::~CMouseTrackballFlystickWalkNavigationTool(void){
}

bool CMouseTrackballFlystickWalkNavigationTool::SetAttr(std::string attr, std::string value){
	if (CMouseTrackballNavigationTool::SetAttr(attr, value)){
		return true;
	} else {
		return CFlystickWalkNavigationTool::SetAttr(attr, value);
	}
}