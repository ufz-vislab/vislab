#pragma once

#include <vrpn_Button.h>
#include <vrpn_Analog.h>
#include <vrpn_Tracker.h>

class CVrpnArtTrackingClient
{
protected:
	CVrpnArtTrackingClient();
public:
	static CVrpnArtTrackingClient *Instance();
	~CVrpnArtTrackingClient(void);

	void	StartTracking(const char* deviceName);
	void	StopTracking();
	bool	IfTrackingIsStarted(){return m_bTrackingStarted;}

	void	MainLoop();

	void	GetBodyTranslation(double &x, double &y, double &z);
	void	GetBodyQuaternion(double &v0, double &v1, double &v2, double &v3);

	void	GetFlyTranslation(double &x, double &y, double &z);
	void	GetFlyQuaternion(double &v0, double &v1, double &v2, double &v3);

	double	GetAnalogData(int index);
	bool	GetButtonData(int index);

protected:
	// The instance
	static CVrpnArtTrackingClient *m_pInstance;

	// Is the tracker initialized ?
	bool	m_bTrackingStarted;

	// Tracking values
	double	m_dBodyQuaternion[4];
	double	m_dBodyTranslation[3];

	// Flystick
	double	m_dFlyQuaternion[4];
	double	m_dFlyTranslation[3];

	// Analogs
	double	m_dAnalogData[10];

	// Buttons
	bool	m_bButtonData[10];

	// VRPN related stuff
	vrpn_Analog_Remote *m_pvrpnAnalog;
	vrpn_Tracker_Remote *m_pvrpnTracker;
	vrpn_Button_Remote *m_pvrpnButtons;

	static void VRPN_CALLBACK CBHandleTracker(void *userdata, const vrpn_TRACKERCB t);
	static void VRPN_CALLBACK CBHandleAnalogs(void *userdata, vrpn_ANALOGCB analogData);
	static void VRPN_CALLBACK CBHandleButtons(void *userdata, vrpn_BUTTONCB buttonData);
};