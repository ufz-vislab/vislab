// SpaceNavigatorClient.cpp
//
//
// Author: Lars Bilke

// ** INCLUDES **
#include "SpaceNavigatorOSGClient.h"

#include "gl/glut.h"

#include <iostream>

using namespace std;


SpaceNavigatorOSGClient* SpaceNavigatorOSGClient::_singleton = 0;

Vec3f SpaceNavigatorOSGClient::getTranslation()
{
	return Vec3f(x, y, z) * _frameTranslationFactor;
}

Vec3f SpaceNavigatorOSGClient::getRotation()
{
	return Vec3f(rx, ry, rz) * _frameRotationFactor;
}
void SpaceNavigatorOSGClient::setHeightControl(bool enable)
{
	_heightControl = enable;
}

void SpaceNavigatorOSGClient::switchHeightControl()
{
	_heightControl = !_heightControl;
}

void SpaceNavigatorOSGClient::setTranslationFactor(float factor)
{
	_translationFactor = factor;
}

void SpaceNavigatorOSGClient::setRotationFactor(float factor)
{
	_rotationFactor = factor;
}

void SpaceNavigatorOSGClient::setTranslationFactorToSceneSize( NodePtr root, float additionalScaling /*= 1.0f*/ )
{
	if(root == NullFC)
		return;

	root->updateVolume();

	Vec3f min,max;
	root->getVolume().getBounds( min, max );
	Vec3f d = max - min;

	if(d.length() < Eps) // Nothing loaded? Use a unity box
	{
		min.setValues(-1.f,-1.f,-1.f);
		max.setValues( 1.f, 1.f, 1.f);
		d = max - min;
	}

	// adjust the translation factors so that motions are sort of scaled
	_translationFactor = ((d[0] + d[1] + d[2]) / 10.f) * additionalScaling;
}

void SpaceNavigatorOSGClient::update()
{
	int newElapsedTime = glutGet(GLUT_ELAPSED_TIME);
	int frameTime = (newElapsedTime - _elapsedTime);	// in ms
	_elapsedTime = newElapsedTime;

	_frameTranslationFactor =  _translationFactor * (frameTime / 1000.0);
	_frameRotationFactor = _rotationFactor * (frameTime / 800.0);

	// call the Space Navigator mainloop
	SpaceNavigatorClient::mainloop();
}

bool SpaceNavigatorOSGClient::getHeightControl()
{
	return _heightControl;
}

SpaceNavigatorOSGClient::SpaceNavigatorOSGClient()
{
	SpaceNavigatorClient::Instance();
}

SpaceNavigatorOSGClient::~SpaceNavigatorOSGClient()
{

}

SpaceNavigatorOSGClient* SpaceNavigatorOSGClient::Instance()
{
	if(_singleton == 0)
		_singleton = new SpaceNavigatorOSGClient();

	return _singleton;
}