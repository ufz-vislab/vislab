#include "StdAfx.h"
#include ".\vtkosgactor.h"

#include "vtkMapper.h"
#include "vtkDataSet.h"

vtkOsgActor *vtkOsgActor::New(){
	// First try to create the object from the vtkGraphicsFactory
  vtkObject* ret = vtkGraphicsFactory::CreateInstance("vtkOsgActor");
  if (ret == NULL) {
	  ret = new vtkOsgActor();
  }
  return (vtkOsgActor*)ret;
}

void vtkOsgActor::PrintSelf(ostream& os, vtkIndent indent){
	vtkOpenGLActor::PrintSelf(os, indent);

	// beside this nothing is done so far ...
}

void vtkOsgActor::Render(vtkRenderer *ren, vtkMapper *mapper){
	vtkOpenGLActor::Render(ren, mapper);
}

void vtkOsgActor::UpdateOsg(){
	this->GetMapper()->Update();
	LookForNormals();
	LookForColors();
	LookForArraySizes();

	NodePtr newNodePtr = this->GetNodePtr();

	if (newNodePtr == NullFC) return;

	if (m_iNormalType == NOT_GIVEN){
		GeometryPtr newGeometryPtr = GeometryPtr::dcast(newNodePtr->getCore());
		if ((newGeometryPtr != NullFC) && (m_iColorType == PER_VERTEX)){
			calcVertexNormals(newGeometryPtr);
		} else if ((newGeometryPtr != NullFC) && (m_iColorType == PER_CELL)){
			calcFaceNormals(newGeometryPtr);
		} else if (m_iColorType == NOT_GIVEN){
		}
	}

	beginEditCP(m_posgRoot);{
		m_posgRoot->addChild(newNodePtr);
	};endEditCP(m_posgRoot);
}

vtkOsgActor::vtkOsgActor(void) : vtkOpenGLActor()
{
	m_posgRoot = makeCoredNode<Group>();
	m_posgGeomNode = Node::create();
	m_posgGeometry = Geometry::create();
	m_posgMaterial = SimpleMaterial::create();
	m_posgPolygonChunk = PolygonChunk::create();

	beginEditCP(m_posgRoot);{
		m_posgRoot->addChild(m_posgGeomNode);
	};endEditCP(m_posgRoot);

	beginEditCP(m_posgGeomNode);{
		m_posgGeomNode->setCore(m_posgGeometry);
	};endEditCP(m_posgGeomNode);

	m_pvtkNormals = NULL;
	m_pvtkTexCoords = NULL;
	m_pvtkColors = NULL;

	m_iColorType = NOT_GIVEN;
	m_iNormalType = NOT_GIVEN;

	m_iNumPoints = 0;
	m_iNumNormals = 0;
	m_iNumColors = 0;
	m_iNumGLPoints = 0;
	m_iNumGLLineStrips = 0;
	m_iNumGLPolygons = 0;
	m_iNumGLTriStrips = 0;
	m_iNumGLPrimitives = 0;

	m_posgTypes = GeoPTypesUI8::create();
	m_posgLengths = GeoPLengthsUI32::create();
	m_posgIndices = GeoIndicesUI32::create();
	m_posgPoints = GeoPositions3f::create();
	m_posgColors = GeoColors3f::create();
	m_posgNormals = GeoNormals3f::create();

	m_bVerbose = false;
}

vtkOsgActor::~vtkOsgActor(void)
{
	if (m_pvtkNormals != NULL) m_pvtkNormals->Delete();
	if (m_pvtkTexCoords != NULL) m_pvtkTexCoords->Delete();
	if (m_pvtkColors != NULL) m_pvtkColors->Delete();
}

void vtkOsgActor::LookForNormals(){
	vtkPolyData *pPolyData = (vtkPolyData*) this->GetMapper()->GetInput();
	m_iNormalType = NOT_GIVEN;
	if (this->GetProperty()->GetInterpolation() == VTK_FLAT){
		m_pvtkNormals = pPolyData->GetCellData()->GetNormals();
		if (m_pvtkNormals != NULL) m_iNormalType = PER_CELL;
	}else{
		m_pvtkNormals = pPolyData->GetPointData()->GetNormals();
		if (m_pvtkNormals != NULL) m_iNormalType = PER_VERTEX;
	}
	if (m_bVerbose){
		if (m_iNormalType != NOT_GIVEN){
			std::cerr << "	number of normals: " << m_pvtkNormals->GetNumberOfTuples() << std::endl;
			std::cerr << "	normals are given: ";
			std::cerr << ((m_iNormalType == PER_VERTEX) ? "per vertex" : "per cell") << std::endl;
		}else{
			std::cerr << "	no normals are given" << std::endl;
		}
	}
}

