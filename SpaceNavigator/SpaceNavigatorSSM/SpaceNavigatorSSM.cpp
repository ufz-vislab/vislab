// SpaceNavigatorClient.cpp
//
//
// Author: Lars Bilke

// ** INCLUDES **
#include "SpaceNavigatorSSM.h"

OSG_USING_NAMESPACE

SpaceNavigatorSSM::SpaceNavigatorSSM(char *deviceName, bool zUpAxis)
{
	SpaceNavigatorOSGClient* spaceNav = SpaceNavigatorOSGClient::Instance();
	// Init the Space Navigator
	if(zUpAxis)
		spaceNav->init(deviceName, SpaceNavigatorClient::Z);
	else
		spaceNav->init(deviceName);

	_groundDistance = 2.0f;

	spaceNav->setTranslationFactor(1.f);
	spaceNav->setRotationFactor(1.f);

	spaceNav->setHeightControl(false);

	_objectPicking = true;

	_grid = NULL;

	_useElevationGrid = false;
}

void SpaceNavigatorSSM::setRoot(NodePtr root)
{
	// don t know if correct behavior
	this->initialize();

	SimpleSceneManager::setRoot(root);

	// set up axis
	if(SpaceNavigatorOSGClient::Instance()->getZUpAxis())
	{
		this->getNavigator()->setUp(Vec3f(0, 0, 1));
	}
}

void SpaceNavigatorSSM::showAll(void)
{
	if(_root == NullFC)
	    return;
	
	_root->updateVolume();
	
	Vec3f min,max;
	_root->getVolume().getBounds( min, max );
	Vec3f d = max - min;
	
	if(d.length() < Eps) // Nothing loaded? Use a unity box
	{
	    min.setValues(-1.f,-1.f,-1.f);
	    max.setValues( 1.f, 1.f, 1.f);
	    d = max - min;
	}

	SpaceNavigatorOSGClient::Instance()->setTranslationFactorToSceneSize(_root);

	bool zUpAxis = SpaceNavigatorOSGClient::Instance()->getZUpAxis();
	
	Real32 dist;
	if(zUpAxis)
		dist = osgMax(d[0],d[2]) / (2 * osgtan(_camera->getFov() / 2.f));
	else
		dist = osgMax(d[0],d[1]) / (2 * osgtan(_camera->getFov() / 2.f));
	
	// get the correct up axis
	Vec3f up = getNavigator()->getUp();
	Pnt3f at;
	if(zUpAxis)
		at = Pnt3f((min[0] + max[0]) * .5f,(min[2] + max[2]) * .5f,(min[1] + max[1]) * .5f);
	else
		at = Pnt3f((min[0] + max[0]) * .5f,(min[1] + max[1]) * .5f,(min[2] + max[2]) * .5f);
	Pnt3f from=at;
	if(zUpAxis)
		from[1]+=(dist+fabs(max[1]-min[1])*0.5f); 
	else
		from[2]+=(dist+fabs(max[2]-min[2])*0.5f); 
	
	_navigator.set(from,at,up);
	
	// set the camera to go from 1% of the object to twice its size
	Real32 diag = osgMax(osgMax(d[0], d[1]), d[2]);
	beginEditCP(_camera);
	_camera->setNear (diag / 100.f);
	_camera->setFar  (10 * diag);
	endEditCP(_camera);
}

void SpaceNavigatorSSM::initialize()
{
	SimpleSceneManager::initialize();

	//  set walk mode
	this->getNavigator()->setMode(Navigator::WALK);
}

