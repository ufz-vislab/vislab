#include "boost/python.hpp"
/********************************************************************
	created:	6.5.2009   10:25
	filename: 	e:\bilke\apps\vred-2.5\dev\CityGeneratorPlugin\PythonBindings.cpp
	author:		Lars Bilke
	
	purpose:	Implementation of the python bindings
*********************************************************************/

// ** INCLUDES **
#include "SpaceNavigatorPluginForm.h"
#include "vrController.h"
#include "exWindowManager.h"
#include "vrPythonInit.h"

using namespace std;
OSG_USING_NAMESPACE

using namespace boost::python;
namespace python = boost::python;

SpaceNavigatorPluginForm* GetMyWidget( void )
{
	return (SpaceNavigatorPluginForm*)vrController::getModuleManager()->getWidget("SpaceNavigator");
}

// ** Python Bindings **
BOOST_PYTHON_MODULE(SpaceNavigator)
{
	class_<SpaceNavigatorPluginForm>("SpaceNavigatorForm", init<>())
		.def("ConnectString", &SpaceNavigatorPluginForm::SetConnectString)
		.def("ConnectToDevice", &SpaceNavigatorPluginForm::ConnectToDevice)
		.def("Enable", &SpaceNavigatorPluginForm::SetEnableSpaceNavigator)
		.def("RotationFactor", &SpaceNavigatorPluginForm::SetRotationFactor)
		.def("TranslationFactor", &SpaceNavigatorPluginForm::SetTranslationFactor)
		.def("DominationMode", &SpaceNavigatorPluginForm::SetDominationMode)
		.def("NavigationMode", &SpaceNavigatorPluginForm::SetNavigationMode)
		.def("EnableOrbitMode", &SpaceNavigatorPluginForm::SetEnableOrbitMode)
		.def("EnableHeightControl", &SpaceNavigatorPluginForm::SetEnableHeightControl)
		.def("ElevationGridLoadFile", &SpaceNavigatorPluginForm::ElevGridLoadFileCL)
		.def("ElevationGridLoadSelected", &SpaceNavigatorPluginForm::ElevGridLoadSelected)
		.def("EnableElevationGrid", &SpaceNavigatorPluginForm::SetEnableElevGrid)
	;

	def("GetSpaceNavigatorWidget", GetMyWidget, return_value_policy<reference_existing_object>());
}

// Init python module
static void pythonInit(bool destroy)
{
	static bool once = false;

	if(destroy)
	{
		printf("pythonInit: destroy SpaceNavigator!\n");
		return;
	}

	if(!once)
	{
		initSpaceNavigator();
		printf("pythonInit: init SpaceNavigator\n");
		once = true;
	}

	PyRun_SimpleString("from SpaceNavigator import *");
	PyRun_SimpleString("SpaceNavigator = GetSpaceNavigatorWidget()");
}

static vrPythonInit _pythonInit(pythonInit);