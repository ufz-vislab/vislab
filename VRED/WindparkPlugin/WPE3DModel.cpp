#include "WPE3DModel.h"

#include "OpenSG/OSGGeometry.h"
#include "OpenSG/OSGSimpleMaterial.h"

#define _USE_MATH_DEFINES
#include <math.h>

CWPE3DModel::CWPE3DModel(void){
	m_posgRoot = NullFC;

	m_posgBoundingBoxRoot = NullFC;
	m_posgBoundingBoxTransform = NullFC;
	m_uint32StandardTraverseMask = 0;

	m_posgGondelTransform = NullFC;
	m_posgPillarTransform = NullFC;
	m_posgRotorTransform = NullFC;
	m_posgMainTransform = NullFC;
}

CWPE3DModel::~CWPE3DModel(void){
}

bool CWPE3DModel::LoadModel(const char *filename, CWPEState state){
	NodePtr fileRoot = SceneFileHandler::the().read(filename);
	if (fileRoot == NullFC) return false;

	m_posgRoot = makeCoredNode<Transform>(&m_posgMainTransform);
	beginEditCP(m_posgRoot);{
		m_posgRoot->addChild(fileRoot);
	};endEditCP(m_posgRoot);

	NodeByNameSearcher s;
	NodePtr PillarTransformNode = s.SearchNode(m_posgRoot, std::string("PillarTransform"));
	NodePtr GondelTransformNode = s.SearchNode(m_posgRoot, std::string("GondelTransform"));
	NodePtr	RotorTransformNode = s.SearchNode(m_posgRoot, std::string("RotorRotation"));

	//if (PillarTransformNode != NullFC){
	//	m_posgPillarTransform = TransformPtr::dcast(PillarTransformNode->getCore());
	//}

	if (GondelTransformNode != NullFC){
		m_posgGondelTransform = TransformPtr::dcast(GondelTransformNode->getCore());
	}

	//if (RotorTransformNode != NullFC){
	//	m_posgRotorTransform = TransformPtr::dcast(RotorTransformNode->getCore());
	//}

	//why do I need to do this (see uncommented part above for initial version
	m_posgPillarTransform = Transform::create();
	beginEditCP(PillarTransformNode);{
		PillarTransformNode->setCore(m_posgPillarTransform);
	};endEditCP(PillarTransformNode);

	m_posgRotorTransform = Transform::create();
	beginEditCP(RotorTransformNode);{
		RotorTransformNode->setCore(m_posgRotorTransform);
	};endEditCP(RotorTransformNode);



	int counter = 0;
	if (m_posgPillarTransform == NullFC) std::cout << "CWEAVis::LoadModel(): could not find pillar transform node!" << std::endl, counter++;
	if (m_posgGondelTransform == NullFC) std::cout << "CWEAVis::LoadModel(): could not find gondel transform node!" << std::endl, counter++;
	if (m_posgRotorTransform == NullFC) std::cout << "CWEAVis::LoadModel(): could not find rotor rotation node!" << std::endl, counter++;
	if (counter > 0){
		return false;
	}else{
		this->Init(state);
		if ((m_posgBoundingBoxRoot == NullFC) || (m_posgBoundingBoxTransform == NullFC)){
			this->MakeBoundingBox();
		}
		this->SizeBoundingBox();
		return true;
	}
}

bool CWPE3DModel::SetState(CWPEState state){
	bool bFirst = true;
	bool bSecond = true;
	bool bThird = true;
	if (m_state.m_iId == state.m_iId){
		if (m_state.m_fPilarHeight != state.m_fPilarHeight){
			bFirst = this->SetGondelHeight(state.m_fPilarHeight);
		}
		if (m_state.m_fRotorLength != state.m_fRotorLength){
			bSecond = this->SetRotorDiameter(state.m_fRotorLength * 2.0);
		}
		if (m_state.m_fXPos != state.m_fXPos){
			bThird = this->SetTranslation(state.m_fXPos, state.m_fYPos, state.m_fZPos);
		}
		m_state = state;
		this->SizeBoundingBox();
	}
	return (bFirst && bSecond && bThird);
}

void CWPE3DModel::SetMarked(bool marked){
	if (m_posgBoundingBoxRoot == NullFC) return;

	if (marked){
		beginEditCP(m_posgBoundingBoxRoot, Node::TravMaskFieldMask);{
			m_posgBoundingBoxRoot->setTravMask(m_uint32StandardTraverseMask);
		};endEditCP(m_posgBoundingBoxRoot, Node::TravMaskFieldMask);
	}else{
		beginEditCP(m_posgBoundingBoxRoot, Node::TravMaskFieldMask);{
			m_posgBoundingBoxRoot->setTravMask(0);
		};endEditCP(m_posgBoundingBoxRoot, Node::TravMaskFieldMask);
	}
}


