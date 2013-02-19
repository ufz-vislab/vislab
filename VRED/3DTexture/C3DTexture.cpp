#include "C3DTexture.h"

#include "vrOSGWidget.h"
#include "vrScenegraph.h"
#include "vrMaterials.h"
#include "vrLog.h"

#include <fstream>
#include <string>

static std::string _fp_program =
"uniform float fRPos;\n"
"uniform float fGPos;\n"
"uniform float fBPos;\n"
"\n"
"varying vec3 Texcoord;\n"
"varying vec3 ecPosition;\n"
"varying vec3 ecLightPosition;\n"
"\n"
"\n"
"uniform sampler3D baseMap;\n"
"\n"
"void main(void){\n"
"  vec3 red = vec3(1.0, 0.0, 0.0);\n"
"  vec3 green = vec3(0.95, 0.95, 0.95);\n"
"  vec3 blue = vec3(0.0, 0.0, 1.0);\n"
"  vec3 color;\n"
"  float alpha;\n"
"  \n"
"  vec4 fragment = texture3D( baseMap, Texcoord );\n"
"  \n"
"  // ----------------------------------------------------------------------\n"
"  // ---------------------- determining the color--------------------------\n"
"  if (fragment.r <= fBPos){\n"
"    color = blue;\n"
"  } else if (fragment.r < fGPos){\n"
"    float frac = (fragment.r - fBPos) / (fGPos - fBPos);\n"
"    color = mix(blue, green, frac);\n"
"  } else if (fragment.r < fRPos){\n"
"    float frac = (fragment.r - fGPos) / (fRPos - fGPos);\n"
"    color = mix(green, red, frac);\n"
"  } else if (fragment.r >= fRPos){\n"
"    color = red;\n"
"  }\n"
"    \n"
"  gl_FragColor = vec4(color, 1.0);\n"
"}\n";

static std::string _vp_program =
"varying vec3 Texcoord;\n"
"\n"
"void main(void){\n"
"  gl_Position = ftransform();\n"
"  Texcoord = vec3(gl_TextureMatrix[0] * gl_Vertex);\n"
"}  \n";

C3DTexture::C3DTexture(void):
	m_pDataVol(NULL),
	m_pRoot(OSG::NullFC),
	m_pMaterialChunk(OSG::NullFC),
	m_pTextureChunk(OSG::NullFC),
	m_pTexGenChunk(OSG::NullFC),
	m_pTextureTransformChunk(OSG::NullFC),
	m_pSHLChunk(OSG::NullFC),
	m_pMaterialGroup(OSG::NullFC),
	m_pImage(OSG::NullFC),
	m_pChunkMaterial(OSG::NullFC),
	m_pGeometry(OSG::NullFC),
	m_pGeoPTypes(OSG::NullFC),
	m_pGeoPLengths(OSG::NullFC),
	m_pGeoPositions3f(OSG::NullFC),
	m_EXVec(OSG::Vec3f(1.0f, 0.0f, 0.0f)),
	m_EYVec(OSG::Vec3f(0.0f, 1.0f, 0.0f)),
	m_EZVec(OSG::Vec3f(0.0f, 0.0f, 1.0f)),
	m_Origin(OSG::Pnt3f(0.0f, 0.0f, 0.0f)),
	m_bShowXSlice(false),
	m_bShowYSlice(false),
	m_bShowZSlice(false),
	m_fXSlicePos(0.5),
	m_fYSlicePos(0.5),
	m_fZSlicePos(0.5),
	m_bUseColorTable(false){
}

C3DTexture::~C3DTexture(void){
	if (m_pRoot != OSG::NullFC){
		subRefCP(m_pRoot);
		subRefCP(m_pSHLChunk);
	}
}

