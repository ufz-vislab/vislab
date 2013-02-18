#include "MouseTrackballNavigationTool.h"

CMouseTrackballNavigationTool::CMouseTrackballNavigationTool(void){
}

CMouseTrackballNavigationTool::~CMouseTrackballNavigationTool(void){
}

bool CMouseTrackballNavigationTool::SetAttr(std::string attr, std::string value){
	return CToolBase::SetAttr(attr, value);
}

void CMouseTrackballNavigationTool::OnLeftMouseButtonDown(int x, int y){
	UseTrackballNavigatorMode();
	if (m_pSceneManager != NULL) m_pSceneManager->OnLeftMouseButtonDown(x, y);
};

void CMouseTrackballNavigationTool::OnMiddleMouseButtonDown(int x, int y){
	UseTrackballNavigatorMode();
	if (m_pSceneManager != NULL) m_pSceneManager->OnMiddleMouseButtonDown(x, y);
};

void CMouseTrackballNavigationTool::OnRightMouseButtonDown(int x, int y){
	UseTrackballNavigatorMode();
	if (m_pSceneManager != NULL) m_pSceneManager->OnRightMouseButtonDown(x, y);
};

void CMouseTrackballNavigationTool::OnLeftMouseButtonUp(int x, int y){
	UseTrackballNavigatorMode();
	if (m_pSceneManager != NULL) m_pSceneManager->OnLeftMouseButtonUp(x, y);
};

void CMouseTrackballNavigationTool::OnMiddleMouseButtonUp(int x, int y){
	UseTrackballNavigatorMode();
	if (m_pSceneManager != NULL) m_pSceneManager->OnMiddleMouseButtonUp(x, y);
};

void CMouseTrackballNavigationTool::OnRightMouseButtonUp(int x, int y){
	UseTrackballNavigatorMode();
	if (m_pSceneManager != NULL) m_pSceneManager->OnRightMouseButtonUp(x, y);
};

void CMouseTrackballNavigationTool::OnMouseMove(int x, int y){
	UseTrackballNavigatorMode();
	if (m_pSceneManager != NULL) m_pSceneManager->OnMouseMove(x, y);
};
