#include "MouseToolBase.h"

CMouseToolBase::CMouseToolBase(void){
}

CMouseToolBase::~CMouseToolBase(void){
}

void CMouseToolBase::OnLeftMouseButtonDown(int x, int y){
	std::cout << "CToolBase::OnLeftMouseButtonDown(): " << x << " " << y << std::endl;
};

void CMouseToolBase::OnMiddleMouseButtonDown(int x, int y){
};

void CMouseToolBase::OnRightMouseButtonDown(int x, int y){
};

void CMouseToolBase::OnLeftMouseButtonUp(int x, int y){
};

void CMouseToolBase::OnMiddleMouseButtonUp(int x, int y){
};

void CMouseToolBase::OnRightMouseButtonUp(int x, int y){
};

void CMouseToolBase::OnMouseMove(int x, int y){
};