void vtkOsgActor::LookForColors(){
	vtkPolyData *pPolyData = (vtkPolyData*) this->GetMapper()->GetInput();
	m_pvtkColors = this->GetMapper()->MapScalars(1.0);
	m_iColorType = NOT_GIVEN;
	if (this->GetMapper()->GetScalarVisibility() && (m_pvtkColors != NULL)){
		int iScalarMode = this->GetMapper()->GetScalarMode();
		if (iScalarMode == VTK_SCALAR_MODE_USE_CELL_DATA || (!pPolyData->GetPointData()->GetScalars())){ //there are no point data
			m_iColorType = PER_CELL;
		}else{
			m_iColorType = PER_VERTEX;
		}
	}
	if (m_bVerbose){	
		if (m_iColorType != NOT_GIVEN){
			std::cerr << "  number of colors: " << m_pvtkColors->GetNumberOfTuples() << std::endl;
			std::cerr << "	colors are given: ";
			std::cerr << ((m_iColorType == PER_VERTEX) ? "per vertex" : "per cell") << std::endl;
		}else{
			std::cerr << "	no colors are given" << std::endl;
		}
	}
}

void vtkOsgActor::LookForArraySizes(){
	vtkPolyData *pPolyData = (vtkPolyData*) this->GetMapper()->GetInput();

	m_iNumPoints = pPolyData->GetNumberOfPoints();

	vtkCellArray *cellArray; 
		
	cellArray = pPolyData->GetVerts();
	if (cellArray != NULL){
		m_iNumGLPoints = cellArray->GetNumberOfCells();
	}

	cellArray = pPolyData->GetLines();
	if (cellArray != NULL){
		m_iNumGLLineStrips = cellArray->GetNumberOfCells();
	}

	cellArray = pPolyData->GetPolys();
	if (cellArray != NULL){
		m_iNumGLPolygons = pPolyData->GetPolys()->GetNumberOfCells();
	}

	cellArray = pPolyData->GetStrips();
	if (cellArray != NULL){
		m_iNumGLTriStrips = cellArray->GetNumberOfCells();
	}

	m_iNumGLPrimitives = m_iNumGLPoints + m_iNumGLLineStrips + m_iNumGLPolygons + m_iNumGLTriStrips; 


	if (m_bVerbose){
		std::cerr << "	number of vertices: " << m_iNumPoints << std::endl;
		std::cerr << "	number of GL_POINTS: " << m_iNumGLPoints << std::endl;
		std::cerr << "	number of GL_LINE_STRIPS: " << m_iNumGLLineStrips << std::endl;
		std::cerr << "	number of GL_POLYGON's: " << m_iNumGLPolygons << std::endl;
		std::cerr << "	number of GL_TRIANGLE_STRIPS: " << m_iNumGLTriStrips << std::endl;
		std::cerr << "	number of primitives: " << m_iNumGLPrimitives << std::endl;
	}
}

SimpleMaterialPtr vtkOsgActor::CreateMaterial(){
	vtkProperty *prop = this->GetProperty();
	double *diffuseColor = prop->GetDiffuseColor();
	double *ambientColor = prop->GetAmbientColor();
	double *specularColor = prop->GetSpecularColor();
	double specularPower = prop->GetSpecularPower();

	double diffuse = prop->GetDiffuse();
	double ambient = prop->GetAmbient();
	double specular = prop->GetSpecular();

	float opacity = prop->GetOpacity();
	int representation = prop->GetRepresentation();

	if (m_bVerbose){
		std::cerr << "Colors:" << std::endl;
		std::cerr << "diffuse " << diffuse << " * " << diffuseColor[0] << " " << diffuseColor[1] << " " << diffuseColor[2] << std::endl;
		std::cerr << "ambient " << ambient << " * " << ambientColor[0] << " " << ambientColor[1] << " " << ambientColor[2] << std::endl;
		std::cerr << "specular " << specular << " * " << specularColor[0] << " " << specularColor[1] << " " << specularColor[2] << std::endl;
	}

	beginEditCP(m_posgPolygonChunk);{
		if (representation == VTK_SURFACE){
			m_posgPolygonChunk->setFrontMode(GL_FILL);
			m_posgPolygonChunk->setBackMode(GL_FILL);
		}else if (representation == VTK_WIREFRAME){
			m_posgPolygonChunk->setFrontMode(GL_LINE);
			m_posgPolygonChunk->setBackMode(GL_LINE);
		}else{
			m_posgPolygonChunk->setFrontMode(GL_POINT);
			m_posgPolygonChunk->setBackMode(GL_POINT);
		}
	};endEditCP(m_posgPolygonChunk);

	beginEditCP(m_posgMaterial);{
		m_posgMaterial->setDiffuse(Color3f(diffuseColor[0]*diffuse, diffuseColor[1]*diffuse, diffuseColor[2]*diffuse));
		m_posgMaterial->setSpecular(Color3f(specularColor[0]*specular, specularColor[1]*specular, specularColor[2]*specular));
		m_posgMaterial->setAmbient(Color3f(ambientColor[0]*ambient, ambientColor[1]*ambient, ambientColor[2]*ambient));
		m_posgMaterial->setShininess(specularPower);
		m_posgMaterial->setTransparency(1.0f - opacity); // 1-opacity ?
		m_posgMaterial->addChunk(TwoSidedLightingChunk::create());
		m_posgMaterial->addChunk(m_posgPolygonChunk);
		m_posgMaterial->setColorMaterial(GL_AMBIENT_AND_DIFFUSE);
	}endEditCP(m_posgMaterial);
	return m_posgMaterial;
}

