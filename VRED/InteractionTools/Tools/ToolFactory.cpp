#include "ToolFactory.h"

#include <cstdlib>

#include "FlystickCageFilterBBoxTool.h"
#include "FlystickMixedModePickTool.h"
#include "FlystickNavigationTool.h"
#include "FlystickPickTool.h"
#include "FlystickTranslateTool.h"
#include "FlystickTranslationResetTool.h"
#include "FlystickWalkCageFilterTool.h"
#include "FlystickWalkNavigationPickTool.h"
#include "FlystickWalkNavigationTool.h"
#include "FlystickWalkNavigationTranslateTool.h"
#include "FlystickWalkNavigationTranslationResetTool.h"
#include "MouseNavigationTool.h"
#include "MouseTrackballFlystickWalkNavigationTool.h"
#include "MouseTrackballNavigationTool.h"

CToolFactory *CToolFactory::m_pInstance = NULL;

CToolFactory *CToolFactory::Instance(){
	if (m_pInstance == NULL){
		m_pInstance = new CToolFactory();
	}
	return m_pInstance;
}

CToolFactory::CToolFactory(void){
}

CToolFactory::~CToolFactory(void){
}

CToolBase* CToolFactory::GenerateTool(std::string tooltype){
	if (tooltype == std::string("CFlystickCageFilterBBoxTool")){
		return new CFlystickCageFilterBBoxTool();
	} else if (tooltype == std::string("CFlystickMixedModePickTool")){
		return new CFlystickMixedModePickTool();
	} else if (tooltype == std::string("CFlystickNavigationTool")){
		return new CFlystickNavigationTool();
	} else if (tooltype == std::string("CFlystickPickTool")){
		return new CFlystickPickTool();
	} else if (tooltype == std::string("CFlystickTranslateTool")){
		return new CFlystickTranslateTool();
	} else if (tooltype == std::string("CFlystickTranslationResetTool")){
		return new CFlystickTranslationResetTool();
	} else if (tooltype == std::string("CFlystickWalkCageFilterTool")){
		return new CFlystickWalkCageFilterTool();
	} else if (tooltype == std::string("CFlystickWalkNavigationPickTool")){
		return new CFlystickWalkNavigationPickTool();
	} else if (tooltype == std::string("CFlystickWalkNavigationTool")){
		return new CFlystickWalkNavigationTool();
	} else if (tooltype == std::string("CFlystickWalkNavigationTranslateTool")){
		return new CFlystickWalkNavigationTranslateTool();
	} else if (tooltype == std::string("CFlystickWalkNavigationTranslationResetTool")){
		return new CFlystickWalkNavigationTranslationResetTool();
	} else if (tooltype == std::string("CMouseNavigationTool")){
		return new CMouseNavigationTool();
	} else if (tooltype == std::string("CMouseTrackballFlystickWalkNavigationTool")){
		return new CMouseTrackballFlystickWalkNavigationTool();
	} else if (tooltype == std::string("CMouseTrackballNavigationTool")){
		return new CMouseTrackballNavigationTool();
	} else {
		std::cout << "CToolFactory::GenerateTool(tooltype = " << tooltype << ")" << std::endl;
		std::cout << "				unknown tooltype !" << std::endl;
		return NULL;
	}
}