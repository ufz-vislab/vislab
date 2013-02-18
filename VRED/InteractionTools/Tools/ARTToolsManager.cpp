#include "ARTToolsManager.h"
#include "ToolHolder.h"

CARTToolsManager *CARTToolsManager::m_pInstance = NULL;

CARTToolsManager::CARTToolsManager(void){
	m_posgUserTransform = NullFC;

	for (int i=0; i<10; i++){
		m_bVRPNButtonData[i] = false;
	}
}

CARTToolsManager::~CARTToolsManager(void){
}

CARTToolsManager *CARTToolsManager::Instance(){
	if (m_pInstance == NULL){
		m_pInstance = new CARTToolsManager();
	}

	return m_pInstance;
}

void CARTToolsManager::SetProjectionCameraUserTransformPtr(osg::TransformPtr trans){
	m_posgUserTransform = trans;
}

void CARTToolsManager::StartVRPNTracking(const char* deviceName){
	CVrpnArtTrackingClient *art = CVrpnArtTrackingClient::Instance();
	if (art->IfTrackingIsStarted()){
		art->StopTracking();
	}

	art->StartTracking(deviceName);
}

void CARTToolsManager::StopVRPNTracking(){
	CVrpnArtTrackingClient::Instance()->StopTracking();
}

void CARTToolsManager::VRPNLoop(){
	int i;
	double x, y, z;
	double v0, v1, v2, v3;
	Quaternion bodyQuaternion;
	Quaternion flyQuaternion;
	Vec3f bodyTranslation;
	Vec3f flyTranslation;
	Matrix m;

	CVrpnArtTrackingClient *art = CVrpnArtTrackingClient::Instance();
	if (! art->IfTrackingIsStarted()) return;

	TransformPtr posgFlyTransform = CToolHolder::Instance()->GetFlystickTransformPtr();

	art->MainLoop();
			
	// Process the changed body tracker data
	art->GetBodyTranslation(x, y, z);
			
	art->GetBodyQuaternion(v0, v1, v2, v3);
	//bodyQuaternion = Quaternion(Vec3f(v0, v1, v2), v3);
	bodyQuaternion.setValueAsQuat(v0, v1, v2, v3);
	bodyTranslation = Vec3f(x, y, z);

	m.setIdentity();
	m.setTranslate(bodyTranslation);
	m.setRotate(bodyQuaternion);

	beginEditCP(m_posgUserTransform, Transform::MatrixFieldMask);{
		m_posgUserTransform->setMatrix(m);
	};endEditCP(m_posgUserTransform, Transform::MatrixFieldMask);

	// Process the changed flystick data
	// Buttons
	// Tracking sensor
	// Flystick-tracking
	art->GetFlyTranslation(x, y, z);
	art->GetFlyQuaternion(v0, v1, v2, v3);
	flyQuaternion.setValueAsQuat(v0, v1, v2, v3);
	flyTranslation = Vec3f(x, y, z);

	m.setIdentity();
	m.setTranslate(flyTranslation);
	m.setRotate(flyQuaternion);

	beginEditCP(posgFlyTransform, Transform::MatrixFieldMask);{
		posgFlyTransform->setMatrix(m);
	};endEditCP(posgFlyTransform, Transform::MatrixFieldMask);

	// Flystick-buttons
	for (i=0; i<10; i++){
		bool value = art->GetButtonData(i);
		if (m_bVRPNButtonData[i] != value){
			m_bVRPNButtonData[i] = value;
			if (i == 0){
				if (value){
					CToolHolder::Instance()->OnTrackingButtonPressed(flyQuaternion, flyTranslation);
				}else{
					CToolHolder::Instance()->OnTrackingButtonReleased(flyQuaternion, flyTranslation);
				}
			} else if (i < 5){
				if (value){
					CToolHolder::Instance()->UseTool(m_stdstrToolNames[i]);
				}
			} else {
				//process further buttons
			}
		} else {
			CToolHolder::Instance()->OnTrackingMove(flyQuaternion, flyTranslation);
		}
	}

	// Analogs
	for (i=0; i<10; i++){
		double value = art->GetAnalogData(i);
		if ((value > 0.001) || (value < -0.001)){
			std::cout << "analog: " << i << "   value: " << value << std::endl;
		}
		if (i == 0){
			CToolHolder::Instance()->OnAnalog0(flyQuaternion, flyTranslation, value);
		}else if (i == 1){
			CToolHolder::Instance()->OnAnalog1(flyQuaternion, flyTranslation, value);
		}
	}
}

void CARTToolsManager::UseTool(int index){
	if ((index >= 0) && (index < 5)){
		CToolHolder::Instance()->UseTool(m_stdstrToolNames[index]);
	}
}