// SpaceNavigatorTestSSM.cpp
//
// In this program you control the camera with the Space Navigator
// and navigate through a city scene.
// The SpaceNavigatorSSM class is used.
// 

#include <string>
#include <iostream>

#include <OpenSG/OSGGLUT.h>
#include <OpenSG/OSGConfig.h>
#include <OpenSG/OSGSimpleGeometry.h>
#include <OpenSG/OSGGLUTWindow.h>
#include <OpenSG/OSGSceneFileHandler.h>
#include <OpenSG/OSGSimpleStatisticsForeground.h>

#include "../SpaceNavigatorSSM/SpaceNavigatorSSM.h"


OSG_USING_NAMESPACE

SpaceNavigatorSSM *mgr;

int setupGLUT( int *argc, char *argv[] );

// Initialize GLUT & OpenSG and set up the scene
int main(int argc, char **argv)
{
	NodePtr scene;

    // OSG init
    osgInit(argc,argv);

    // GLUT init
    int winid = setupGLUT(&argc, argv);

    // the connection between GLUT and OpenSG
    GLUTWindowPtr gwin= GLUTWindow::create();
    gwin->setId(winid);
    gwin->init();

    // create the new scene manager
	// get connection string from command line
	char* connectionString;
	if(argc > 1)
		connectionString = argv[1];
    

	// get scene filename
	std::string filename;
	if (argc > 2)
		scene = SceneFileHandler::the().read(argv[2]);
	else
		scene = SceneFileHandler::the().read("../osg/City_with_ground.osb");

	// check for z-up
	bool zUp = false;
	if (argc > 3)
	{
		std::string temp = argv[3];
		if (temp == "-z")
			zUp = true;
	}
	
	mgr = new SpaceNavigatorSSM(connectionString, zUp);
	
    // tell the manager what to manage
    mgr->setWindow(gwin );
	mgr->setRoot  (scene);
	
	// setup the walk navigator
	mgr->initWalkNavGroundCollision(mgr->getNodeByName(scene, "ground"));
	mgr->setGroundDistance(5.0f);

    // show the whole scene
	mgr->showAll();
	if (zUp)
		mgr->setCameraPosition(Vec3f(0, 0, 30));
	else
		mgr->setCameraPosition(Vec3f(0, 30, 0));

    // GLUT main loop
    glutMainLoop();

    return 0;
}

//
// GLUT callback functions
//

// redraw the window"
void display(void)
{
	mgr->updateCameraAndMovement();
    mgr->redraw();
}

// react to size changes
void reshape(int w, int h)
{
    mgr->resize(w, h);
    glutPostRedisplay();
}

// react to mouse button presses
void mouse(int button, int state, int x, int y)
{
	
    if (state)
        mgr->mouseButtonRelease(button, x, y);
    else
        mgr->mouseButtonPress(button, x, y);
        
    glutPostRedisplay();
	
}

// react to mouse motions with pressed buttons
void motion(int x, int y)
{
    mgr->mouseMove(x, y);
    glutPostRedisplay();
}

// react to keys
void keyboard(unsigned char k, int x, int y)
{
    switch(k)
    {
	case 't':
		mgr->setNavigationMode(Navigator::TRACKBALL);
		break;
	case 'w':
		mgr->setNavigationMode(Navigator::WALK);
		mgr->getNavigator()->setUp(Vec3f(0, 1, 0));
		break;
	case 'h':
		mgr->getSpaceNavigator()->switchHeightControl();
		break;

    case 27:        
        {
            OSG::osgExit();
            exit(0);
        }
        break;
    }
}

// setup the GLUT library which handles the windows for us
int setupGLUT(int *argc, char *argv[])
{
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(1400, 1050);
    
    int winid = glutCreateWindow("OpenSG");
    
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
	glutIdleFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);

    return winid;
}
