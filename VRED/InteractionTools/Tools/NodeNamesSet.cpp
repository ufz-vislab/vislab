#include "NodeNamesSet.h"

CNodeNamesSet::CNodeNamesSet(void){
}

CNodeNamesSet::~CNodeNamesSet(void){
}

bool CNodeNamesSet::AddNamesOfGeometryNodes(NodePtr node){
	if (node->getCore()->getType().isDerivedFrom(Geometry::getClassType())){
		if (getName(node)){
			std::string name = getName(node);
			if (! this->insert(name).second){
				m_bFoundDuplicateNames = true;
			} else {
				std::cout << "Found named geometry node: " << name << std::endl;
			}
		}
	}

	int numChildren = node->getNChildren();
	for (int i=0; i<numChildren; i++){
		this->AddNamesOfGeometryNodes(node->getChild(i));
	}
	return true;
}

bool CNodeNamesSet::ContainsName(std::string name){
	return (this->find(name) != this->end());
}