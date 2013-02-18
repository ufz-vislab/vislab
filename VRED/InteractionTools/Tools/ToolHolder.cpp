#include ".\toolholder.h"

CToolHolder* CToolHolder::m_pInstance = NULL;

OSG_USING_NAMESPACE;

CToolHolder::CToolHolder(void){
	m_pCurrentTool = NULL;
	m_pCurrentMouseTool = NULL;
	m_pCurrentVr3DButtonTool = NULL;
	m_pCurrentVr3DAnalogTool = NULL;
	m_pSceneManager = NULL;

	m_posgCursorNode = makeCoredNode<Transform>(&m_posgCursorTransform);

	//m_uint32IntersectActionTraverseMask = 4294967295;
	//m_uint32CursorTraverseMask = 4294967295;

	m_uint32IntersectActionTraverseMask = 1;
	m_uint32CursorTraverseMask = 2;
}

CToolHolder* CToolHolder::Instance(){
	if (m_pInstance == NULL){
		m_pInstance = new CToolHolder();
		return m_pInstance;
	} else {
		return m_pInstance;
	}
}

CToolHolder::~CToolHolder(void){
}

void CToolHolder::SetSceneManager(COpenSGToolsSceneManager *mgr){
	m_pSceneManager = mgr;

	PerspectiveCameraPtr camera = m_pSceneManager->GetPerspectiveCamera();
	//if (camera == NullFC) return;
	assert(camera != NullFC);

	NodePtr beacon = camera->getBeacon();
	//if (beacon == NullFC) return;
	assert(beacon != NullFC);

	beginEditCP(beacon, Node::ChildrenFieldMask);{
		beacon->addChild(m_posgCursorNode);
	};endEditCP(beacon, Node::ChildrenFieldMask);

	if (m_pCurrentTool != NULL) this->SetTool(m_pCurrentTool);

	int travMask = m_pSceneManager->GetRoot()->getTravMask();
	std::cout << "TravMask: " << travMask << std::endl;
}

void CToolHolder::SetTool(CToolBase *tool){
	if (tool != NULL){
		std::cout << "CToolHolder::SetTool() called" << std::endl;
		std::cout << "  name: " << tool->GetToolName() << std::endl;
		m_pCurrentTool = tool;
		m_pCurrentMouseTool = dynamic_cast<CMouseToolBase*>(tool);
		m_pCurrentVr3DButtonTool = dynamic_cast<CVr3DButtonToolBase*>(tool);
		m_pCurrentVr3DAnalogTool = dynamic_cast<CVr3DAnalogToolBase*>(tool);
		m_pCurrentTool->SetSceneManager(m_pSceneManager);
		NodePtr cursor = m_pCurrentTool->Get3DCursor();
		if (cursor != NullFC){
			std::cout << "  tool has a cursor" << std::endl;
			beginEditCP(m_posgCursorNode, Node::ChildrenFieldMask);{
				m_posgCursorNode->getMFChildren()->clear();
				m_posgCursorNode->addChild(cursor);
			};endEditCP(m_posgCursorNode, Node::ChildrenFieldMask);
		} else {
			std::cout << "  tool has no cursor" << std::endl;
			beginEditCP(m_posgCursorNode, Node::ChildrenFieldMask);{
				m_posgCursorNode->getMFChildren()->clear();
			};endEditCP(m_posgCursorNode, Node::ChildrenFieldMask);
		}
	}
}

void CToolHolder::SetIntersectActionTravMask(UInt32 travMask){
	this->m_uint32IntersectActionTraverseMask = travMask;

	std::map<std::string, CToolBase*>::iterator toolsIter;
	for (toolsIter = m_NameToToolMap.begin(); toolsIter != m_NameToToolMap.end(); toolsIter++){
		(*toolsIter).second->SetIntersectActionTravMask(travMask);
	}
}

void CToolHolder::Set3DCursorTravMask(UInt32 travMask){
	this->m_uint32CursorTraverseMask = travMask;

	beginEditCP(m_posgCursorNode, Node::TravMaskFieldMask);{
		m_posgCursorNode->setTravMask(travMask);
	};endEditCP(m_posgCursorNode, Node::TravMaskFieldMask);

	std::map<std::string, CToolBase*>::iterator toolsIter;
	for (toolsIter = m_NameToToolMap.begin(); toolsIter != m_NameToToolMap.end(); toolsIter++){
		(*toolsIter).second->Set3DCursorTravMask(travMask);
	}
}

bool CToolHolder::AddTool(std::string name, CToolBase *tool){
	if (tool == NULL) return false;

	tool->Set3DCursorTravMask(this->m_uint32CursorTraverseMask);
	tool->SetIntersectActionTravMask(this->m_uint32IntersectActionTraverseMask);

	return m_NameToToolMap.insert(std::map<std::string, CToolBase*>::value_type(name, tool)).second;
}

CToolBase* CToolHolder::RemoveTool(std::string name){
	std::map<std::string, CToolBase*>::iterator iter;
	iter = m_NameToToolMap.find(name);
	if (iter == m_NameToToolMap.end()){
		return NULL;
	} else {
		m_NameToToolMap.erase(iter);
		return iter->second;
	}
}

int CToolHolder::GetNumberOfTools(){
	return m_NameToToolMap.size();
}

CToolBase* CToolHolder::GetTool(std::string name){
	std::map<std::string, CToolBase*>::iterator iter = m_NameToToolMap.find(name);
	if (iter == m_NameToToolMap.end()){
		return NULL;
	} else {
		return iter->second;
	}
}

bool CToolHolder::UseTool(std::string name){
	std::map<std::string, CToolBase*>::iterator iter = m_NameToToolMap.find(name);
	if (iter == m_NameToToolMap.end()){
		return false;
	} else {
		SetTool(iter->second);
		return true;
	}
}

std::vector<std::string> CToolHolder::GetAllToolNames(){
	std::map<std::string, CToolBase*>::iterator iter;
	std::vector<std::string> returnvec;
	for (iter = m_NameToToolMap.begin(); iter != m_NameToToolMap.end(); iter++){
		returnvec.push_back((*iter).first);
	}
	return returnvec;
}

void CToolHolder::ClearAndDeleteTools(){
	std::vector<std::string>::iterator iter;
	std::vector<std::string> oldToolNamesVec = GetAllToolNames();
	for (iter = oldToolNamesVec.begin(); iter != oldToolNamesVec.end(); iter++){
		CToolBase *tool = RemoveTool(*iter);
		delete tool;
	}
}