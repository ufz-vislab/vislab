#ifndef VRPN_3DCONNEXION_H
#define VRPN_3DCONNEXION_H

#pragma warning (disable : 4996)

#include "vrpn_HumanInterface.h"
#include "vrpn_Button.h"
#include "vrpn_Analog.h"

#ifdef  VRPN_USE_HID

// Device drivers for the 3DConnexion SpaceNavigator and SpaceTraveler
// devices, connecting to them as HID devices.

// Exposes two VRPN device classes: Button and Analog.
// Analogs are mapped to the six channels, each in the range (-1..1).

// This is the base driver for the two devices.  The Navigator has
// only two buttons and has product ID 50726, the traveler has 8
// buttons and ID 50723.  The derived classes just construct with
// the appropriate number of buttons and an acceptor for the proper
// product ID; the baseclass does all the work.

// Changed: 22.01.08, Bilke
// The pilot has ID 0xc625 and 21 buttons and the explorer has ID 0xc627
// and 15 buttons.
// Added support for the SpacePilot (tested) and the SpaceExplorer (not tested).
// There are only the first 8 buttons availlable.

class vrpn_3DConnexion: public vrpn_Button, public vrpn_Analog, protected vrpn_HidInterface {
public:
  vrpn_3DConnexion(vrpn_HidAcceptor *filter, unsigned num_buttons,
                   const char *name, vrpn_Connection *c = 0);
  virtual ~vrpn_3DConnexion();

  virtual void mainloop();

  virtual void reconnect();

protected:
  // Set up message handlers, etc.
  void on_data_received(size_t bytes, vrpn_uint8 *buffer);

  virtual void decodePacket(size_t bytes, vrpn_uint8 *buffer);
  struct timeval _timestamp;
  vrpn_HidAcceptor *_filter;

  // Send report iff changed
  void report_changes (vrpn_uint32 class_of_service = vrpn_CONNECTION_LOW_LATENCY);
  // Send report whether or not changed
  void report (vrpn_uint32 class_of_service = vrpn_CONNECTION_LOW_LATENCY);
  // NOTE:  class_of_service is only applied to vrpn_Analog
  //  values, not vrpn_Button or vrpn_Dial
};

class vrpn_3DConnexion_Navigator: public vrpn_3DConnexion {
public:
  vrpn_3DConnexion_Navigator(const char *name, vrpn_Connection *c = 0);
  virtual ~vrpn_3DConnexion_Navigator() {};

protected:
};

class vrpn_3DConnexion_Traveler: public vrpn_3DConnexion {
public:
  vrpn_3DConnexion_Traveler(const char *name, vrpn_Connection *c = 0);
  virtual ~vrpn_3DConnexion_Traveler() {};

protected:
};

// Changed: 22.01.08, Bilke
class vrpn_3DConnexion_Pilot: public vrpn_3DConnexion {
public:
  vrpn_3DConnexion_Pilot(const char *name, vrpn_Connection *c = 0);
  virtual ~vrpn_3DConnexion_Pilot() {};

protected:
};

// Changed: 22.01.08, Bilke
class vrpn_3DConnexion_Explorer: public vrpn_3DConnexion {
public:
  vrpn_3DConnexion_Explorer(const char *name, vrpn_Connection *c = 0);
  virtual ~vrpn_3DConnexion_Explorer() {};

protected:
};

// end of VRPN_USE_HID
#endif

// end of VRPN_3DCONNEXION_H
#endif

