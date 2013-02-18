#include "ProjectControllerLoader.h"

CProjectControllerLoader::CProjectControllerLoader(void){
}

CProjectControllerLoader::~CProjectControllerLoader(void){
}

bool CProjectControllerLoader::AllDataComplete(){
	int counter = 0;
	if (m_stdstrTerrainFilename.empty()) counter++;
	if (m_stdstrWPETypesFilename.empty()) counter++;
	if (m_stdstrArealImageFilename.empty()) counter++;
	if (m_stdstrWPE3DModelFilename.empty()) counter++;

	return (counter == 0);
}

void CProjectControllerLoader::LoadAll(){
	if (AllDataComplete()){
		CWPEProjectController *c = CWPEProjectController::Instance();
		c->LoadTerrainModel(m_stdstrTerrainFilename.c_str());
		c->LoadTypes(m_stdstrWPETypesFilename.c_str());
		c->LoadArealImage(m_stdstrArealImageFilename.c_str());
		c->Set3DModelFilename(m_stdstrWPE3DModelFilename.c_str());
	}
}