bool CWPE3DModel::SetGondelHeight(float height){
	if ((m_posgPillarTransform == NullFC) || (m_posgGondelTransform == NullFC)) return false;

	Matrix mGondel = m_posgGondelTransform->getMatrix();
	mGondel.setTranslate(0.0, 0.0, height);
	beginEditCP(m_posgGondelTransform);{
		m_posgGondelTransform->setMatrix(mGondel);
	};endEditCP(m_posgGondelTransform);

	Matrix mPillar;
	mPillar.setIdentity();
	mPillar.setScale(height / 20.0);
	mPillar.setTranslate(0.0, -2.0, 0.5 * height);
	beginEditCP(m_posgPillarTransform);{
		m_posgPillarTransform->setMatrix(mPillar);
	};endEditCP(m_posgPillarTransform);

	PrintPillarMatrix();
	return true;
}

bool CWPE3DModel::SetRotorDiameter(float diameter){
	//we need to scale Gondel and Rotor .....
	if (m_posgRotorTransform == NullFC) return false;
	Matrix m;
	m = m_posgGondelTransform->getMatrix();
	m.setScale(diameter / 20.0);
	beginEditCP(m_posgGondelTransform);{
		m_posgGondelTransform->setMatrix(m);
	};endEditCP(m_posgGondelTransform);
	return true;
}

bool CWPE3DModel::SetRotorRotation(float angle){
	if (m_posgRotorTransform == NullFC){
		std::cout << "rotor transformation is null!" << std::endl;
		return false;
	}
	Quaternion q;
	Matrix m;
	m.setIdentity();

	angle = angle + m_state.m_fXPos + m_state.m_fYPos;

	q = Quaternion(Vec3f(0, 1, 0), angle * 3.141 / 180.0);
	m.setRotate(q);

	beginEditCP(m_posgRotorTransform, Transform::MatrixFieldMask);{
		m_posgRotorTransform->setMatrix(m);
	};endEditCP(m_posgRotorTransform, Transform::MatrixFieldMask);
	return true;
}

bool CWPE3DModel::SetTranslation(float x, float y, float z){
	if (m_posgMainTransform == NullFC) return false;

	Matrix m;
	m = m_posgMainTransform->getMatrix();
	m.setTranslate(x, y, z);
	beginEditCP(m_posgMainTransform, Transform::MatrixFieldMask);{
		m_posgMainTransform->setMatrix(m);
	};endEditCP(m_posgMainTransform, Transform::MatrixFieldMask);
	return true;
}

bool CWPE3DModel::SetRotation(float angleInDegree){
	if (m_posgMainTransform == NullFC) return false;

	Matrix m;
	m = m_posgMainTransform->getMatrix();
	float angleInRad = M_PI * angleInDegree / 180.0;
	Quaternion q(Vec3f(0.0, 0.0, 1.0), angleInRad);
	m.setRotate(q);
	beginEditCP(m_posgMainTransform, Transform::MatrixFieldMask);{
		m_posgMainTransform->setMatrix(m);
	};endEditCP(m_posgMainTransform, Transform::MatrixFieldMask);
	return true;
}

Action::ResultE CWPE3DModel::NodeByNameSearcher::TraverseFunction(NodePtr &node){
	if (getName(node)){
		if (getName(node) == m_strName){
			m_foundNode = node;
				return Action::Quit;
			}
		}
		return Action::Continue;
	}

NodePtr CWPE3DModel::NodeByNameSearcher::SearchNode(NodePtr root, std::string nodeName){
	m_foundNode = NullFC;
	m_strName = nodeName;
	traverse(root, osgTypedMethodFunctor1ObjPtrCPtrRef<Action::ResultE, NodeByNameSearcher, NodePtr>(this, &NodeByNameSearcher::TraverseFunction));
	return m_foundNode;
}

void CWPE3DModel::PrintPillarMatrix(){
	if (m_posgPillarTransform == NullFC) return;

	Matrix m;
	m = m_posgPillarTransform->getMatrix();
}

bool CWPE3DModel::Init(CWPEState state){
	m_state = state;

	bool bFirst = this->SetGondelHeight(m_state.m_fPilarHeight);
	bool bSecond = this->SetRotorDiameter(m_state.m_fRotorLength * 2.0);
	bool bThird = this->SetTranslation(m_state.m_fXPos, m_state.m_fYPos, m_state.m_fZPos);
	if (m_posgRoot != NullFC) setName(m_posgRoot, m_state.m_stdstrName);
	return (bFirst && bSecond && bThird);
}

