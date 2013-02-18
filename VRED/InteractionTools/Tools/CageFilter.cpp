#include "CageFilter.h"

static std::string _vp_program =
"varying vec4 color;\n"
"varying float diffuseCoeff;\n"
"varying vec4 objectSpacePosition;\n"
"uniform vec3 shaderClipPlanesPos;\n"
"\n"
"void main(void){\n"
"    objectSpacePosition = gl_Vertex;\n"
"    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;;\n"
"    \n"
"    vec3 normal = normalize(gl_NormalMatrix * gl_Normal);\n"
"    color = gl_Color;\n"
"    \n"
"    float nDotVP = dot(normal, vec3(gl_LightSource[0].position));\n"
"    nDotVP = abs(nDotVP);\n"
"    float nDotHV = max(0.0, dot(normal, vec3(gl_LightSource[0].halfVector)));\n"
"    float pf;\n"
"    \n"
"    if (nDotVP == 0.0){\n"
"        pf = 0.0;\n"
"    } else {\n"
"        pf = pow(nDotHV, gl_FrontMaterial.shininess);\n"
"    }\n"
"    \n"
"    color = gl_LightSource[0].diffuse * nDotVP * gl_Color + gl_LightSource[0].specular * pf + 0.2 * gl_Color;\n"
"}\n";


static std::string _fp_program =
"varying vec4 color;\n"
"varying float diffuseCoeff;\n"
"varying vec4 objectSpacePosition;\n"
"uniform vec3 shaderMinVec;\n"
"uniform vec3 shaderMaxVec;\n"
"uniform vec3 shaderClipPlanesPos;\n"
"uniform bool shaderXClipReverse;\n"
"uniform bool shaderYClipReverse;\n"
"uniform bool shaderZClipReverse;\n"
"bvec3 shaderClipPlanesReverse = bvec3(shaderXClipReverse, shaderYClipReverse, shaderZClipReverse);\n"
"\n"
"//vec3 minVec = vec3(20, -30, -10);\n"
"//vec3 maxVec = vec3(80, -20, -2);\n"
"\n"
"void main(void){\n"
"    \n"
"    //if ((objectSpacePosition.x > 60)\n"
"    //    && (objectSpacePosition.y > -20)\n"
"    //    && (objectSpacePosition.z > -3)) discard;\n"
"    \n"
"    if (! all(greaterThanEqual(objectSpacePosition.xyz, shaderMinVec))) discard;\n"
"    if (! all(lessThanEqual(objectSpacePosition.xyz, shaderMaxVec))) discard;\n"
"	 bvec3 greaterVec = greaterThanEqual(objectSpacePosition.xyz, shaderClipPlanesPos);\n"
"    if (all(equal(greaterVec, shaderClipPlanesReverse))) discard;\n"
"    \n"
"    gl_FragColor = color;\n"
"}\n";

CCageFilter* CCageFilter::m_pInstance = NULL;

CCageFilter* CCageFilter::Instance(){
	if (m_pInstance == NULL){
		m_pInstance = new CCageFilter();
		return m_pInstance;
	} else {
		return m_pInstance;
	}
}

