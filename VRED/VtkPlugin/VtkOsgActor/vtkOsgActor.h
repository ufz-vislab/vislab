#pragma once
#include "vtkOpenGLActor.h"

#include "vtkGraphicsFactory.h"
#include "vtkTimeStamp.h"

#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkMapper.h>
#include <vtkPolyData.h>
#include <vtkDataSet.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkCellData.h>
#include <vtkCellArray.h>

#include <OpenSG/OSGNode.h>
#include <OpenSG/OSGSimpleGeometry.h>
#include <OpenSG/OSGSimpleMaterial.h>
#include <OpenSG/OSGGeoFunctions.h>
#include <OpenSG/OSGGroup.h>
#include <OpenSG/OSGTwoSidedLightingChunk.h>
#include <OpenSG/OSGPolygonChunk.h>
#include <OpenSG/OSGGeoFunctions.h>
#include <OpenSG/OSGTransform.h>

OSG_USING_NAMESPACE

class vtkOsgActor : public vtkOpenGLActor
{
private:
	vtkDataArray			*m_pvtkNormals;
	vtkDataArray			*m_pvtkTexCoords;
	vtkUnsignedCharArray	*m_pvtkColors;

	enum {NOT_GIVEN, PER_VERTEX, PER_CELL};
	int						m_iColorType;
	int						m_iNormalType;
	bool					m_bVerbose;

	int						m_iNumPoints;
	int						m_iNumNormals;
	int						m_iNumColors;
	int						m_iNumGLPoints;
	int						m_iNumGLLineStrips;
	int						m_iNumGLPolygons;
	int						m_iNumGLTriStrips;
	int						m_iNumGLPrimitives;

	//For the translation to OpenSG
	NodePtr m_posgRoot;
	NodePtr m_posgGeomNode;
	GeometryPtr m_posgGeometry;
	SimpleMaterialPtr m_posgMaterial;
	PolygonChunkPtr m_posgPolygonChunk;

	GeoPTypesPtr m_posgTypes;
	GeoPLengthsPtr m_posgLengths;
	GeoIndicesUI32Ptr m_posgIndices;
	GeoPositions3fPtr m_posgPoints;
	GeoColors3fPtr m_posgColors;
	GeoNormals3fPtr m_posgNormals;


public:
	vtkTypeMacro(vtkOsgActor,vtkOpenGLActor);
	static vtkOsgActor* New();

	void PrintSelf(ostream& os, vtkIndent indent);
	void Render(vtkRenderer *ren, vtkMapper *mapper);

protected:
	vtkOsgActor(void);
	~vtkOsgActor(void);

private:
  vtkOsgActor(const vtkOsgActor&);  // Not implemented.
  void operator=(const vtkActor&);  // Not implemented.

private:
	//for the translation to OpenSG
	void LookForNormals();
	void LookForColors();
	void LookForArraySizes();

	SimpleMaterialPtr CreateMaterial();

	//Can use OpenSG simple indexed geometry
	NodePtr ProcessGeometryNormalsAndColorsPerVertex();

	//Can't use indexing and so requires a lot of storage space
	NodePtr ProcessGeometryNonIndexedCopyAttributes(int gl_primitive_type);

public:
	void SetVerbose(bool value){m_bVerbose = value;}
	void UpdateOsg();
	NodePtr GetOsgRoot(){return m_posgRoot;}

	NodePtr GetNodePtr();
};
