#pragma once

#include <OpenSG/OSGTransform.h>
#include <OpenSG/OSGGroup.h>
#include <OpenSG/OSGSimpleGeometry.h>
#include <OpenSG/OSGSimpleAttachments.h>

#include <OpenSG/OSGChunkMaterial.h>
#include <OpenSG/OSGMaterialChunk.h>
#include <OpenSG/OSGSHLChunk.h>
#include <OpenSG/OSGTwoSidedLightingChunk.h>
#include <OpenSG/OSGMaterialGroup.h>
#include <OpenSG/OSGSimpleMaterial.h>

#include <vector>
#include <string>

OSG_USING_NAMESPACE;

class CCageFilter {
protected:
	static CCageFilter			*m_pInstance;
	CCageFilter();

public:
	
	NodePtr						m_posgLinesRoot;
	GeometryPtr					m_posgLinesGeometry;
	GeoPositions3fPtr			m_posgLinesVertexPos;

	NodePtr						m_posgMarkerRoot;
	NodePtr						m_posgRoot;
	NodePtr						m_posgMaterialRoot;

	ChunkMaterialPtr			m_posgChunkMaterial;
	MaterialChunkPtr			m_posgMaterialChunk;
	SHLChunkPtr					m_posgSHLChunk;
	TwoSidedLightingChunkPtr	m_posgTwoSidedLightingChunk;
	MaterialGroupPtr			m_posgMaterialGroup;

	TransformPtr				m_pposgMarkerScale[9];
	TransformPtr				m_pposgMarkerTransform[9];
	Matrix						m_pOldMarkerMatrices[9];
	Vec3f						m_pOldMarkerPositions[9];
	std::vector<std::string>	m_stdvecNames;

	Vec3f						m_vec3fMinVec;
	Vec3f						m_vec3fMaxVec;
	Matrix						m_osgFilterToGeometryMatrix;

	static CCageFilter			*Instance();
	~CCageFilter(void);

	void						SetMarkerScaling(double scale);
	void						SetAroundBox(Vec3f minvec, Vec3f maxvec);
	void						SetOldMarkerPositions();
	void						TransformMarker(std::string name, Vec3f translation);
	void						TransformClipPlanesMarker(std::string name, Vec3f translation);
	void						SetClippedOctant(std::string octantName);
	bool						IsBBoxMarker(std::string nodeName);
	bool						IsClipPlanesMarker(std::string nodeName);
	void						SetChild(NodePtr childRoot);
	NodePtr						FindGeometry(NodePtr root);

	NodePtr						GetMarkerRoot();
	NodePtr						GetRoot();

	// returns NullFC if it can not find the node or if the node has no geometry core
	NodePtr						FindNodeWithName(NodePtr node, std::string name);
	NodePtr						FindGeometryNodeWithName(NodePtr sceneRoot, std::string name);
	bool						SetCageFilterAroundNode(NodePtr sceneRoot, std::string nodeName);
	//void						SetNewChild(NodePtr newChild);
	//void						SetNewChild(std::string name);
};
