#pragma once

// SpaceNavigatorClient.h
//
//
// Author: Lars Bilke

// comment this out to suppress debug outputs on the console
#define SPACENAVIGATOR_DEBUG_OUTPUT

// ** INCLUDES **
#include <vrpn_Button.h>
#include <vrpn_Analog.h>

// ** SpaceNavigatorClient - CLASS **
// This implements the singleton design pattern.
// ******************************
class SpaceNavigatorClient
{	
public:
	// ** ENUMS **
	// Modes
	enum SpaceNavigatorMode
	{
		TRANSROT = 0,
		TRANS,
		ROT
	};

	// Axes
	enum SpaceNavigatorAxes
	{
		X = 1,
		Y,
		Z,
		rX,
		rY,
		rZ
	};

	// ** Public member fields **

	// state of the up to eight buttons
	bool buttons[8];

	// ** Public member functions **

	// returns the singleton of the SpaceNavigator
	static SpaceNavigatorClient* Instance();

	// Initializes the SpaceNavigator
	// Connects with the server and registers the callback handlers
	// deviceName example: "SpaceNav@viswork01.intern.ufz.de"
	// it is possible to specify the z-axis as the up-axis
	// default the y-axis is the up-axis
	void init(const char *deviceName, SpaceNavigatorAxes axis = Y);

	// get up axis
	bool getZUpAxis();
	void setZUpAxis(bool zUp);

	// enables / disables the axis-domination mode
	// only the axis with the highest value is used
	void setDomination(bool dominating);
	void switchDomination();

	// Switch mode
	void setMode(SpaceNavigatorClient::SpaceNavigatorMode mode);
	void switchMode();

	// set default button behavior
	// on:	left button  --> switch mode
	//		right button --> switch domination
	// off: no button behavior
	void setDefaultButtonBehaviour(bool enable);

	// inverts the specified axis of type SpaceNavigatorClient::SpaceNavigatorAxes
	void invertAxis(SpaceNavigatorAxes axisToInvert);

protected:
	// ** Protected member functions **
	// The constructor is protected because of the singleton
	// design pattern

	// Constructor
	SpaceNavigatorClient();

	// Destructor
	~SpaceNavigatorClient();


	// Does all the event processing
	// this function should be run one time per Frame
	// e.g. in the glut display function
	void mainloop();

	// actual values of the rotation
	float rx, ry, rz;

	// actual values of the translation
	float x, y, z;

private:
	// ** Private member fields **
	vrpn_Button_Remote *_button;
	vrpn_Analog_Remote *_analog;

	// is domination mode active?
	bool _dominating;

	// mode: 0 - translation and rotation
	SpaceNavigatorMode _mode;

	// default button behavior?
	bool _defaultButtonBehaviour;

	// which axis should be inverted?
	float _invertAxes[6];

	// this one points to the class
	// you can use only one SpaceNavigator because it´s static
	// this is needed for the callback methods which only
	// can access static members
	static SpaceNavigatorClient* _spacenavigator;

	// ** Private member functions **

	// Callbacks which are called whenever a button is pressed
	// or the SpaceNavigator is moved
	// Callbacks as class members have to be static
	static void CALLBACK _handleButtons(void *, vrpn_BUTTONCB buttonData);
	static void CALLBACK _handleAnalogs(void *, vrpn_ANALOGCB analogData);
	// which is the up-axis (y - normal, z - from the gis world)
	SpaceNavigatorAxes _upAxis;

};
