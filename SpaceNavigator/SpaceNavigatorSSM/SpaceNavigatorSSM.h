#pragma once

// SpaceNavigatorSSM.h
//
//
// Author: Lars Bilke

// ** INCLUDES **
#include <OpenSG/OSGGLUT.h>
#include <OpenSG/OSGSimpleSceneManager.h>
#include <OpenSG/OSGSimpleMaterial.h>
#include <OpenSG/OSGMaterialGroup.h>
#include <OpenSG/OSGComponentTransform.h>
#include <OpenSG/OSGNode.h>
#include <OpenSG/OSGSimpleAttachments.h>
#include "../SpaceNavigator/SpaceNavigatorOSGClient.h"
#include "../TriangleElevationGrid/TriangleElevationGrid.h"

OSG_USING_NAMESPACE

// ** SpaceNavigatorSSM - CLASS **
// ******************************
class SpaceNavigatorSSM : public SimpleSceneManager
{
public:
	// ** Public member fields **

	// ** Public member functions **

	// Constructor
	// deviceName: the device to connect to
	// zUpVector: sets the z-axis as up-axis, default: y-axis
	SpaceNavigatorSSM(char *deviceName, bool zUpAxis = false);

	// set root of the displayed tree (overridden)
	void setRoot(NodePtr root);

	// show the whole scene (overridden)
	void showAll(void);

	// returns the Space Navigator
	SpaceNavigatorOSGClient* getSpaceNavigator();

	// process Space Navigator inputs and update camera position
	void updateCameraAndMovement();

	// set camera position
	void setCameraPosition(Pnt3f position);

	// enable / disable object picking
	void setObjectPicking(bool enable);
	void switchObjectPicking();

	// handles mouse clicks (overridden)
	void mouseButtonPress(UInt16 button, Int16 x, Int16 y);

	// sets the default walk navigator ground collision detection
	void initWalkNavGroundCollision(NodePtr groundNode);

	// sets the elevation grid
	void initElevationGrid(char *gridFile, int numXCells = 10, int numYCells = 10);

	// sets height over ground
	void setGroundDistance(float distance);

	// ** Helper Functions **

	// returns the node with the specified name
	NodePtr getNodeByName(NodePtr rootNode, const Char8 *nodeName);

	// prints the matrix on the console
	void printMatrix(Matrix m);

protected:
	// ** Protected member functions **

	// Init the scene manager (overridden)
	void SpaceNavigatorSSM::initialize();

private:
	// ** Private member fields **

	// distance to ground
	float _groundDistance;

	// is object picking active?
	bool _objectPicking;

	// the transformation core of the picked object
	NodePtr _pickedObjectNode;

	// a higher transformation core of the picked object
	NodePtr _pickedObjectNodeHigherTransform;

	// the elevation grid
	CElevationGridBase *_grid;

	// is an elevation grid used or the default walk navigator
	bool _useElevationGrid;
};