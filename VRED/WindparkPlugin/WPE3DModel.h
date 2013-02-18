#pragma once

#include <OpenSG/OSGConfig.h>
#include <OpenSG/OSGNode.h>
#include <OpenSG/OSGTransform.h>
#include <OpenSG/OSGSceneFileHandler.h>
#include <OpenSG/OSGSimpleAttachments.h>

#include "WPEState.h"

OSG_USING_NAMESPACE

class CWPE3DModel
{
private:
	NodePtr			m_posgRoot;
	
	NodePtr			m_posgBoundingBoxRoot;
	TransformPtr	m_posgBoundingBoxTransform;
	UInt32			m_uint32StandardTraverseMask;

	TransformPtr	m_posgMainTransform;
	TransformPtr	m_posgGondelTransform;
	TransformPtr	m_posgPillarTransform;
	TransformPtr	m_posgRotorTransform;
	CWPEState		m_state;

public:
	CWPE3DModel(void);
	~CWPE3DModel(void);

	bool LoadModel(const char *filename, CWPEState state);
	bool SetState(CWPEState state);

	void SetMarked(bool marked);

	bool SetGondelHeight(float height);
	bool SetRotorDiameter(float diameter);
	bool SetRotorRotation(float angle);
	bool SetTranslation(float x, float y, float z);
	bool SetRotation(float angleInDegree);
	NodePtr GetRoot(){return m_posgRoot;}
	void PrintPillarMatrix();

private:
	bool Init(CWPEState state);
	void MakeBoundingBox();
	void SizeBoundingBox();

private:
	class NodeByNameSearcher{
	private:
		std::string m_strName;
		NodePtr m_foundNode;
	public:
		NodeByNameSearcher(){m_foundNode = NullFC;};
		~NodeByNameSearcher(){};
		void			SetName(std::string name){m_strName = name;};
		NodePtr			GetNode(){return m_foundNode;};
		Action::ResultE TraverseFunction(NodePtr &node);
		NodePtr			SearchNode(NodePtr root, std::string nodeName);
	};
};