void C3DTexture::Init(){
	if (m_pMaterialGroup == OSG::NullFC){
		m_pMaterialGroup = OSG::MaterialGroup::create();
		m_pChunkMaterial = OSG::ChunkMaterial::create();
		m_pMaterialChunk = OSG::MaterialChunk::create();
		m_pTextureChunk = OSG::TextureChunk::create();
		m_pTexGenChunk = OSG::TexGenChunk::create();
		m_pSHLChunk = OSG::SHLChunk::create();
		m_pTextureTransformChunk = OSG::TextureTransformChunk::create();
		m_pRoot = OSG::Node::create();
		addRefCP(m_pRoot);
		addRefCP(m_pSHLChunk);

		beginEditCP(m_pTexGenChunk);{
			m_pTexGenChunk->setGenFuncS(GL_OBJECT_LINEAR);
			m_pTexGenChunk->setGenFuncT(GL_OBJECT_LINEAR);
			m_pTexGenChunk->setGenFuncR(GL_OBJECT_LINEAR);
		};endEditCP(m_pTexGenChunk);

		beginEditCP(m_pSHLChunk);{
			m_pSHLChunk->setUniformParameter("fRPos", 1.0f);
			m_pSHLChunk->setUniformParameter("fGPos", 0.5f);
			m_pSHLChunk->setUniformParameter("fBPos", 0.0f);
			m_pSHLChunk->setFragmentProgram(_fp_program);
			m_pSHLChunk->setVertexProgram(_vp_program);
		};endEditCP(m_pSHLChunk);

		beginEditCP(m_pChunkMaterial);{
			m_pChunkMaterial->addChunk(OSG::TwoSidedLightingChunk::create());
			m_pChunkMaterial->addChunk(m_pMaterialChunk);
			m_pChunkMaterial->addChunk(m_pTextureChunk);
			m_pChunkMaterial->addChunk(m_pTexGenChunk);
			m_pChunkMaterial->addChunk(m_pTextureTransformChunk);
			//m_pChunkMaterial->addChunk(m_pSHLChunk);
			setName(m_pChunkMaterial, "3DTextureMaterial");
		};endEditCP(m_pChunkMaterial);
		beginEditCP(m_pMaterialGroup);{
			m_pMaterialGroup->setMaterial(m_pChunkMaterial);
			setName(m_pMaterialGroup, "3DTexture");
		}; endEditCP(m_pMaterialGroup);
		beginEditCP(m_pRoot);{
			m_pRoot->setCore(m_pMaterialGroup);
			setName(m_pRoot, "3DTextureRoot");
		};endEditCP(m_pRoot);
	}
}

void C3DTexture::InitSlices(){
	Init();
	if (m_pGeometry == OSG::NullFC){
		OSG::NodePtr node = OSG::makeCoredNode<OSG::Geometry>(&m_pGeometry);
		setName(node, "Slices");
		beginEditCP(m_pRoot, OSG::Node::ChildrenFieldMask);{
			m_pRoot->addChild(node);
		};endEditCP(m_pRoot, OSG::Node::ChildrenFieldMask);

		m_pGeoPTypes = OSG::GeoPTypesUI8::create();
		m_pGeoPLengths = OSG::GeoPLengthsUI32::create();
		m_pGeoPositions3f = OSG::GeoPositions3f::create();

		beginEditCP(m_pGeoPTypes, OSG::GeoPTypesUI8::GeoPropDataFieldMask);{
			m_pGeoPTypes->addValue(GL_QUADS);
		};endEditCP  (m_pGeoPTypes, OSG::GeoPTypesUI8::GeoPropDataFieldMask);

		beginEditCP(m_pGeoPLengths, OSG::GeoPLengthsUI32::GeoPropDataFieldMask);{
			m_pGeoPLengths->addValue(0);
		};endEditCP  (m_pGeoPLengths, OSG::GeoPLengthsUI32::GeoPropDataFieldMask);

		beginEditCP(m_pGeoPositions3f);{
			for (int i=0; i<12; i++){
				m_pGeoPositions3f->addValue(OSG::Pnt3f(0.0, 0.0, 0.0));
			};
		};endEditCP(m_pGeoPositions3f);

		beginEditCP(m_pGeometry);{
			m_pGeometry->setTypes(m_pGeoPTypes);
			m_pGeometry->setLengths(m_pGeoPLengths);
			m_pGeometry->setPositions(m_pGeoPositions3f);
		};endEditCP(m_pGeometry);

		beginEditCP(m_pRoot);{
			m_pRoot->addChild(node);
		};endEditCP(m_pRoot);

		vrScenegraph::update();
	}
}

