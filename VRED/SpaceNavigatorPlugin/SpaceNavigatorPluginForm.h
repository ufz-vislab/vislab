/********************************************************************
	created:	6.5.2009   10:27
	filename: 	e:\bilke\apps\vred-2.5\dev\CityGeneratorPlugin\PluginForm.h
	author:		Lars Bilke
	
	purpose:	Definition of the CityGenerator plug in form
*********************************************************************/
#ifndef PLUGINFORM_HEADER
#define PLUGINFORM_HEADER

// ** INCLUDES **
#include "SpaceNavigatorPluginFormBase.h"
#include "TriangleElevationGrid.h"
#include <string>

#include "vrAEBase.h"

class QString;

using namespace std;
OSG_USING_NAMESPACE

/**
 * SpaceNavigatorPluginForm
 */
class SpaceNavigatorPluginForm : public SpaceNavigatorPluginFormBase, vrAEBase
{
	Q_OBJECT

public:
	// ** Public member fields **
	
	// ** Public member functions **
	SpaceNavigatorPluginForm( QWidget* parent = 0, const char* name = 0, WFlags fl = 0);

	~SpaceNavigatorPluginForm();

	void loop();
	void SetConnectString(const char* connectString);
	void ElevGridLoadFileCL(const char* file);

	// ** Public slots **
	public slots:
		void CheckVRPNDeviceTextEditInput(const QString& text);
		void ConnectToDevice();
		void SetEnableSpaceNavigator(bool enable);
		void SetTranslationFactor(float value);
		void SetRotationFactor(float value);
		void SetDominationMode(bool enable);
		void SetNavigationMode(int mode);
		void SetEnableOrbitMode(bool enable);
		void SetEnableHeightControl(bool enable);
		void ElevGridLoadFile();
		void ElevGridLoadSelected();
		void SetEnableElevGrid(bool enable);

protected:
	// ** Protected member fields **

	// ** Protected member functions **
private:
	// ** Private member fields **
	bool _isOrbitMode;
	bool _isElevGridEnabled;
	float _heightOverZero;
	CTriangleElevationGrid _elevGrid;

	// ** Private member functions **
	void WalkNavigatorForward(float step, Pnt3f& oldFrom, Pnt3f& oldAt, Vec3f& oldUp);
	void WalkNavigatorRight(float step, Pnt3f& oldFrom, Pnt3f& oldAt, Vec3f& oldUp);
	void WalkNavigatorRotate(float deltaX, float deltaY, Pnt3f& oldFrom, Pnt3f& oldAt, Vec3f& oldUp);
};

#endif // PLUGINFORM_HEADER