void SpaceNavigatorSSM::mouseButtonPress(UInt16 button, Int16 x, Int16 y)
{
	switch (button)
	{
		case MouseLeft:
			// test if an object is picked
			if(_objectPicking)
			{
				Line ray = calcViewRay(x, y);
				IntersectAction *iAct = IntersectAction::create();
				iAct->setLine(ray);
				iAct->apply(this->getRoot());

				// we have a hit
				if(iAct->didHit())
				{
					_pickedObjectNode = iAct->getHitObject();
					#ifdef SPACENAVIGATOR_DEBUG_OUTPUT
					std::cout << "SpaceNavigatorSSM: Object transformation mode active ( " << getName(_pickedObjectNode) << " )" << std::endl;
					#endif // SPACENAVIGATOR_DEBUG_OUTPUT

					// go up in the graph to the next transformation
					while(!_pickedObjectNode->getCore()->getType().isDerivedFrom(Transform::getClassType()))
					{
						if(_pickedObjectNode->getParent() != this->getRoot())
							_pickedObjectNode = _pickedObjectNode->getParent();
						else
						{
							// insert a new transformation node
							NodePtr pickedObject = iAct->getHitObject();
							TransformPtr newTransform = Transform::create();
							Matrix m;
							m.setIdentity();
							beginEditCP(newTransform, Transform::MatrixFieldMask);
								newTransform->setMatrix(m);
							endEditCP(newTransform, Transform::MatrixFieldMask);

							NodePtr newTransformNode = Node::create();

							beginEditCP(newTransformNode, Node::CoreFieldMask);
								newTransformNode->setCore(newTransform);
							endEditCP(newTransformNode, Node::CoreFieldMask);
							
							NodePtr pickedObjectParent = pickedObject->getParent();

							// add reference because reCount would be 0 and then it will
							// be deleted
							addRefCP(pickedObject);

							beginEditCP(pickedObjectParent);
								pickedObjectParent->replaceChildBy(pickedObject, newTransformNode);
							endEditCP(pickedObjectParent);

							beginEditCP(newTransformNode);
								newTransformNode->addChild(pickedObject);
							endEditCP(newTransformNode);
							
							// sub the reference which was added before
							subRefCP(pickedObject);

							_pickedObjectNode = newTransformNode;
						}
					}

					// a transformation was found and the objects bounding box is showed
					this->setHighlight(_pickedObjectNode);
				}
			}
			_navigator.buttonPress(Navigator::LEFT_MOUSE,x,y);
			break;

		case MouseMiddle: 
			_navigator.buttonPress(Navigator::MIDDLE_MOUSE,x,y);
			break;

		case MouseRight:
			// release picked object and switch off bounding box rendering
			if(_objectPicking)
			{
				_pickedObjectNode = NullFC;
				this->setHighlight(NullFC);
				#ifdef SPACENAVIGATOR_DEBUG_OUTPUT
					std::cout << "SpaceNavigatorSSM: Camera transformation mode active" << std::endl;
				#endif // SPACENAVIGATOR_DEBUG_OUTPUT
			}
			_navigator.buttonPress(Navigator::RIGHT_MOUSE,x,y);
			break;

		case MouseUp:     
			_navigator.buttonPress(Navigator::UP_MOUSE,x,y);
			break;

		case MouseDown:    
			_navigator.buttonPress(Navigator::DOWN_MOUSE,x,y);
			break;
		}

		_mousebuttons |= 1 << button;
		_lastx = x;
		_lasty = y;
}

void SpaceNavigatorSSM::initWalkNavGroundCollision(NodePtr groundNode)
{
	NodePtr nullNode = Node::create();
	GeometryPtr nullGeo = Geometry::create();
	beginEditCP(nullNode);
		nullNode->setCore(nullGeo);
	endEditCP(nullNode);
	getNavigator()->getWalkNavigator()->setWorld(nullNode);
	getNavigator()->getWalkNavigator()->setGround(groundNode);

	_useElevationGrid = false;
}

void SpaceNavigatorSSM::initElevationGrid(char *gridFile, int numXCells, int numYCells)
{
	// disable walk nav default ground collision
	NodePtr nullNode = Node::create();
	GeometryPtr nullGeo = Geometry::create();
	beginEditCP(nullNode);
		nullNode->setCore(nullGeo);
	endEditCP(nullNode);
	getNavigator()->getWalkNavigator()->setWorld(nullNode);
	getNavigator()->getWalkNavigator()->setGround(nullNode);

	// load grid
	CTriangleElevationGrid* grid = new CTriangleElevationGrid();
	grid->LoadFile(gridFile);
	grid->SortIntoGrid(numXCells, numYCells);
	_grid = grid;
	
	_useElevationGrid = true;
}

void SpaceNavigatorSSM::setGroundDistance(float distance)
{
	_groundDistance = distance;
	getNavigator()->getWalkNavigator()->setGroundDistance(distance);
}

