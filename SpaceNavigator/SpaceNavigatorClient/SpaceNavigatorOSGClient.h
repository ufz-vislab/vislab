#pragma once

// SpaceNavigatorOSGClient.h
//
//
// Author: Lars Bilke

// ** INCLUDES **
#include "SpaceNavigatorClient.h"

#include "OpenSG/OSGNode.h"

OSG_USING_NAMESPACE

// ** SpaceNavigatorOSGClient - CLASS **
// ******************************
class SpaceNavigatorOSGClient : public SpaceNavigatorClient
{
public:

	// returns the singleton of the SpaceNavigatorOSGClient
	static SpaceNavigatorOSGClient* Instance();

	// returns the translation values
	Vec3f getTranslation();

	// returns the rotation values
	Vec3f getRotation();

	// updates the translation and rotation values
	// must be called once per frame before getTranslation/Rotation
	void update();

	// enable / disable height control
	void setHeightControl(bool enable);
	bool getHeightControl();
	void switchHeightControl();

	// set translation factor
	void setTranslationFactor(float factor);
	void setTranslationFactorToSceneSize(NodePtr root, float additionalScaling = 1.0f);

	// set rotation factor
	void setRotationFactor(float factor);


protected:
	SpaceNavigatorOSGClient();
	~SpaceNavigatorOSGClient();
private:

	// the translation factor
	float _translationFactor;

	// the rotation factor
	float _rotationFactor;

	float _frameTranslationFactor;
	float _frameRotationFactor;

	// is the z-axis the up-axis
	bool _zUpAxis;

	// control height over ground?
	bool _heightControl;

	// elapsed time since glutInit
	int _elapsedTime;

	static SpaceNavigatorOSGClient* _singleton;
};