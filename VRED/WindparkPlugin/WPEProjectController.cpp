#include "WPEProjectController.h"

#include "WPEAttrPanel.h"
#include "WPEDiagramView.h"
#include "WPEDiagramModel.h"
#include "WPEState.h"
#include "WPETypeInfo.h"
#include "ElevationGridBase.h"
#include "TriangleElevationGrid.h"
#include "WPE3DModel.h"

#include <vrScenegraph.h>
#include <vrLog.h>

#include "OpenSG/OSGSimpleGeometry.h"
#include "OpenSG/OSGSceneFileHandler.h"

#include <fstream>

CWPEProjectController* CWPEProjectController::m_pInstance = NULL;
int CWPEProjectController::m_iCurrentId = 100;

CWPEProjectController::CWPEProjectController(void){
	m_pAttrPanel = NULL;
	m_pDiagramView = NULL;

	m_pElevationGrid = new CElevationGridBase;

	m_bUseRotorRotation = false;
}

CWPEProjectController::~CWPEProjectController(void){
}

CWPEProjectController *CWPEProjectController::Instance(){
	if (m_pInstance == NULL){
		m_pInstance = new CWPEProjectController();
	}
	return m_pInstance;
}

void CWPEProjectController::SetAttrPanel(CWPEAttrPanel *panel){
	m_pAttrPanel = panel;
}

void CWPEProjectController::SetDiagramView(CWPEDiagramView *view){
	m_pDiagramView = view;
}

int CWPEProjectController::GetUniqueId(){
	return m_iCurrentId++;
}

CWPEState CWPEProjectController::GetNewState(const char *wpeTypeName){
	CWPETypeInfo typeInfo;
	if (wpeTypeName != NULL){
		std::string typeName(wpeTypeName);
		std::map<std::string, CWPETypeInfo>::iterator iter;
		iter = m_stdmapWPETypes.find(typeName);
		if (iter != m_stdmapWPETypes.end()){
			typeInfo = iter->second;
		}else{
		}
	}
	CWPEState wpeState;
	wpeState.m_fInfluenceRadius = typeInfo.m_fInfluenceRadius;
	wpeState.m_fPilarHeight = typeInfo.m_fPilarHeight;
	wpeState.m_fRotorLength = typeInfo.m_fRotorLength;
	wpeState.m_stdstrWPETypeName = typeInfo.m_stdstrWPETypeName;
	wpeState.m_iId = this->GetUniqueId();
	char cUniqueName[255];
	sprintf(cUniqueName, "WPE_%i", wpeState.m_iId);
	wpeState.m_stdstrName = std::string(cUniqueName);
	return wpeState;
}

void CWPEProjectController::InitAddWPE(){
	if (m_pDiagramView != NULL){
		CWPEState state;
		
		if (m_pAttrPanel != NULL){
			const char *wpeTypeName = m_pAttrPanel->GetCurrentTypeNameFromTypeNamesCombo();
			state = this->GetNewState(wpeTypeName);
		}else{
		}

		CWPEDiagramModel *item = new CWPEDiagramModel(state, m_pDiagramView->canvas());
		m_pDiagramView->AddItem(item);
	}
}

void CWPEProjectController::FinishAddWPE(CWPEState state, float fXPos, float fYPos){
	float x, y, z;
	this->Get3DCoordFrom2DCoords(fXPos, fYPos, x, y, z);
	state.m_fXPos = x;
	state.m_fYPos = y;
	state.m_fZPos = z;
	m_stdmapWPEStates.insert(std::map<int, CWPEState>::value_type(state.m_iId, state));

	CWPE3DModel *model = new CWPE3DModel();

	vrLog::info("attempt to load file: %s", m_stdstr3DModelFilename.c_str());
	bool ifModelLoaded = model->LoadModel(m_stdstr3DModelFilename.c_str(), state);
	m_stdmap3DModels.insert(std::map<int,CWPE3DModel*>::value_type(state.m_iId, model));

	if (m_pAttrPanel != NULL){
		float direction = m_pAttrPanel->GetWinddirection();
		direction = 180.0 - direction;
		model->SetRotation(direction);
	}

	//NodePtr root = vrScenegraph::getRoot();
	NodePtr root = vrScenegraph::getInternalRoot();
	NodePtr addedNode = model->GetRoot();

	beginEditCP(root);{
		root->addChild(addedNode);
	};endEditCP(root);

	this->SetEnergyProduction();
	//vrScenegraph::update();
}