CCageFilter::CCageFilter(void){
	int i;

	m_stdvecNames.push_back(std::string("FBL_MARKER"));
	m_stdvecNames.push_back(std::string("FBR_MARKER"));
	m_stdvecNames.push_back(std::string("BBR_MARKER"));
	m_stdvecNames.push_back(std::string("BBL_MARKER"));
	m_stdvecNames.push_back(std::string("FTL_MARKER"));
	m_stdvecNames.push_back(std::string("FTR_MARKER"));
	m_stdvecNames.push_back(std::string("BTR_MARKER"));
	m_stdvecNames.push_back(std::string("BTL_MARKER"));
	m_stdvecNames.push_back(std::string("CLIPPLANES_MARKER"));

	//Initializing the cage shown as lines
	m_posgLinesRoot = makeCoredNode<Geometry>(&m_posgLinesGeometry);
	setName(m_posgLinesRoot, std::string("LinesRoot"));
	GeoPTypesPtr types = GeoPTypesUI8::create();
	beginEditCP(types);{
		for (i=0; i<12; i++){
			types->addValue(GL_LINE);
		}
	};endEditCP(types);
	GeoPLengthsPtr lengths = GeoPLengthsUI32::create();
	beginEditCP(lengths);{
		for (i=0; i<12; i++){
			lengths->addValue(2);
		};
	};endEditCP(lengths);
	m_posgLinesVertexPos = GeoPositions3f::create();
	beginEditCP(m_posgLinesVertexPos);{
		for (i=0; i<24; i++){
			m_posgLinesVertexPos->addValue(Pnt3f(0.0, 0.0, 0.0));
		}
	};endEditCP(m_posgLinesVertexPos);
	SimpleMaterialPtr linesMaterial = SimpleMaterial::create();
	beginEditCP(linesMaterial);{
		linesMaterial->setEmission(Color3f(1.0, 1.0, 1.0));
	};endEditCP(linesMaterial);
	beginEditCP(m_posgLinesGeometry);{
		m_posgLinesGeometry->setTypes(types);
		m_posgLinesGeometry->setLengths(lengths);
		m_posgLinesGeometry->setPositions(m_posgLinesVertexPos);
		m_posgLinesGeometry->setMaterial(linesMaterial);
	};endEditCP(m_posgLinesGeometry);

	m_posgMarkerRoot = makeCoredNode<Group>();
	m_posgRoot = makeCoredNode<Group>();
	addRefCP(m_posgRoot);

	m_posgMaterialRoot = makeCoredNode<MaterialGroup>(&m_posgMaterialGroup);
	m_posgChunkMaterial = ChunkMaterial::create();
	m_posgMaterialChunk = MaterialChunk::create();
	m_posgTwoSidedLightingChunk = TwoSidedLightingChunk::create();
	m_posgSHLChunk = SHLChunk::create();

	beginEditCP(m_posgSHLChunk);{
		m_posgSHLChunk->setVertexProgram(_vp_program);
		m_posgSHLChunk->setFragmentProgram(_fp_program);
		m_posgSHLChunk->setUniformParameter("shaderMinVec", Vec3f(0.0, 0.0, 0.0));
		m_posgSHLChunk->setUniformParameter("shaderMaxVec", Vec3f(0.0, 0.0, 0.0));
		m_posgSHLChunk->setUniformParameter("shaderClipPlanesPos", Vec3f(0.0, 0.0, 0.0));
		m_posgSHLChunk->setUniformParameter("shaderXClipReverse", true);
		m_posgSHLChunk->setUniformParameter("shaderYClipReverse", false);
		m_posgSHLChunk->setUniformParameter("shaderZClipReverse", true);
	};endEditCP(m_posgSHLChunk);
	beginEditCP(m_posgChunkMaterial);{
		m_posgChunkMaterial->addChunk(m_posgMaterialChunk);
		m_posgChunkMaterial->addChunk(m_posgTwoSidedLightingChunk);
		m_posgChunkMaterial->addChunk(m_posgSHLChunk);
	};endEditCP(m_posgChunkMaterial);

	beginEditCP(m_posgMaterialGroup);{
		m_posgMaterialGroup->setMaterial(m_posgChunkMaterial);
	};endEditCP(m_posgMaterialGroup);

	addRefCP(m_posgMarkerRoot);
	
	//m_pOldMarkerMatrices = new Matrix[8];
	//m_pOldMarkerPositions = new Vec3f[8];

	for (int i=0; i<9; i++){
		Matrix m;
		m.setIdentity();
		m_pOldMarkerMatrices[i] = m;
		m_pOldMarkerPositions[i] = Vec3f(0.0, 0.0, 0.0);

		m_pposgMarkerScale[i] = Transform::create();
		m_pposgMarkerTransform[i] = Transform::create();

		NodePtr scaleNode = Node::create();
		NodePtr transformNode = Node::create();

		NodePtr markerNode;
		if (i < 8){
			markerNode = makeBox(0.5, 0.5, 0.5, 1, 1, 1);
		} else {
			markerNode = makeSphere(3, 0.5);
		}
		setName(markerNode, m_stdvecNames[i]);

		beginEditCP(scaleNode);{
			scaleNode->setCore(m_pposgMarkerScale[i]);
			scaleNode->addChild(markerNode);
		};endEditCP(scaleNode);

		beginEditCP(transformNode);{
			transformNode->setCore(m_pposgMarkerTransform[i]);
			transformNode->addChild(scaleNode);
			setName(transformNode, m_stdvecNames[i]);
		};endEditCP(transformNode);

		beginEditCP(m_posgMarkerRoot);{
			m_posgMarkerRoot->addChild(transformNode);
		};endEditCP(m_posgMarkerRoot);
	}

	beginEditCP(m_posgMarkerRoot);{
		m_posgMarkerRoot->addChild(m_posgLinesRoot);
	};

	beginEditCP(m_posgRoot);{
			m_posgRoot->addChild(m_posgMarkerRoot);
			m_posgRoot->addChild(m_posgMaterialRoot);
	};endEditCP(m_posgRoot);

	SetOldMarkerPositions();
}