bool C3DTexture::AddToVRED(){
	vrLog::info("AddToVRED has been called");
	OSG::NodePtr vredRoot = OSG::NullFC;

	if (m_pRoot != OSG::NullFC){
		vredRoot = vrScenegraph::getRoot();
		if (vredRoot != OSG::NullFC){
			OSG::Int32 index = vredRoot->findChild(m_pRoot);
			if (index == -1){
				beginEditCP(vredRoot);{
					vredRoot->addChild(m_pRoot);
					vrScenegraph::update();
				};endEditCP(vredRoot);
				vrMaterials::addRef(m_pChunkMaterial);
				vrMaterials::setName(m_pChunkMaterial, "3D_Texture_Material");
				return true;
			} else {
				std::cout << "In AddToVred(), m_pRoot has been already added" << std::endl;
				return false;
			}
		} else {
			return false;
		}
	} else {
		return false;
	}
}

bool C3DTexture::RemoveFromVRED(){
	OSG::NodePtr vredRoot = OSG::NullFC;

	if (m_pRoot != OSG::NullFC){
		vredRoot = vrScenegraph::getRoot();
		if (vredRoot != OSG::NullFC){
			OSG::Int32 index = vredRoot->findChild(m_pRoot);
			std::cout << "child index: " << index << std::endl;
			if (index > -1){
				beginEditCP(vredRoot);{
					vredRoot->subChild(index);
				};endEditCP(vredRoot);

				vrScenegraph::update();
				vrMaterials::subRef(m_pChunkMaterial);
				vrMaterials::removeUnusedMaterials();
				vrMaterials::updateMaterialPool();
			}
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

bool C3DTexture::LoadImage(const char* filename){
	if (m_pDataVol != NULL){
		delete m_pDataVol;
	}

	m_pDataVol = new CDataVolume();

	std::string sFilename(filename);
	std::string endung;

	std::string::size_type pos = sFilename.find(".");
	if (pos != std::string::npos){
		endung = sFilename.substr(pos);
	} else {
		return false;
	}

	if (endung == ".bpf"){
		m_pDataVol->ReadTextureBin(sFilename.c_str());
		int iXDim = m_pDataVol->GetXDim();
		int iYDim = m_pDataVol->GetYDim();
		int iZDim = m_pDataVol->GetZDim();
		int components = m_pDataVol->GetNumComponents();

		std::cout << "xdim " << iXDim << std::endl;
		std::cout << "ydim " << iYDim << std::endl;
		std::cout << "zdim " << iZDim << std::endl;
		std::cout << "comps " << components << std::endl;

		if (components == 4){
			std::cout << "Reading four compoment image" << std::endl;
			int iNumTexels = iXDim * iYDim * iZDim;
			int iNumVals = iNumTexels * 4;

			//if (m_pImage == OSG::NullFC){
			//	m_pImage = OSG::Image::create();
			//}

			OSG::ImagePtr tmpImage = OSG::Image::create();

			beginEditCP(tmpImage);{
				tmpImage->set(OSG::Image::OSG_RGBA_PF,iXDim,iYDim,iZDim);
				OSG::UInt8 *d = tmpImage->getData();

				unsigned char *ucTexels = m_pDataVol->GetTexelPointer();
				int iSliceSize = iXDim * iYDim;

				for (int iZ=0; iZ<iZDim; iZ++){
					for (int iY=0; iY<iYDim; iY++){
						for (int iX=0; iX<iXDim; iX++){
						//int ind = 4 * (iZ * iSliceSize + iY * iYDim + iX);
							int ind = 4 * (iZ * iSliceSize + iY * iXDim + iX);
							d[ind+0] = (OSG::UInt8) ucTexels[ind+0];
							d[ind+1] = (OSG::UInt8) ucTexels[ind+1];
							d[ind+2] = (OSG::UInt8) ucTexels[ind+2];
							d[ind+3] = (OSG::UInt8) ucTexels[ind+3];
						}
					}
				}
			};endEditCP(tmpImage);
			tmpImage->scaleNextPower2();
			m_pImage = tmpImage;
		} else if (components == 1){
			std::cout << "Reading one component image" << std::endl;
			int iNumTexels = iXDim * iYDim * iZDim;
			int iNumVals = iNumTexels * 4;

			//if (m_pImage == OSG::NullFC){
			//	m_pImage = OSG::Image::create();
			//}

			OSG::ImagePtr tmpImage = OSG::Image::create();

			beginEditCP(tmpImage);{
				tmpImage->set(OSG::Image::OSG_L_PF,iXDim,iYDim,iZDim);
				OSG::UInt8 *d = tmpImage->getData();

				unsigned char *ucTexels = m_pDataVol->GetTexelPointer();
				int iSliceSize = iXDim * iYDim;

				for (int iZ=0; iZ<iZDim; iZ++){
					for (int iY=0; iY<iYDim; iY++){
						for (int iX=0; iX<iXDim; iX++){
							int ind = iZ * iSliceSize + iY * iXDim + iX;
							d[ind] = (OSG::UInt8) ucTexels[ind];
						}
					}
				}
			};endEditCP(tmpImage);
			tmpImage->scaleNextPower2();
			m_pImage = tmpImage;
		} else {
			return false;
		}
		std::cout << "Calling Init()" << std::endl;
		Init();

		beginEditCP(m_pTextureChunk);{
			m_pTextureChunk->setImage(m_pImage);
		};endEditCP(m_pTextureChunk);

		return true;
	}

	return false;
}

bool C3DTexture::LoadTransformation(const char* filename){
	std::ifstream in(filename);
	if (! in){
		std::cout << "Could not read transformation file: " << filename << std::endl;
		return false;
	}

	std::string tag;
	int controller = 0;
	float x, y, z;
	while (! in.eof()){
		in >> tag;
		if (tag == "ORIGIN"){
			in >> x;
			in >> y;
			in >> z;
			m_Origin = OSG::Pnt3f(x, y, z);
			std::cout << "Origin: " << x << " " << y << " " << z << std::endl;
			controller++;
		} else if (tag == "EX"){
			in >> x;
			in >> y;
			in >> z;
			m_EXVec = OSG::Vec3f(x, y, z);
			std::cout << "EX: " << x << " " << y << " " << z << std::endl;
			controller++;
		} else if (tag == "EY"){
			in >> x;
			in >> y;
			in >> z;
			m_EYVec = OSG::Vec3f(x, y, z);
			std::cout << "EY: " << x << " " << y << " " << z << std::endl;
			controller++;
		} else if (tag == "EZ"){
			in >> x;
			in >> y;
			in >> z;
			m_EZVec = OSG::Vec3f(x, y, z);
			std::cout << "EZ: " << x << " " << y << " " << z << std::endl;
			controller++;
		}
	}

	OSG::Matrix4f m;
	m.setValue(m_EXVec[0], m_EYVec[0], m_EZVec[0], m_Origin[0],
		m_EXVec[1], m_EYVec[1], m_EZVec[1], m_Origin[1],
		m_EXVec[2], m_EYVec[2], m_EZVec[2], m_Origin[2],
		0.0, 0.0, 0.0, 1.0);
	if (m.invert()){
		beginEditCP(m_pTextureTransformChunk);{
			m_pTextureTransformChunk->setMatrix(m);
		};endEditCP(m_pTextureTransformChunk);
	} else {
		std::cout << "Could not invert transformation for 3D texture!" << std::endl;
	}
	return (controller == 4);
}

void C3DTexture::SetSlicesRel(bool bX, bool bY, bool bZ, float fX, float fY, float fZ){
	if (m_pGeometry == OSG::NullFC){
		this->InitSlices();
	};

	int planes = 0;
	beginEditCP(m_pGeoPositions3f);{
		if (bX){
			OSG::Vec3f p1 = m_Origin + fX * m_EXVec;
			OSG::Vec3f p2 = m_Origin + fX * m_EXVec + m_EYVec;
			OSG::Vec3f p3 = m_Origin + fX * m_EXVec + m_EYVec + m_EZVec;
			OSG::Vec3f p4 = m_Origin + fX * m_EXVec + m_EZVec;

			m_pGeoPositions3f->setValue(p1, planes * 4 + 0);
			m_pGeoPositions3f->setValue(p2, planes * 4 + 1);
			m_pGeoPositions3f->setValue(p3, planes * 4 + 2);
			m_pGeoPositions3f->setValue(p4, planes * 4 + 3);
			planes++;
		};
		if (bY){
			OSG::Vec3f p1 = m_Origin + fY * m_EYVec;
			OSG::Vec3f p2 = m_Origin + m_EXVec + fY * m_EYVec;
			OSG::Vec3f p3 = m_Origin + m_EXVec + fY * m_EYVec + m_EZVec;
			OSG::Vec3f p4 = m_Origin + fY * m_EYVec + m_EZVec;

			m_pGeoPositions3f->setValue(p1, planes * 4 + 0);
			m_pGeoPositions3f->setValue(p2, planes * 4 + 1);
			m_pGeoPositions3f->setValue(p3, planes * 4 + 2);
			m_pGeoPositions3f->setValue(p4, planes * 4 + 3);
			planes++;
		};
		if (bZ){
			OSG::Vec3f p1 = m_Origin + fZ * m_EZVec;
			OSG::Vec3f p2 = m_Origin + m_EXVec + fZ * m_EZVec;
			OSG::Vec3f p3 = m_Origin + m_EXVec + m_EYVec + fZ * m_EZVec;
			OSG::Vec3f p4 = m_Origin + m_EYVec + fZ * m_EZVec;

			m_pGeoPositions3f->setValue(p1, planes * 4 + 0);
			m_pGeoPositions3f->setValue(p2, planes * 4 + 1);
			m_pGeoPositions3f->setValue(p3, planes * 4 + 2);
			m_pGeoPositions3f->setValue(p4, planes * 4 + 3);
			planes++;
		};
	};endEditCP(m_pGeoPositions3f);

	beginEditCP(m_pGeoPLengths);{
		m_pGeoPLengths->setValue(planes * 4, 0);
	};endEditCP(m_pGeoPLengths);
}

void C3DTexture::SetUseColortable(bool bUse){
	if ((m_pChunkMaterial == OSG::NullFC) || (m_pSHLChunk == OSG::NullFC)) return;

	if (bUse && (!m_bUseColorTable)){
		beginEditCP(m_pChunkMaterial);{
			m_pChunkMaterial->addChunk(m_pSHLChunk);
		};endEditCP(m_pChunkMaterial);
		m_bUseColorTable = true;
	} else if ((!bUse) && m_bUseColorTable){
		beginEditCP(m_pChunkMaterial);{
			m_pChunkMaterial->subChunk(m_pSHLChunk);
		};endEditCP(m_pChunkMaterial);
		m_bUseColorTable = false;
	}
}

void C3DTexture::SetColortable(float fLowClip, float fHighClip){
	if (m_pSHLChunk == OSG::NullFC) return;

	if ((fLowClip < 0.0) || (fLowClip > 1.0)) return;
	if ((fHighClip < 0.0) || (fHighClip > 1.0)) return;

	if (fHighClip < fLowClip){
		float tmp = fLowClip;
		fLowClip = fHighClip;
		fHighClip = tmp;
	}
	float fMed = 0.5 * (fHighClip + fLowClip);

	beginEditCP(m_pSHLChunk);{
		m_pSHLChunk->setUniformParameter("fRPos", fHighClip);
		m_pSHLChunk->setUniformParameter("fGPos", fMed);
		m_pSHLChunk->setUniformParameter("fBPos", fLowClip);
	};endEditCP(m_pSHLChunk);
}

void C3DTexture::CreateSimpleTexture(){
	if (m_pDataVol != NULL){
		delete m_pDataVol;
	}

	m_pDataVol = new CDataVolume();
	m_pDataVol->CreateVolume(7, 23, 51, 1);

	if (m_pImage == OSG::NullFC){
		m_pImage = OSG::Image::create();
	}

	int iXDim = m_pDataVol->GetXDim();
	int iYDim = m_pDataVol->GetYDim();
	int iZDim = m_pDataVol->GetZDim();
	int components = m_pDataVol->GetNumComponents();

	std::cout << "xdim " << iXDim << std::endl;
	std::cout << "ydim " << iYDim << std::endl;
	std::cout << "zdim " << iZDim << std::endl;
	std::cout << "comps " << components << std::endl;

	OSG::ImagePtr tmpImage = OSG::Image::create();

	beginEditCP(tmpImage);{
		tmpImage->set(OSG::Image::OSG_L_PF,iXDim,iYDim,iZDim);
		OSG::UInt8 *d = tmpImage->getData();

		unsigned char *ucTexels = m_pDataVol->GetTexelPointer();
		int iSliceSize = iXDim * iYDim;

		for (int iZ=0; iZ<iZDim; iZ++){
			for (int iY=0; iY<iYDim; iY++){
				for (int iX=0; iX<iXDim; iX++){
					int ind = iZ * iSliceSize + iY * iXDim + iX;
					d[ind] = (OSG::UInt8) ucTexels[ind];
				}
			}
		}
	};endEditCP(tmpImage);
	tmpImage->scaleNextPower2(m_pImage);
	Init();

	beginEditCP(m_pTextureChunk);{
		m_pTextureChunk->setImage(m_pImage);
	};endEditCP(m_pTextureChunk);
}