void CWPEProjectController::RemoveActiveWPE(){
	if (m_pDiagramView != NULL){
		int removedId = m_pDiagramView->RemoveActiveItem();

		// removing the corresponding state from the states list (map)
		std::map<int, CWPEState>::iterator stateIter = m_stdmapWPEStates.find(removedId);
		if (stateIter != m_stdmapWPEStates.end()){
			CWPEState state = stateIter->second;
			m_stdmapWPEStates.erase(stateIter);

			//NodePtr node = vrScenegraph::findNode(state.m_stdstrName.c_str());
			//NodePtr parent = node->getParent();
			//parent->subChild(node);
			//vrScenegraph::deleteNode(node);
		}

		// removing the corresponding 3D model from the 3D model list (map)
		std::map<int, CWPE3DModel*>::iterator modelIter = m_stdmap3DModels.find(removedId);
		if (modelIter != m_stdmap3DModels.end()){
			CWPE3DModel *model = modelIter->second;
			m_stdmap3DModels.erase(modelIter);

			NodePtr modelRoot = model->GetRoot();

			//NodePtr node = vrScenegraph::findNode(getName(modelRoot));
			//if (node == NullFC){
			//	std::cout << "error, found NullFC" << std::endl;
			//}
			//NodePtr parent = node->getParent();
			//if (parent == NullFC){
			//	std::cout << "error, found NullFC" << std::endl;
			//}
			//std::vector<vrNodePtr> someNodes;
			//someNodes.push_back(node);
			//vrScenegraph::subChilds(parent, someNodes);
			
			NodePtr root = vrScenegraph::getInternalRoot();
			beginEditCP(root, Node::ChildrenFieldMask);{
				root->subChild(modelRoot);
			};endEditCP(root, Node::ChildrenFieldMask);

			//vrScenegraph::deleteNode(modelRoot);
			delete model;
		}

		this->SetEnergyProduction();
	}
}

void CWPEProjectController::ChangeTypeOfActiveWPE(const char *wpeTypeName){
	if (m_pDiagramView != NULL){
		int currentWPEId = m_pDiagramView->GetIdOfActiveItem();

		//checking for the state
		std::map<int, CWPEState>::iterator statesIter = m_stdmapWPEStates.find(currentWPEId);
		if (statesIter == m_stdmapWPEStates.end()) return;
		CWPEState currentState = statesIter->second;

		//getting the new type
		std::string typeName(wpeTypeName);
		std::map<std::string, CWPETypeInfo>::iterator typesIter;
		typesIter = m_stdmapWPETypes.find(typeName);

		CWPETypeInfo typeInfo;
		if (typesIter != m_stdmapWPETypes.end()){
			typeInfo = typesIter->second;
		}

		//setting the new state
		currentState.m_fInfluenceRadius = typeInfo.m_fInfluenceRadius;
		currentState.m_fPilarHeight = typeInfo.m_fPilarHeight;
		currentState.m_fRotorLength = typeInfo.m_fRotorLength;
		currentState.m_stdstrWPETypeName = typeInfo.m_stdstrWPETypeName;
		m_stdmapWPEStates.erase(statesIter);
		m_stdmapWPEStates.insert(std::map<int, CWPEState>::value_type(currentState.m_iId, currentState));

		vrLog::info("New type: %s", currentState.m_stdstrWPETypeName.c_str());
		vrLog::info("   %f %f", currentState.m_fPilarHeight, currentState.m_fRotorLength);

		//setting the 2D view
		m_pDiagramView->AdaptActiveItemToNewState(currentState);

		//setting the 3D view
		std::map<int, CWPE3DModel*>::iterator modelIter = m_stdmap3DModels.find(currentWPEId);
		if (modelIter != m_stdmap3DModels.end()){
			CWPE3DModel *model = modelIter->second;
			model->SetState(currentState);
		}

		//setting the new energy production
		this->SetEnergyProduction();
	}
}