NodePtr vtkOsgActor::ProcessGeometryNormalsAndColorsPerVertex(){
	beginEditCP(m_posgTypes);{
		m_posgTypes->clear();
	};endEditCP(m_posgTypes);

	beginEditCP(m_posgLengths);{
		m_posgLengths->clear();
	};endEditCP(m_posgLengths);

	beginEditCP(m_posgIndices);{
		m_posgIndices->clear();
	};endEditCP(m_posgIndices);

	beginEditCP(m_posgPoints);{
		m_posgPoints->clear();
	};endEditCP(m_posgPoints);

	beginEditCP(m_posgColors);{
		m_posgColors->clear();
	};endEditCP(m_posgColors);

	beginEditCP(m_posgNormals);{
		m_posgNormals->clear();
	};endEditCP(m_posgNormals);

	int iNumPoints = 0;
	int iNumNormals = 0;
	int iNumColors = 0;
	int i, j;

	vtkPolyData *pPolyData = (vtkPolyData*) this->GetMapper()->GetInput();

	//getting the vertices:
	beginEditCP(m_posgPoints);{
		iNumPoints = pPolyData->GetNumberOfPoints();
		for (i=0; i<iNumPoints; i++){
			double *aVertex = pPolyData->GetPoint(i);
			m_posgPoints->addValue(Vec3f(aVertex[0], aVertex[1], aVertex[2]));
		}
	}endEditCP(m_posgPoints);

	//possibly getting the normals
	if (m_iNormalType == PER_VERTEX){
		iNumNormals = m_pvtkNormals->GetNumberOfTuples();
		beginEditCP(m_posgNormals);{
			double *aNormal;
			for (i=0; i<iNumNormals; i++){
				aNormal = m_pvtkNormals->GetTuple(i);
				m_posgNormals->addValue(Vec3f(aNormal[0], aNormal[1], aNormal[2]));
			}
		}endEditCP(m_posgNormals);
		if (iNumNormals != iNumPoints){
			std::cerr << "WARNING: CVtkActorToOpenSG::ProcessGeometryNormalsAndColorsPerVertex() number of normals" << std::endl;
			std::cerr << "should equal the number of vertices (points)!" << std::endl << std::endl;
		}
	}

	//possibly getting the colors
	if (m_iColorType == PER_VERTEX){
		iNumColors = m_pvtkColors->GetNumberOfTuples();
		beginEditCP(m_posgColors);{
			unsigned char aColor[4];
			for (i=0; i<iNumColors; i++){
				m_pvtkColors->GetTupleValue(i, aColor);
				float r = ((float) aColor[0]) / 255.0f;
				float g = ((float) aColor[1]) / 255.0f;
				float b = ((float) aColor[2]) / 255.0f;
				m_posgColors->addValue(Color3f(r, g, b));
			}
		}endEditCP(m_posgColors);
		if (iNumColors != iNumPoints){
			std::cerr << "WARNING: CVtkActorToOpenSG::ProcessGeometryNormalsAndColorsPerVertex() number of colors" << std::endl;
			std::cerr << "should equal the number of vertices (points)!" << std::endl << std::endl;
		}
	}

	//getting the cells
	beginEditCP(m_posgTypes);
	beginEditCP(m_posgLengths);
	beginEditCP(m_posgIndices);{
		vtkCellArray *pCells;
		int npts, *pts;
		int prim;

		prim = 0;
		pCells = pPolyData->GetVerts();
		if (pCells != NULL){
			if (pCells->GetNumberOfCells() > 0){
				for (pCells->InitTraversal(); pCells->GetNextCell(npts, pts); prim++){
					m_posgLengths->addValue(npts);
					m_posgTypes->addValue(GL_POINTS);
					for (i=0; i<npts; i++){
						m_posgIndices->addValue(pts[i]);
					}
				}
			}
		}

		prim = 0;
		pCells = pPolyData->GetLines();
		if (pCells != NULL){
			if (pCells->GetNumberOfCells() > 0){
				for (pCells->InitTraversal(); pCells->GetNextCell(npts, pts); prim++){
					m_posgLengths->addValue(npts);
					m_posgTypes->addValue(GL_LINE_STRIP);
					for (i=0; i<npts; i++){
						m_posgIndices->addValue(pts[i]);
					}
				}
			}
		}

		prim = 0;
		pCells = pPolyData->GetPolys();
		if (pCells != NULL){
			if (pCells->GetNumberOfCells() > 0){
				for (pCells->InitTraversal(); pCells->GetNextCell(npts, pts); prim++){
					m_posgLengths->addValue(npts);
					m_posgTypes->addValue(GL_POLYGON);
					for (i=0; i<npts; i++){
						m_posgIndices->addValue(pts[i]);
					}
				}
			}
		}

		prim = 0;
		pCells = pPolyData->GetStrips();
		if (pCells != NULL){
			if (pCells->GetNumberOfCells() > 0){
				for (pCells->InitTraversal(); pCells->GetNextCell(npts, pts); prim++){
					m_posgLengths->addValue(npts);
					m_posgTypes->addValue(GL_TRIANGLE_STRIP);
					for (i=0; i<npts; i++){
						m_posgIndices->addValue(pts[i]);
					}
				}
			}
		}
	}endEditCP(m_posgIndices);
	endEditCP(m_posgLengths);
	endEditCP(m_posgTypes);

	SimpleMaterialPtr material = CreateMaterial();
	beginEditCP(m_posgGeometry);{
		m_posgGeometry->setPositions(m_posgPoints);
		m_posgGeometry->setTypes(m_posgTypes);
		m_posgGeometry->setLengths(m_posgLengths);
		m_posgGeometry->setIndices(m_posgIndices);
		m_posgGeometry->setMaterial(material);

		if (m_iNormalType == PER_VERTEX) m_posgGeometry->setNormals(m_posgNormals);
		if (m_iColorType == PER_VERTEX) m_posgGeometry->setColors(m_posgColors);
	};endEditCP(m_posgGeometry);
	return m_posgGeomNode;
}