CCageFilter::~CCageFilter(void){
	subRefCP(m_posgMarkerRoot);
	subRefCP(m_posgRoot);
}

void CCageFilter::SetMarkerScaling(double scale){
	Matrix m;
	m.setIdentity();
	m.setScale(scale, scale, scale);

	for (int i=0; i<9; i++){
		beginEditCP(m_pposgMarkerScale[i], Transform::MatrixFieldMask);{
			m_pposgMarkerScale[i]->setMatrix(m);
		};endEditCP(m_pposgMarkerScale[i], Transform::MatrixFieldMask);
	}
}

void CCageFilter::SetAroundBox(Vec3f minvec, Vec3f maxvec){
	std::cout << "CCageFilter::SetAroundBox()" << std::endl;
	std::cout << "box min vec: " << minvec << std::endl;
	std::cout << "box max vec: " << maxvec << std::endl;
	std::cout << "old fbl vec: " << m_pOldMarkerPositions[0] << std::endl;
	std::cout << "old btr vec: " << m_pOldMarkerPositions[6] << std::endl;

	Vec3f xVec = Vec3f(maxvec[0] - minvec[0], 0.0, 0.0);
	Vec3f yVec = Vec3f(0.0, maxvec[1] - minvec[1], 0.0);
	Vec3f zVec = Vec3f(0.0, 0.0, maxvec[2] - minvec[2]);

	// Center of the roof
	Vec3f clipPlaneSpherePos = minvec + xVec * 0.5 + yVec * 0.5 + zVec * 0.4;
	Matrix m = m_pOldMarkerMatrices[8];
	m.setTranslate(clipPlaneSpherePos);
	beginEditCP(m_pposgMarkerTransform[8]);{
		m_pposgMarkerTransform[8]->setMatrix(m);
	};endEditCP(m_pposgMarkerTransform[8]);
	SetOldMarkerPositions();

	Vec3f fblTranslateVec = minvec - m_pOldMarkerPositions[0];
	Vec3f btrTranslateVec = maxvec - m_pOldMarkerPositions[6];

	std::cout << "fbl translate: " << fblTranslateVec << std::endl;
	std::cout << "btr translate: " << btrTranslateVec << std::endl;

	TransformMarker(m_stdvecNames[0], fblTranslateVec);
	SetOldMarkerPositions();
	TransformMarker(m_stdvecNames[6], btrTranslateVec);
	SetOldMarkerPositions();

	std::cout << "new fbl: " << m_pOldMarkerPositions[0];
	std::cout << "new btr: " << m_pOldMarkerPositions[6];

	//// FBL
	//m.setTranslate(minvec);
	//beginEditCP(m_pposgMarkerTransform[0], Transform::MatrixFieldId);{
	//	m_pposgMarkerTransform[0]->setMatrix(m);
	//};endEditCP(m_pposgMarkerTransform[0], Transform::MatrixFieldId);

	//// FBR
	//m.setTranslate(minvec + xVec);
	//beginEditCP(m_pposgMarkerTransform[1], Transform::MatrixFieldId);{
	//	m_pposgMarkerTransform[1]->setMatrix(m);
	//};endEditCP(m_pposgMarkerTransform[1], Transform::MatrixFieldId);

	//// BBR
	//m.setTranslate(minvec + xVec + yVec);
	//beginEditCP(m_pposgMarkerTransform[2], Transform::MatrixFieldId);{
	//	m_pposgMarkerTransform[2]->setMatrix(m);
	//};endEditCP(m_pposgMarkerTransform[2], Transform::MatrixFieldId);

	//// BBL
	//m.setTranslate(minvec + yVec);
	//beginEditCP(m_pposgMarkerTransform[3], Transform::MatrixFieldId);{
	//	m_pposgMarkerTransform[3]->setMatrix(m);
	//};endEditCP(m_pposgMarkerTransform[3], Transform::MatrixFieldId);

	//// FTL
	//m.setTranslate(minvec + zVec);
	//beginEditCP(m_pposgMarkerTransform[4], Transform::MatrixFieldId);{
	//	m_pposgMarkerTransform[4]->setMatrix(m);
	//};endEditCP(m_pposgMarkerTransform[4], Transform::MatrixFieldId);

	//// FTR
	//m.setTranslate(minvec + xVec + zVec);
	//beginEditCP(m_pposgMarkerTransform[5], Transform::MatrixFieldId);{
	//	m_pposgMarkerTransform[5]->setMatrix(m);
	//};endEditCP(m_pposgMarkerTransform[5], Transform::MatrixFieldId);

	//// BTR
	//m.setTranslate(minvec + xVec + yVec + zVec);
	//beginEditCP(m_pposgMarkerTransform[6], Transform::MatrixFieldId);{
	//	m_pposgMarkerTransform[6]->setMatrix(m);
	//};endEditCP(m_pposgMarkerTransform[6], Transform::MatrixFieldId);

	//// BTL
	//m.setTranslate(minvec + yVec + zVec);
	//beginEditCP(m_pposgMarkerTransform[7], Transform::MatrixFieldId);{
	//	m_pposgMarkerTransform[7]->setMatrix(m);
	//};endEditCP(m_pposgMarkerTransform[7], Transform::MatrixFieldId);

	//SetOldMarkerPositions();
}

