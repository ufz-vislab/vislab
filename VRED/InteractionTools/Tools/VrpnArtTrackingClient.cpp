#include "VrpnArtTrackingClient.h"

#include <iostream>
#include <assert.h>

CVrpnArtTrackingClient* CVrpnArtTrackingClient::m_pInstance = NULL;

CVrpnArtTrackingClient::CVrpnArtTrackingClient(){
	int i;
	for (i=0; i<3; i++){
		m_dBodyTranslation[i] = 0.0;
		m_dFlyTranslation[i] = 0.0;
	}
	for (i=0; i<4; i++){
		m_dBodyQuaternion[i] = 0.0;
		m_dFlyQuaternion[i] = 0.0;
	}

	for (i=0; i<10; i++){
		m_dAnalogData[i] = 0.0;
		m_bButtonData[i] = false;
	}

	m_pvrpnAnalog = NULL;
	m_pvrpnTracker = NULL;
	m_pvrpnButtons = NULL;

	m_bTrackingStarted = false;
}


CVrpnArtTrackingClient *CVrpnArtTrackingClient::Instance(){
	if (m_pInstance == NULL){
		m_pInstance = new CVrpnArtTrackingClient();
		return m_pInstance;
	} else {
		return m_pInstance;
	}
}


CVrpnArtTrackingClient::~CVrpnArtTrackingClient(void){
	StopTracking();
}


void CVrpnArtTrackingClient::StartTracking(const char* deviceName){
	m_pvrpnAnalog = new vrpn_Analog_Remote(deviceName);
	m_pvrpnTracker = new vrpn_Tracker_Remote(deviceName);
	m_pvrpnButtons = new vrpn_Button_Remote(deviceName);

	m_pvrpnTracker->register_change_handler(NULL, (vrpn_TRACKERCHANGEHANDLER) CVrpnArtTrackingClient::CBHandleTracker);
	m_pvrpnAnalog->register_change_handler(NULL, (vrpn_ANALOGCHANGEHANDLER) CVrpnArtTrackingClient::CBHandleAnalogs);
	m_pvrpnButtons->register_change_handler(NULL, (vrpn_BUTTONCHANGEHANDLER) CVrpnArtTrackingClient::CBHandleButtons);

	m_bTrackingStarted = true;
}


void CVrpnArtTrackingClient::StopTracking(){
	if (m_pvrpnAnalog){
		m_pvrpnAnalog->unregister_change_handler(NULL, (vrpn_ANALOGCHANGEHANDLER) CVrpnArtTrackingClient::CBHandleAnalogs);
		delete m_pvrpnAnalog;
	}
	if (m_pvrpnTracker){
		m_pvrpnTracker->unregister_change_handler(NULL, (vrpn_TRACKERCHANGEHANDLER) CVrpnArtTrackingClient::CBHandleTracker);
		delete m_pvrpnTracker;
	}
	if (m_pvrpnButtons){
		m_pvrpnButtons->unregister_change_handler(NULL, (vrpn_BUTTONCHANGEHANDLER) CVrpnArtTrackingClient::CBHandleButtons);
		delete m_pvrpnButtons;
	}

	m_pvrpnAnalog = NULL;
	m_pvrpnTracker = NULL;
	m_pvrpnButtons = NULL;

	int i;
	for (i=0; i<3; i++){
		m_dBodyTranslation[i] = 0.0;
		m_dFlyTranslation[i] = 0.0;
	}
	for (i=0; i<4; i++){
		m_dBodyQuaternion[i] = 0.0;
		m_dFlyQuaternion[i] = 0.0;
	}

	for (i=0; i<10; i++){
		m_dAnalogData[i] = 0.0;
		m_bButtonData[i] = false;
	}

	m_bTrackingStarted = false;
}

void CVrpnArtTrackingClient::MainLoop(){
	if (m_bTrackingStarted){
		m_pvrpnTracker->mainloop();
		m_pvrpnAnalog->mainloop();
		m_pvrpnButtons->mainloop();
	} else {
		std::cout << "WARNING: CVrpnArtTrackingClient::MainLoop() has been called but tracking ist not been started!" << std::endl;
	}
}