NodePtr vtkOsgActor::ProcessGeometryNonIndexedCopyAttributes(int gl_primitive_type){
	std::cout << "starting CVtkActorToOpenSG::ProcessGeometryNonIndexedCopyAttributes(int gl_primitive_type)" << std::endl;

	beginEditCP(m_posgTypes);{
		m_posgTypes->clear();
	};endEditCP(m_posgTypes);

	beginEditCP(m_posgLengths);{
		m_posgLengths->clear();
	};endEditCP(m_posgLengths);

	beginEditCP(m_posgIndices);{
		m_posgIndices->clear();
	};endEditCP(m_posgIndices);

	beginEditCP(m_posgPoints);{
		m_posgPoints->clear();
	};endEditCP(m_posgPoints);

	beginEditCP(m_posgColors);{
		m_posgColors->clear();
	};endEditCP(m_posgColors);

	beginEditCP(m_posgNormals);{
		m_posgNormals->clear();
	};endEditCP(m_posgNormals);

	vtkPolyData *pPolyData = (vtkPolyData*) this->GetMapper()->GetInput();
	vtkCellArray *pCells;
	if (gl_primitive_type == GL_POINTS){
		pCells = pPolyData->GetVerts();
	} else if (gl_primitive_type == GL_LINE_STRIP){
		pCells = pPolyData->GetLines();
	} else if (gl_primitive_type == GL_POLYGON){
		pCells = pPolyData->GetPolys();
	} else if (gl_primitive_type == GL_TRIANGLE_STRIP){
		pCells = pPolyData->GetStrips();
	} else {
		std::cerr << "CVtkActorToOpenSG::ProcessGeometryNonIndexedCopyAttributes(int gl_primitive_type)" << std::endl;
		std::cerr << "	was called with non implemented gl_primitive_type!" << std::endl;
		return NullFC;
	}

	beginEditCP(m_posgTypes);
	beginEditCP(m_posgLengths);
	beginEditCP(m_posgPoints);
	beginEditCP(m_posgColors);
	beginEditCP(m_posgNormals);{
		int prim = 0;
		int npts, *pts, i;
		if (pCells->GetNumberOfCells() > 0){
			for (pCells->InitTraversal(); pCells->GetNextCell(npts, pts); prim++){
				m_posgLengths->addValue(npts);
				m_posgTypes->addValue(GL_POLYGON);
				for (i=0; i<npts; i++){
					double *aVertex;
					double *aNormal;
					unsigned char aColor[4];

					aVertex = pPolyData->GetPoint(pts[i]);
					m_posgPoints->addValue(Vec3f(aVertex[0], aVertex[1], aVertex[2]));

					if (m_iNormalType == PER_VERTEX){
						aNormal = m_pvtkNormals->GetTuple(pts[i]);
						m_posgNormals->addValue(Vec3f(aNormal[0], aNormal[1], aNormal[2]));
					} else if (m_iNormalType == PER_CELL){
						aNormal = m_pvtkNormals->GetTuple(prim);
						m_posgNormals->addValue(Vec3f(aNormal[0], aNormal[1], aNormal[2]));
					}

					if (m_iColorType == PER_VERTEX){
						m_pvtkColors->GetTupleValue(pts[i], aColor);
						float r = ((float) aColor[0]) / 255.0f;
						float g = ((float) aColor[1]) / 255.0f;
						float b = ((float) aColor[2]) / 255.0f;
						m_posgColors->addValue(Color3f(r, g, b));
					} else if (m_iColorType == PER_CELL){
						m_pvtkColors->GetTupleValue(prim, aColor);
						float r = ((float) aColor[0]) / 255.0f;
						float g = ((float) aColor[1]) / 255.0f;
						float b = ((float) aColor[2]) / 255.0f;
						m_posgColors->addValue(Color3f(r, g, b));
					}
				}
			}
		}
	}endEditCP(m_posgTypes);
	endEditCP(m_posgLengths);
	endEditCP(m_posgPoints);
	endEditCP(m_posgColors);
	endEditCP(m_posgNormals);

	SimpleMaterialPtr material = CreateMaterial();
	//GeometryPtr geo = Geometry::create();
	beginEditCP(m_posgGeometry);{
		m_posgGeometry->setPositions(m_posgPoints);
		m_posgGeometry->setTypes(m_posgTypes);
		m_posgGeometry->setLengths(m_posgLengths);
		m_posgGeometry->setMaterial(material);

		if (m_iNormalType != NOT_GIVEN) m_posgGeometry->setNormals(m_posgNormals);
		if (m_iColorType != NOT_GIVEN) m_posgGeometry->setColors(m_posgColors);
		//geo->setMaterial(getDefaultMaterial());
	}endEditCP(m_posgGeometry);

	return m_posgGeomNode;
}