void CCageFilter::SetOldMarkerPositions(){
	Vec3f translation;
	Quaternion qDummy1;
	Vec3f	dummy;
	Quaternion qDummy2;

	for (int i=0; i<9; i++){
		m_pOldMarkerMatrices[i] = m_pposgMarkerTransform[i]->getMatrix();
		m_pOldMarkerMatrices[i].getTransform(translation, qDummy1, dummy, qDummy2);
		m_pOldMarkerPositions[i] = translation;
	}
}

void CCageFilter::TransformMarker(std::string name, Vec3f translation){
	// influence matrix FBL FBR BBR BBL FTL FTR BTR BTL
	int m_piInf[8][8] =
		{{7,6,4,5,3,2,0,1},
		 {6,7,5,4,2,3,1,0},
		 {4,5,7,6,0,1,3,2},
		 {5,4,6,7,1,0,2,3},
		 {3,2,0,1,7,6,4,5},
		 {2,3,1,0,6,7,5,4},
		 {0,1,3,2,4,5,7,6},
		 {1,0,2,3,5,4,6,7},
	};

	int k = -1;

	int i;
	for (i=0; i<8; i++){
		if (m_stdvecNames[i] == name){
			k = i;
			break;
		}
	}
	if (k == -1){
		std::cout << "error while updating corner" << std::endl;
		return;
	}

	Vec3f xVec = Vec3f(translation[0], 0.0, 0.0);
	Vec3f yVec = Vec3f(0.0, translation[1], 0.0);
	Vec3f zVec = Vec3f(0.0, 0.0, translation[2]);

	for (i=0; i<8; i++){
		Vec3f newTranslation = Vec3f(0.0, 0.0, 0.0);
		newTranslation += xVec * int(bool(m_piInf[k][i] & 1));
		newTranslation += yVec * int(bool(m_piInf[k][i] & 2));
		newTranslation += zVec * int(bool(m_piInf[k][i] & 4));
		Vec3f newTrans = m_pOldMarkerPositions[i] + newTranslation;

		Matrix m = m_pOldMarkerMatrices[i];
		m.setTranslate(newTrans);

		beginEditCP(m_pposgMarkerTransform[i], Transform::MatrixFieldMask);{
			m_pposgMarkerTransform[i]->setMatrix(m);
		};endEditCP(m_pposgMarkerTransform[i], Transform::MatrixFieldMask);

		if (i == 0){
			m_vec3fMinVec = newTrans;
			m_vec3fMaxVec = newTrans;
		} else {
			if (newTrans[0] < m_vec3fMinVec[0]) m_vec3fMinVec[0] = newTrans[0];
			if (newTrans[1] < m_vec3fMinVec[1]) m_vec3fMinVec[1] = newTrans[1];
			if (newTrans[2] < m_vec3fMinVec[2]) m_vec3fMinVec[2] = newTrans[2];

			if (newTrans[0] > m_vec3fMaxVec[0]) m_vec3fMaxVec[0] = newTrans[0];
			if (newTrans[1] > m_vec3fMaxVec[1]) m_vec3fMaxVec[1] = newTrans[1];
			if (newTrans[2] > m_vec3fMaxVec[2]) m_vec3fMaxVec[2] = newTrans[2];
		}
	}

	//Setting the lines for the cage
	beginEditCP(m_posgLinesVertexPos);{
		m_posgLinesVertexPos->setValue(Pnt3f(m_vec3fMinVec[0], m_vec3fMinVec[1], m_vec3fMinVec[2]), 0); //FBL
		m_posgLinesVertexPos->setValue(Pnt3f(m_vec3fMinVec[0], m_vec3fMaxVec[1], m_vec3fMinVec[2]), 1); //BBL
		m_posgLinesVertexPos->setValue(Pnt3f(m_vec3fMinVec[0], m_vec3fMaxVec[1], m_vec3fMinVec[2]), 2); //BBL
		m_posgLinesVertexPos->setValue(Pnt3f(m_vec3fMaxVec[0], m_vec3fMaxVec[1], m_vec3fMinVec[2]), 3); //BBR
		m_posgLinesVertexPos->setValue(Pnt3f(m_vec3fMaxVec[0], m_vec3fMaxVec[1], m_vec3fMinVec[2]), 4); //BBR
		m_posgLinesVertexPos->setValue(Pnt3f(m_vec3fMaxVec[0], m_vec3fMinVec[1], m_vec3fMinVec[2]), 5); //FBR
		m_posgLinesVertexPos->setValue(Pnt3f(m_vec3fMaxVec[0], m_vec3fMinVec[1], m_vec3fMinVec[2]), 6); //FBR
		m_posgLinesVertexPos->setValue(Pnt3f(m_vec3fMinVec[0], m_vec3fMinVec[1], m_vec3fMinVec[2]), 7); //FBL

		m_posgLinesVertexPos->setValue(Pnt3f(m_vec3fMinVec[0], m_vec3fMinVec[1], m_vec3fMinVec[2]), 8); //FBL
		m_posgLinesVertexPos->setValue(Pnt3f(m_vec3fMinVec[0], m_vec3fMinVec[1], m_vec3fMaxVec[2]), 9); //FTL
		m_posgLinesVertexPos->setValue(Pnt3f(m_vec3fMinVec[0], m_vec3fMaxVec[1], m_vec3fMinVec[2]), 10); //BBL
		m_posgLinesVertexPos->setValue(Pnt3f(m_vec3fMinVec[0], m_vec3fMaxVec[1], m_vec3fMaxVec[2]), 11); //BTL
		m_posgLinesVertexPos->setValue(Pnt3f(m_vec3fMaxVec[0], m_vec3fMaxVec[1], m_vec3fMinVec[2]), 12); //BBR
		m_posgLinesVertexPos->setValue(Pnt3f(m_vec3fMaxVec[0], m_vec3fMaxVec[1], m_vec3fMaxVec[2]), 13); //BTR
		m_posgLinesVertexPos->setValue(Pnt3f(m_vec3fMaxVec[0], m_vec3fMinVec[1], m_vec3fMinVec[2]), 14); //FBR
		m_posgLinesVertexPos->setValue(Pnt3f(m_vec3fMaxVec[0], m_vec3fMinVec[1], m_vec3fMaxVec[2]), 15); //FTR

		m_posgLinesVertexPos->setValue(Pnt3f(m_vec3fMinVec[0], m_vec3fMinVec[1], m_vec3fMaxVec[2]), 16); //FTL
		m_posgLinesVertexPos->setValue(Pnt3f(m_vec3fMinVec[0], m_vec3fMaxVec[1], m_vec3fMaxVec[2]), 17); //BTL
		m_posgLinesVertexPos->setValue(Pnt3f(m_vec3fMinVec[0], m_vec3fMaxVec[1], m_vec3fMaxVec[2]), 18); //BTL
		m_posgLinesVertexPos->setValue(Pnt3f(m_vec3fMaxVec[0], m_vec3fMaxVec[1], m_vec3fMaxVec[2]), 19); //BTR
		m_posgLinesVertexPos->setValue(Pnt3f(m_vec3fMaxVec[0], m_vec3fMaxVec[1], m_vec3fMaxVec[2]), 20); //BTR
		m_posgLinesVertexPos->setValue(Pnt3f(m_vec3fMaxVec[0], m_vec3fMinVec[1], m_vec3fMaxVec[2]), 21); //FTR
		m_posgLinesVertexPos->setValue(Pnt3f(m_vec3fMaxVec[0], m_vec3fMinVec[1], m_vec3fMaxVec[2]), 22); //FTR
		m_posgLinesVertexPos->setValue(Pnt3f(m_vec3fMinVec[0], m_vec3fMinVec[1], m_vec3fMaxVec[2]), 23); //FTL
	};endEditCP(m_posgLinesVertexPos);

	Pnt3f shaderMinPnt;
	Pnt3f shaderMaxPnt;

	Pnt3f minPnt(m_vec3fMinVec);
	Pnt3f maxPnt(m_vec3fMaxVec);

	m_osgFilterToGeometryMatrix.multFullMatrixPnt(minPnt, shaderMinPnt);
	m_osgFilterToGeometryMatrix.multFullMatrixPnt(maxPnt, shaderMaxPnt);

	Vec3f shaderMinVec(shaderMinPnt);
	Vec3f shaderMaxVec(shaderMaxPnt);

	beginEditCP(m_posgSHLChunk, SHLChunk::ParametersFieldMask);{
		m_posgSHLChunk->setUniformParameter("shaderMinVec", shaderMinVec);
		m_posgSHLChunk->setUniformParameter("shaderMaxVec", shaderMaxVec);
	};endEditCP(m_posgSHLChunk, SHLChunk::ParametersFieldMask);
}