void CVrpnArtTrackingClient::GetBodyTranslation(double &x, double &y, double &z){
	x = m_dBodyTranslation[0];
	y = m_dBodyTranslation[1];
	z = m_dBodyTranslation[2];

	if (! m_bTrackingStarted){
		std::cout << "WARNING: CVrpnArtTrackingClient::GetBodyTranslation() has been called but tracking has not been started!" << std::endl;
	}
}

void CVrpnArtTrackingClient::GetBodyQuaternion(double &v0, double &v1, double &v2, double &v3){
	v0 = m_dBodyQuaternion[0];
	v1 = m_dBodyQuaternion[1];
	v2 = m_dBodyQuaternion[2];
	v3 = m_dBodyQuaternion[3];

	if (! m_bTrackingStarted){
		std::cout << "WARNING: CVrpnArtTrackingClient::GetBodyQuaternion() has been called but tracking has not been started!" << std::endl;
	}
}

void CVrpnArtTrackingClient::GetFlyTranslation(double &x, double &y, double &z){
	x = m_dFlyTranslation[0];
	y = m_dFlyTranslation[1];
	z = m_dFlyTranslation[2];

	if (! m_bTrackingStarted){
		std::cout << "WARNING: CVrpnArtTrackingClient::GetFlyTranslation() has been called but tracking has not been started!" << std::endl;
	}
}

void CVrpnArtTrackingClient::GetFlyQuaternion(double &v0, double &v1, double &v2, double &v3){
	v0 = m_dFlyQuaternion[0];
	v1 = m_dFlyQuaternion[1];
	v2 = m_dFlyQuaternion[2];
	v3 = m_dFlyQuaternion[3];

	if (! m_bTrackingStarted){
		std::cout << "WARNING: CVrpnArtTrackingClient::GetFlyQuaternion() has been called but tracking has not been started!" << std::endl;
	}
}

double CVrpnArtTrackingClient::GetAnalogData(int index){
	if (index < 10){
		return m_dAnalogData[index];
	}else{
		return 0.0;
	}

	if (! m_bTrackingStarted){
		std::cout << "WARNING: CVrpnArtTrackingClient::GetAnalogData() has been called but tracking has not been started!" << std::endl;
	}
}

bool CVrpnArtTrackingClient::GetButtonData(int index){
	if (index < 10){
		return m_bButtonData[index];
	}else{
		return false;
	}

	if (! m_bTrackingStarted){
		std::cout << "WARNING: CVrpnArtTrackingClient::GetButtonData() has been called but tracking has not been started!" << std::endl;
	}
}

void VRPN_CALLBACK CVrpnArtTrackingClient::CBHandleTracker(void *userdata, const vrpn_TRACKERCB t){
	int i;

	CVrpnArtTrackingClient *art = CVrpnArtTrackingClient::Instance();
	if (art != NULL){
		//std::cout << "sensor: " << t.sensor << std::endl;
		if (t.sensor == 0){
			for (i=0; i<3; i++){
				art->m_dBodyTranslation[i] = t.pos[i];
			} 
			for (i=0; i<4; i++){
				art->m_dBodyQuaternion[i] = t.quat[i];
			}
		} else if (t.sensor == 1){
			for (i=0; i<3; i++){
				art->m_dFlyTranslation[i] = t.pos[i];
				//std::cout << "	" << t.pos[i];
			}
			//std::cout << "	-----	";
			for (i=0; i<4; i++){
				art->m_dFlyQuaternion[i] = t.quat[i];
				//std::cout << t.quat[i] << " ";
			}
			//std::cout << std::endl;
		}
	}
}

void VRPN_CALLBACK CVrpnArtTrackingClient::CBHandleAnalogs(void *, vrpn_ANALOGCB analogData){
	CVrpnArtTrackingClient *art = CVrpnArtTrackingClient::Instance();
	if (art != NULL){
		int numChannels = analogData.num_channel;
		if (numChannels > 10) numChannels = 10;
		for (int i=0; i<numChannels; i++){
			art->m_dAnalogData[i] = analogData.channel[i];
		}
	}
}

void VRPN_CALLBACK CVrpnArtTrackingClient::CBHandleButtons(void *userdata, vrpn_BUTTONCB buttonData){
	CVrpnArtTrackingClient *art = CVrpnArtTrackingClient::Instance();
	if (art != NULL){
		int buttonIndex = buttonData.button;
		if (buttonIndex < 10){
			art->m_bButtonData[buttonIndex] = buttonData.state;
		}
	}
}