NodePtr vtkOsgActor::GetNodePtr(){
	if (m_bVerbose){
		std::cerr << "--------Starting vtkActorToOpenSG(...)----------" << std::endl;
	}

	this->GetMapper()->Update();

	//Rendering with OpenSG simple indexed geometry
	if (((m_iNormalType == PER_VERTEX) || (m_iNormalType == NOT_GIVEN))  &&
		((m_iColorType == PER_VERTEX) || (m_iColorType == NOT_GIVEN))){
			return this->ProcessGeometryNormalsAndColorsPerVertex();
	}else{
		//Rendering with OpenSG non indexed geometry by copying a lot of attribute data
		if (m_iNumGLPolygons > 0){
			if (m_iNumGLPolygons != m_iNumGLPrimitives){
				std::cerr << "WARNING: vtkActor contains different kind of primitives" << std::endl;
			}
			return this->ProcessGeometryNonIndexedCopyAttributes(GL_POLYGON);
		} else if (m_iNumGLLineStrips > 0){
			if (m_iNumGLLineStrips != m_iNumGLPrimitives){
				std::cerr << "WARNING: vtkActor contains different kind of primitives" << std::endl;
			}
			return this->ProcessGeometryNonIndexedCopyAttributes(GL_LINE_STRIP);
		} else if (m_iNumGLTriStrips > 0){
			if (m_iNumGLTriStrips != m_iNumGLPrimitives){
				std::cerr << "WARNING: vtkActor contains different kind of primitives" << std::endl;
			}
			return this->ProcessGeometryNonIndexedCopyAttributes(GL_TRIANGLE_STRIP);
		} else if (m_iNumGLPoints > 0){
			if (m_iNumGLPoints != m_iNumGLPrimitives){
				std::cerr << "WARNING: vtkActor contains different kind of primitives" << std::endl;
			}
			return this->ProcessGeometryNonIndexedCopyAttributes(GL_POINTS);
		} else {
			return NullFC;
		}
	}
}