void CCageFilter::TransformClipPlanesMarker(std::string name, Vec3f translation){
	if (!(name == m_stdvecNames[8])) return;

	Matrix m = m_pOldMarkerMatrices[8];
	Vec3f newTranslation = translation + m_pOldMarkerPositions[8];
	m.setTranslate(newTranslation);

	beginEditCP(m_pposgMarkerTransform[8], Transform::MatrixFieldMask);{
		m_pposgMarkerTransform[8]->setMatrix(m);
	};endEditCP(m_pposgMarkerTransform[8], Transform::MatrixFieldMask);

	Pnt3f clipPlanesPos(newTranslation);
	Pnt3f shaderClipPlanesPosPnt;
	m_osgFilterToGeometryMatrix.multFullMatrixPnt(clipPlanesPos, shaderClipPlanesPosPnt);
	Vec3f shaderClipPlanesPosVec(shaderClipPlanesPosPnt);

	beginEditCP(m_posgSHLChunk, SHLChunk::ParametersFieldMask);{
		m_posgSHLChunk->setUniformParameter("shaderClipPlanesPos", shaderClipPlanesPosVec);
	};endEditCP(m_posgSHLChunk, SHLChunk::ParametersFieldMask);
}

void CCageFilter::SetClippedOctant(std::string octantName){
	bool bvec[3]={false, false, false};

	if (octantName == m_stdvecNames[0]){
	} else if (octantName == m_stdvecNames[1]){
		bvec[0] = true;
	} else if (octantName == m_stdvecNames[2]){
		bvec[0] = true;
		bvec[1] = true;
	} else if (octantName == m_stdvecNames[3]){
		bvec[1] = true;
	} else if (octantName == m_stdvecNames[4]){
		bvec[2] = true;
	} else if (octantName == m_stdvecNames[5]){
		bvec[2] = true;
		bvec[0] = true;
	} else if (octantName == m_stdvecNames[6]){
		bvec[2] = true;
		bvec[0] = true;
		bvec[1] = true;
	} else if (octantName == m_stdvecNames[7]){
		bvec[2] = true;
		bvec[1] = true;
	}

	beginEditCP(m_posgSHLChunk, SHLChunk::ParametersFieldMask);{
		m_posgSHLChunk->setUniformParameter("shaderXClipReverse", bvec[0]);
		m_posgSHLChunk->setUniformParameter("shaderYClipReverse", bvec[1]);
		m_posgSHLChunk->setUniformParameter("shaderZClipReverse", bvec[2]);
	};endEditCP(m_posgSHLChunk, SHLChunk::ParametersFieldMask);
}