void CWPEProjectController::SetActiveWPE(int id){
	//Get the state for the wpe
	std::map<int, CWPEState>::iterator statesIter = m_stdmapWPEStates.find(id);
	if (statesIter == m_stdmapWPEStates.end()) return;
	CWPEState currentState = statesIter->second;

	//Get the wpe type and set it in the attribute panel
	std::map<std::string, CWPETypeInfo>::iterator typesIter = m_stdmapWPETypes.find(currentState.m_stdstrWPETypeName);
	if (typesIter == m_stdmapWPETypes.end()) return;
	CWPETypeInfo currentType = typesIter->second;
	if (m_pAttrPanel != NULL){
		m_pAttrPanel->SetCurrentWPEType(currentType);
	};

	//Mark the node as selected in the 3D view
	std::map<int, CWPE3DModel*>::iterator model3DIter = m_stdmap3DModels.find(id);
	if (model3DIter != m_stdmap3DModels.end()){
		model3DIter->second->SetMarked(true);
	};
}

void CWPEProjectController::UnsetActiveWPE(int id){
	//Unmark the node as selected in the 3D view
	std::map<int, CWPE3DModel*>::iterator model3DIter = m_stdmap3DModels.find(id);
	if (model3DIter != m_stdmap3DModels.end()){
		model3DIter->second->SetMarked(false);
	};
}

void CWPEProjectController::SetNew3DModelPosition(int id, float xPos, float yPos){
	float x, y, z;
	std::map<int, CWPEState>::iterator stateIter = m_stdmapWPEStates.find(id);
	if (stateIter != m_stdmapWPEStates.end()){
		CWPEState state = stateIter->second;
		this->Get3DCoordFrom2DCoords(xPos, yPos, x, y, z);
		state.m_fXPos = x;
		state.m_fYPos = y;
		state.m_fZPos = z;

		m_stdmapWPEStates.erase(stateIter);
		m_stdmapWPEStates.insert(std::map<int, CWPEState>::value_type(state.m_iId, state));

		std::map<int, CWPE3DModel*>::iterator modelIter = m_stdmap3DModels.find(state.m_iId);
		if (modelIter != m_stdmap3DModels.end()){
			CWPE3DModel *model = modelIter->second;
			model->SetState(state);
		}

		//find the corresponding 3D model
		//set the state to the corresponding 3D Model,
	}
}

void CWPEProjectController::ScaleDown(){
	if (m_pDiagramView != NULL){
		m_pDiagramView->ScaleDown();
	}
}

void CWPEProjectController::ScaleUp(){
	if (m_pDiagramView != NULL){
		m_pDiagramView->ScaleUp();
	}
}

void CWPEProjectController::LoadArealImage(const char *filename){
	if (m_pDiagramView != NULL){
		m_pDiagramView->LoadBackgroundImageAndResize(filename);
		m_pDiagramView->GetCanvasSize(m_fCanvasXSize, m_fCanvasYSize);
	}
}

void CWPEProjectController::LoadTerrainModel(const char *filename){
	CTriangleElevationGrid *triangleGrid = new CTriangleElevationGrid();
	delete m_pElevationGrid;
	m_pElevationGrid = triangleGrid;
	triangleGrid->LoadFile(filename);
	triangleGrid->SortIntoGrid(20, 20);
	
	//NodePtr terrainRoot = triangleGrid->GetRoot();
	//NodePtr sceneRoot = vrScenegraph::getRoot();
	//beginEditCP(sceneRoot, Node::ChildrenFieldMask);{
	//	sceneRoot->addChild(terrainRoot);
	//};endEditCP(sceneRoot);
	////vrScenegraph::update();

	double xMin, xMax, yMin, yMax;
	triangleGrid->GetXYExtension(xMin, yMin, xMax, yMax);
	m_fDiagramTo3DXTranslate = xMin;
	m_fDiagramTo3DYTranslate = yMin;
}

void CWPEProjectController::LoadLandscape(const char *filename){
	NodePtr scene = SceneFileHandler::the().read(filename);
	NodePtr internalRoot = vrScenegraph::getInternalRoot();
	beginEditCP(internalRoot, Node::ChildrenFieldMask);{
		internalRoot->addChild(scene);
	};endEditCP(internalRoot, Node::ChildrenFieldMask);
}