void CWPE3DModel::MakeBoundingBox(){
	m_posgBoundingBoxRoot = makeCoredNode<Transform>(&m_posgBoundingBoxTransform);
	m_uint32StandardTraverseMask = m_posgBoundingBoxRoot->getTravMask();

	GeoPTypesPtr types = GeoPTypesUI8::create();
	beginEditCP(types, GeoPTypesUI8::GeoPropDataFieldMask);{
        types->addValue(GL_LINES);
	};endEditCP  (types, GeoPTypesUI8::GeoPropDataFieldMask);

	GeoPLengthsPtr lengths = GeoPLengthsUI32::create();    
    beginEditCP(lengths, GeoPLengthsUI32::GeoPropDataFieldMask);{
        lengths->addValue(24);
    };endEditCP  (lengths, GeoPLengthsUI32::GeoPropDataFieldMask);

	GeoPositions3fPtr verts = GeoPositions3f::create();
	beginEditCP(verts, GeoPositions3f::GeoPropDataFieldMask);{
		verts->addValue(Pnt3f(-0.5, -0.5, -0.5));	verts->addValue(Pnt3f(0.5, -0.5, -0.5));
		verts->addValue(Pnt3f(0.5, -0.5, -0.5));	verts->addValue(Pnt3f(0.5, 0.5, -0.5));
		verts->addValue(Pnt3f(0.5, 0.5, -0.5));		verts->addValue(Pnt3f(-0.5, 0.5, -0.5));
		verts->addValue(Pnt3f(-0.5, 0.5, -0.5));	verts->addValue(Pnt3f(-0.5, -0.5, -0.5));

		verts->addValue(Pnt3f(-0.5, -0.5, -0.5));	verts->addValue(Pnt3f(-0.5, -0.5, 0.5));
		verts->addValue(Pnt3f(0.5, -0.5, -0.5));	verts->addValue(Pnt3f(0.5, -0.5, 0.5));
		verts->addValue(Pnt3f(0.5, 0.5, -0.5));		verts->addValue(Pnt3f(0.5, 0.5, 0.5));
		verts->addValue(Pnt3f(-0.5, 0.5, -0.5));	verts->addValue(Pnt3f(-0.5, 0.5, 0.5));

		verts->addValue(Pnt3f(-0.5, -0.5, 0.5));	verts->addValue(Pnt3f(0.5, -0.5, 0.5));
		verts->addValue(Pnt3f(0.5, -0.5, 0.5));		verts->addValue(Pnt3f(0.5, 0.5, 0.5));
		verts->addValue(Pnt3f(0.5, 0.5, 0.5));		verts->addValue(Pnt3f(-0.5, 0.5, 0.5));
		verts->addValue(Pnt3f(-0.5, 0.5, 0.5));		verts->addValue(Pnt3f(-0.5, -0.5, 0.5));
	};endEditCP(verts, GeoPositions3f::GeoPropDataFieldMask);

	Color3f color(1.0, 0.0, 0.0);
	SimpleMaterialPtr material = SimpleMaterial::create();
	beginEditCP(material);{
		material->setDiffuse(color);
		material->setEmission(color);
	};endEditCP(material);

	GeometryPtr geo = Geometry::create();
	beginEditCP(geo);{
        geo->setTypes(types);
        geo->setLengths(lengths);
        geo->setPositions(verts);
		geo->setMaterial(material);
    }endEditCP(geo);
	NodePtr geoNode = Node::create();
	beginEditCP(geoNode);{
		geoNode->setCore(geo);
	};endEditCP(geoNode);

	beginEditCP(m_posgBoundingBoxRoot);{
		m_posgBoundingBoxRoot->addChild(geoNode);
	};endEditCP(m_posgBoundingBoxRoot);
	
	beginEditCP(m_posgRoot);{
		m_posgRoot->addChild(m_posgBoundingBoxRoot);
	};endEditCP(m_posgRoot);

	SizeBoundingBox();
}

void CWPE3DModel::SizeBoundingBox(){
	float xLength = 2.0 * m_state.m_fRotorLength;
	float yLength = 2.0 * m_state.m_fRotorLength;
	float zLength = m_state.m_fPilarHeight + m_state.m_fRotorLength;

	Pnt3f position(0.0, 0.0, 0.5 * zLength);

	Matrix m;
	m.setIdentity();
	m.setScale(xLength, yLength, zLength);
	m.setTranslate(position);

	beginEditCP(m_posgBoundingBoxTransform, Transform::MatrixFieldMask);{
		m_posgBoundingBoxTransform->setMatrix(m);
	};endEditCP(m_posgBoundingBoxTransform, Transform::MatrixFieldMask);
}