bool CCageFilter::IsBBoxMarker(std::string nodeName){
	for (int i=0; i<8; i++){
		if (m_stdvecNames[i] == nodeName) return true;
	}
	return false;
}

bool CCageFilter::IsClipPlanesMarker(std::string nodeName){
	return (m_stdvecNames[8] == nodeName);
}

void CCageFilter::SetChild(NodePtr childRoot){
	if (childRoot == NullFC) return;

	// evaluate the bounding box
	childRoot->updateVolume();
	DynamicVolume bbox = childRoot->getVolume();
	Vec3f minVec, maxVec;
	bbox.getBounds(minVec, maxVec);

	std::cout << "CCageFilter::SetChild(): " << std::endl;
	std::cout << "	minVec: " << minVec << std::endl;
	std::cout << "	maxVec: " << maxVec << std::endl;

	this->SetAroundBox(minVec, maxVec);
	beginEditCP(m_posgMaterialRoot);{
		m_posgMaterialRoot->getMFChildren()->clear();
		m_posgMaterialRoot->addChild(childRoot);
	};endEditCP(m_posgMaterialRoot);

	NodePtr geometryNode = FindGeometry(childRoot);
	GeometryPtr geometry = GeometryPtr::dcast(geometryNode->getCore());

	if (geometry != NullFC){
		std::cout << "geometry to world: " << std::endl;
		Matrix geometryToWorldMatrix = geometryNode->getToWorld();
		std::cout << geometryToWorldMatrix << std::endl;

		Matrix worldToGeometryMatrix = geometryToWorldMatrix;
		worldToGeometryMatrix.invert();
		std::cout << "world to geometry matrix" << std::endl;
		std::cout << worldToGeometryMatrix << std::endl;

		Matrix filterRootToWorldMatrix = m_posgRoot->getToWorld();
		std::cout << "filter root to world matrix" << std::endl;
		std::cout << filterRootToWorldMatrix << std::endl;

		m_osgFilterToGeometryMatrix.setIdentity();
		m_osgFilterToGeometryMatrix.mult(filterRootToWorldMatrix);
		m_osgFilterToGeometryMatrix.mult(worldToGeometryMatrix);
		std::cout << "filter to geometry matrix:" << std::endl;
		std::cout << m_osgFilterToGeometryMatrix << std::endl;
	}
}

