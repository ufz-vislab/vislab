#include ".\mousenavigationtool.h"

CMouseNavigationTool::CMouseNavigationTool(void){
}

CMouseNavigationTool::~CMouseNavigationTool(void){
}

bool CMouseNavigationTool::SetAttr(std::string attr, std::string value){
	return CToolBase::SetAttr(attr, value);
}

void CMouseNavigationTool::OnLeftMouseButtonDown(int x, int y){
	UseTrackballNavigatorMode();
	if (m_pSceneManager != NULL) m_pSceneManager->OnLeftMouseButtonDown(x, y);
};

void CMouseNavigationTool::OnMiddleMouseButtonDown(int x, int y){
	UseTrackballNavigatorMode();
	if (m_pSceneManager != NULL) m_pSceneManager->OnMiddleMouseButtonDown(x, y);
};

void CMouseNavigationTool::OnRightMouseButtonDown(int x, int y){
	UseTrackballNavigatorMode();
	if (m_pSceneManager != NULL) m_pSceneManager->OnRightMouseButtonDown(x, y);
};

void CMouseNavigationTool::OnLeftMouseButtonUp(int x, int y){
	UseTrackballNavigatorMode();
	if (m_pSceneManager != NULL) m_pSceneManager->OnLeftMouseButtonUp(x, y);
};

void CMouseNavigationTool::OnMiddleMouseButtonUp(int x, int y){
	UseTrackballNavigatorMode();
	if (m_pSceneManager != NULL) m_pSceneManager->OnMiddleMouseButtonUp(x, y);
};

void CMouseNavigationTool::OnRightMouseButtonUp(int x, int y){
	UseTrackballNavigatorMode();
	if (m_pSceneManager != NULL) m_pSceneManager->OnRightMouseButtonUp(x, y);
};

void CMouseNavigationTool::OnMouseMove(int x, int y){
	UseTrackballNavigatorMode();
	if (m_pSceneManager != NULL) m_pSceneManager->OnMouseMove(x, y);
};
