#pragma once
#include <string>
#include <set>

#include <OpenSG/OSGNode.h>
#include <OpenSG/OSGGeometry.h>
#include <OpenSG/OSGSimpleAttachments.h>

//OSG_USING_NAMESPACE
using namespace osg;

class CNodeNamesSet : public std::set<std::string>{
public:
	CNodeNamesSet(void);
	~CNodeNamesSet(void);

	bool			AddNamesOfGeometryNodes(NodePtr node);
	bool			ContainsName(std::string name);

private:
	bool			m_bFoundDuplicateNames;
	std::string		m_stdstrErrorString;
};