SpaceNavigatorOSGClient* SpaceNavigatorSSM::getSpaceNavigator()
{
	return SpaceNavigatorOSGClient::Instance();
}

void SpaceNavigatorSSM::updateCameraAndMovement()
{	
	// get space navigator and update it
	SpaceNavigatorOSGClient* spaceNavigator = SpaceNavigatorOSGClient::Instance();
	spaceNavigator->update();

	// get walk navigator
	WalkNavigator* wnav = _navigator.getWalkNavigator();

	switch(this->getNavigator()->getMode())
	{
	// walk mode
	case Navigator::WALK:

		// transform picked object
		if(_pickedObjectNode != NullFC)
		{
			/*
			// some dummy variables necessary for Node::getToWorld()
			// only the quaternion rotation is used
			Vec3f dummy1, dummy2;
			Quaternion rotation, dummy3;

			// get the camera rotation in world coordinates
			Matrix camToWorld = getCamera()->getBeacon()->getToWorld();
			camToWorld.getTransform(dummy1, rotation, dummy2, dummy3);
			camToWorld.setIdentity();
			camToWorld.setRotate(rotation);

			// get the transpose of the old object rotation in world coordinates
			Matrix objectToWorld = _pickedObjectNode->getToWorld();
			objectToWorld.getTransform(dummy1, rotation, dummy2, dummy3);
			objectToWorld.setIdentity();
			objectToWorld.setRotate(rotation);	
			objectToWorld.transpose();

			// build the translation vector from the SpaceNavigator input
			Vec3f dv;
			if(_zUpAxis)
				dv.setValues(spaceNavigator->x * transFactor, spaceNavigator->z * transFactor, spaceNavigator->y * transFactor);
			else
				dv.setValues(spaceNavigator->x * transFactor, spaceNavigator->y * transFactor, spaceNavigator->z * transFactor);
			Vec3f dvworld;
			Vec3f dvobject;

			// get the camera to object transformation (only rotation) through multiplying
			// the transposed object to world-rotation with inversed transposed camera to
			// world-rotation
			Matrix camToObject;
			camToObject.setIdentity();
			camToObject.mult(objectToWorld);
			camToObject.mult(camToWorld);
			
			// get translation vector in object coordinates
			camToObject.mult(dv, dvobject);

			// scale should have no effect on translation sensitivity
			dvobject.setValues(dvobject[0] / dummy2[0], dvobject[1] / dummy2[1], dvobject[2]/ dummy2[2]);

			// define the rotation axes in camera coordinates
			Vec3f cameraRE1(1, 0, 0);
			Vec3f cameraRE2(0, 1, 0);
			Vec3f cameraRE3(0, 0, 1);

			// compute the rotation axes in object space through transforming the axes
			// in camera space width camera to object transformation.
			// then a quaternion is created with the transformed axis and value from
			// the SpaceNavigator data
			Vec3f objectRE1;
			Vec3f objectRE2;
			Vec3f objectRE3;
			camToObject.mult(cameraRE1, objectRE1);
			camToObject.mult(cameraRE2, objectRE2);
			camToObject.mult(cameraRE3, objectRE3);
			Quaternion qx(objectRE1, spaceNavigator->rx * rotFactorObject);
			Quaternion qy, qz;
			if(_zUpAxis)
			{
				qy.setValueAsAxisRad(objectRE2, spaceNavigator->rz * rotFactorObject);
				qz.setValueAsAxisRad(objectRE3, spaceNavigator->ry * rotFactorObject);
			}
			else
			{
				qy.setValueAsAxisRad(objectRE2, spaceNavigator->ry * rotFactorObject);
				qz.setValueAsAxisRad(objectRE3, spaceNavigator->rz * rotFactorObject);
			}


			// get the old rotation from the picked object and multiply it with the
			// three quaternions built from the SpaceNavigator data
			Matrix transform = (TransformPtr::dcast(_pickedObjectNode->getCore()))->getMatrix();
			transform.getTransform(dummy1, rotation, dummy2, dummy3);
			rotation.mult(qx);
			rotation.mult(qy);
			rotation.mult(qz);

			// Build the final transformation matrix
			// old transformation * new translation * new rotation (in object coordinates)
			Matrix m;
			m.identity();
			m.setTranslate(dvobject);
			transform.mult(m);
			transform.setRotate(rotation);
			
			// set transformation for ComponentTransforms
			if(_pickedObjectNode->getCore()->getType().isDerivedFrom(ComponentTransform::getClassType()))
			{
				// set translation and rotation separately
				ComponentTransformPtr compTrans = ComponentTransformPtr::dcast(_pickedObjectNode->getCore());
				beginEditCP(compTrans);
					compTrans->setTranslation(Vec3f(transform[3][0], transform[3][1], transform[3][2]));	
					compTrans->setRotation(rotation);
				endEditCP(compTrans);
			}
			// set transformation for normal Transforms
			else if(_pickedObjectNode->getCore()->getType().isDerivedFrom(Transform::getClassType()))
			{
				// set final matrix
				TransformPtr transCore = TransformPtr::dcast(_pickedObjectNode->getCore());
				beginEditCP(transCore, Transform::MatrixFieldMask);
					transCore->setMatrix(transform);
				endEditCP(transCore, Transform::MatrixFieldMask);
			}
			*/
		}
		// camera movement
		else
		{
			// update the camera orientation and position	
			Vec3f trans = spaceNavigator->getTranslation();
			Vec3f rot = spaceNavigator->getRotation();
			
			if(spaceNavigator->getZUpAxis())
			{
				wnav->rotate(-rot.z(), -rot.x());
				wnav->forward(trans.y());
			}
			else
			{
				wnav->rotate(-rot.y(), -rot.x());
				wnav->forward(trans.z());
			}
			wnav->right(-trans.x());

			// update the distance to the ground
			if(spaceNavigator->getHeightControl())
			{
				if(spaceNavigator->getZUpAxis())
					_groundDistance += trans.z();
				else
					_groundDistance += trans.y();
				
				// make sure we never get below the ground
				if(_groundDistance < 0.1f) _groundDistance = 0.1f;
				wnav->setGroundDistance(_groundDistance);
			}

			if(_useElevationGrid)
			{
				Pnt3f pos = getNavigator()->getFrom();
				if(spaceNavigator->getZUpAxis())
					setCameraPosition(Pnt3f(pos[0], pos[1], _grid->GetZ(pos[0], pos[1]) + _groundDistance));
				else
					setCameraPosition(Pnt3f(pos[0], _grid->GetZ(pos[0], pos[2]) + _groundDistance, pos[2]));
			}

		}
		break;

	// normal trackball mode
	case Navigator::TRACKBALL:

		break;
	}
}


