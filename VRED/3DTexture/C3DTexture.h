#pragma once

#include "DataVolume.h"

#include <OpenSG/OSGSimpleMaterial.h>
#include <OpenSG/OSGChunkMaterial.h>
#include <OpenSG/OSGGroup.h>
#include <OpenSG/OSGTransform.h>
#include <OpenSG/OSGTwoSidedLightingChunk.h>
#include <OpenSG/OSGImage.h>
#include <OpenSG/OSGMaterialGroup.h>
#include <OpenSG/OSGChunkMaterial.h>
#include <OpenSG/OSGSimpleAttachments.h>
#include <OpenSG/OSGMaterialChunk.h>
#include <OpenSG/OSGTextureChunk.h>
#include <OpenSG/OSGTexGenChunk.h>
#include <OpenSG/OSGTextureTransformChunk.h>
#include <OpenSG/OSGSHLChunk.h>
#include <OpenSG/OSGSimpleGeometry.h>
#include <OpenSG/OSGNode.h>

class C3DTexture{
private:
	CDataVolume						*m_pDataVol;

	OSG::NodePtr					m_pRoot;
	OSG::ImagePtr					m_pImage;
	OSG::MaterialGroupPtr			m_pMaterialGroup;
	OSG::ChunkMaterialPtr			m_pChunkMaterial;
	OSG::MaterialChunkPtr			m_pMaterialChunk;
	OSG::TextureChunkPtr			m_pTextureChunk;
	OSG::TexGenChunkPtr				m_pTexGenChunk;
	OSG::TextureTransformChunkPtr	m_pTextureTransformChunk;
	OSG::SHLChunkPtr				m_pSHLChunk;
	OSG::GeometryPtr				m_pGeometry;
	OSG::GeoPTypesUI8Ptr			m_pGeoPTypes;
	OSG::GeoPLengthsUI32Ptr			m_pGeoPLengths;
	OSG::GeoPositions3fPtr			m_pGeoPositions3f;

	OSG::Vec3f					m_EXVec;
	OSG::Vec3f					m_EYVec;
	OSG::Vec3f					m_EZVec;
	OSG::Pnt3f					m_Origin;

	bool						m_bShowXSlice;
	bool						m_bShowYSlice;
	bool						m_bShowZSlice;

	float						m_fXSlicePos;
	float						m_fYSlicePos;
	float						m_fZSlicePos;

	bool						m_bUseColorTable;
public:
	C3DTexture(void);
	~C3DTexture(void);

	void Init();
	void InitSlices();
	bool AddToVRED();
	bool RemoveFromVRED();
	bool LoadImage(const char* filename);
	bool LoadTransformation(const char* filename);
	void SetSlicesRel(bool bX, bool bY, bool bZ, float fX, float fY, float fZ);
	void SetUseColortable(bool bUse);
	void SetColortable(float fLowClip, float fHighClip);
	void CreateSimpleTexture();
};