NodePtr CCageFilter::FindGeometry(NodePtr node){
	int numChildren = node->getNChildren();

	if (node->getCore()->getType().isDerivedFrom(Geometry::getClassType())){
		std::cout << "found a geometry node" << std::endl;
		return node;
	}

	for (int i=0; i<numChildren; i++){
		NodePtr geomNode = FindGeometry(node->getChild(i));
		if (geomNode != NullFC) return geomNode;
	}

	return NullFC;
}

NodePtr CCageFilter::GetMarkerRoot(){
	return m_posgMarkerRoot;
}

NodePtr CCageFilter::GetRoot(){
	return m_posgRoot;
}

NodePtr CCageFilter::FindNodeWithName(NodePtr node, std::string name){
	int numChildren = node->getNChildren();

	if (getName(node)){
		std::string nodeName(getName(node));
		if (nodeName == name)return node;
	}

	for (int i=0; i<numChildren; i++){
		NodePtr childNode = FindNodeWithName(node->getChild(i), name);
		if (childNode != NullFC) return childNode;
	}

	return NullFC;
}

NodePtr CCageFilter::FindGeometryNodeWithName(NodePtr subSceneRoot, std::string name){
	NodePtr geomNode = FindNodeWithName(subSceneRoot, name);
	if (geomNode != NullFC){
		if (geomNode->getCore()->getType().isDerivedFrom(Geometry::getClassType())){
			return geomNode;
		}
	}
	return NullFC;
}

bool CCageFilter::SetCageFilterAroundNode(NodePtr sceneRoot, std::string nodeName){
	std::cout << "Removed old child from cage filter" << std::endl;
	NodePtr currentParent = m_posgRoot->getParent();

	int numberOfCurrentChildren = m_posgMaterialRoot->getNChildren();
	for (int i=0; i<numberOfCurrentChildren; i++){
		currentParent->addChild(m_posgMaterialRoot->getChild(i));
	}

	if (currentParent != NullFC){
		currentParent->subChild(m_posgRoot);
	}

	NodePtr newChild = NullFC;
	NodePtr newParent = NullFC;
	
	newChild = FindNodeWithName(sceneRoot, nodeName);
	if (newChild != NullFC) newParent = newChild->getParent();

	if ((newChild != NullFC) && (newParent != NullFC)){
		std::cout << "inserting cage" << std::endl;
		newParent->addChild(m_posgRoot);
		addRefCP(newChild);
		newParent->subChild(newChild);
		this->SetChild(newChild);
		subRefCP(newChild);
		return true;
	} else {
		return false;
	}
}