void SpaceNavigatorSSM::setObjectPicking(bool enable)
{
	_objectPicking = enable;
}

void SpaceNavigatorSSM::switchObjectPicking()
{
	_objectPicking = !_objectPicking;
}


void SpaceNavigatorSSM::setCameraPosition(Pnt3f position)
{
	Navigator* nav = getNavigator();

	// set new look at point = old look at + new Position - old Position
	nav->setAt(nav->getAt() + position - nav->getFrom());

	// set new position
	nav->setFrom(position);
}

// ** Helper functions **

NodePtr SpaceNavigatorSSM::getNodeByName(NodePtr rootNode, const Char8 *nodeName)
{
	if (getName(rootNode))
	{
		if (getName(rootNode) == std::string(nodeName))
			return rootNode;
	}

	for (int i = 0; i < rootNode->getNChildren(); i++)
	{
		NodePtr r = getNodeByName(rootNode->getChild(i), nodeName);
	
		if (r != NullFC)
			return r;
	}

	return NullFC;
}

void SpaceNavigatorSSM::printMatrix(Matrix m){
	std::cout << "  " << m[0][0] << " " << m[1][0] << " " << m[2][0] << " " << m[3][0] << std::endl;
	std::cout << "  " << m[0][1] << " " << m[1][1] << " " << m[2][1] << " " << m[3][1] << std::endl;
	std::cout << "  " << m[0][2] << " " << m[1][2] << " " << m[2][2] << " " << m[3][2] << std::endl;
	std::cout << "  " << m[0][3] << " " << m[1][3] << " " << m[2][3] << " " << m[3][3] << std::endl << std::endl;
}