void CWPEProjectController::LoadTypes(const char *filename){
	std::ifstream infile(filename);
	std::string strWord;
	infile >> strWord;
	if (strWord != std::string("WPE_TYPES_INFO")) return;
	char cWord[255];
	infile.getline(&cWord[0], 255, '\n');

	std::string strName;
	float fPilarHeight;
	float fRotorLength;
	float fInfluenceRadius;
	float fMWh;

	do {
		infile >> strName;
		infile >> fPilarHeight;
		infile >> fRotorLength;
		infile >> fInfluenceRadius;
		infile >> fMWh;
		
		CWPETypeInfo typeInfo;
		typeInfo.m_stdstrWPETypeName = strName;
		typeInfo.m_fPilarHeight = fPilarHeight;
		typeInfo.m_fRotorLength = fRotorLength;
		typeInfo.m_fInfluenceRadius = fInfluenceRadius;
		typeInfo.m_fMWh = fMWh;

		this->AddType(typeInfo);
	} while (! infile.eof());
}

void CWPEProjectController::AddType(CWPETypeInfo typeInfo){
	std::string typeName = typeInfo.m_stdstrWPETypeName;
	m_stdmapWPETypes.insert(std::map<std::string, CWPETypeInfo>::value_type(typeName, typeInfo));

	if (m_pAttrPanel){
		m_pAttrPanel->AddTypeNameToTypeNamesCombo(typeName.c_str());
	}
}

CWPETypeInfo CWPEProjectController::GetTypeInfoByName(const char* typeName){
	std::map<std::string, CWPETypeInfo>::iterator typeIter = m_stdmapWPETypes.find(typeName);
	CWPETypeInfo typeInfo;
	if (typeIter != m_stdmapWPETypes.end()){
		typeInfo = typeIter->second;
	}
	return typeInfo;
}

float CWPEProjectController::CalculateEnergyProduction(){
	std::map<int, CWPEState>::iterator stateIter;
	float fMWh = 0.0;
	for (stateIter = m_stdmapWPEStates.begin(); stateIter != m_stdmapWPEStates.end(); stateIter++){
		CWPEState currentWPEState = stateIter->second;
		CWPETypeInfo currentWPEType = GetTypeInfoByName(currentWPEState.m_stdstrWPETypeName.c_str());
		fMWh += currentWPEType.m_fMWh;
	}
	return fMWh;
}

void CWPEProjectController::SetEnergyProduction(){
	float production = CalculateEnergyProduction();
	if (m_pAttrPanel != NULL){
		m_pAttrPanel->SetEnergyProduction(production);
	}
}

void CWPEProjectController::SetNewWindDirection(float direction){
	direction = 180.0 - direction;
	std::map<int, CWPE3DModel*>::iterator modelIter;
	for (modelIter = m_stdmap3DModels.begin(); modelIter != m_stdmap3DModels.end(); modelIter++){
		modelIter->second->SetRotation(direction);
	}
}

void CWPEProjectController::SetRotorRotation(float rotation){
	if (m_bUseRotorRotation){
		std::map<int, CWPE3DModel*>::iterator modelIter;
		for (modelIter = m_stdmap3DModels.begin(); modelIter != m_stdmap3DModels.end(); modelIter++){
			modelIter->second->SetRotorRotation(rotation);
		}
	}
}

void CWPEProjectController::UseRotorRotation(bool use){
	m_bUseRotorRotation = use;
}

void CWPEProjectController::SaveWindparkInfo(const char *filename){
	std::ofstream outFile(filename);

	std::map<int, CWPEState>::iterator stateIter;
	for (stateIter = m_stdmapWPEStates.begin(); stateIter != m_stdmapWPEStates.end(); stateIter++){
		CWPEState currentWPEState = stateIter->second;
		CWPETypeInfo currentWPEType = GetTypeInfoByName(currentWPEState.m_stdstrWPETypeName.c_str());
		outFile << currentWPEState.m_fXPos << " ";
		outFile << currentWPEState.m_fYPos << " ";
		outFile << currentWPEState.m_fZPos << " ";
		outFile << currentWPEState.m_stdstrWPETypeName << std::endl;
	}
	outFile << "Wind energy production: " << this->CalculateEnergyProduction() << std::endl;
}

void CWPEProjectController::RemoveAllWPEs(){
	if (m_pDiagramView != NULL){
		m_pDiagramView->RemoveAllItems();
	}
}

void CWPEProjectController::Get3DCoordFrom2DCoords(float inX, float inY, float &outX, float &outY, float &outZ){
	float x, y, z;
	x = inX;
	y = m_fCanvasYSize - inY;
	x += m_fDiagramTo3DXTranslate;
	y += m_fDiagramTo3DYTranslate;
	z = m_pElevationGrid->GetZ((double) x, (double) y);
	outX = x;
	outY = y;
	outZ = z;
}