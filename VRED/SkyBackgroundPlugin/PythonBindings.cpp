#include "boost/python.hpp"
/********************************************************************
purpose:	Implementation of the python bindings
*********************************************************************/

// ** INCLUDES **
#include "SkyBackgroundPluginForm.h"
#include "vrController.h"
#include "exWindowManager.h"
#include "vrPythonInit.h"

using namespace std;
OSG_USING_NAMESPACE

using namespace boost::python;
namespace python = boost::python;

SkyBackgroundPluginForm* GetMyWidget( void )
{
	return (SkyBackgroundPluginForm*)vrController::getModuleManager()->getWidget("SkyBackground");
}

// ** Python Bindings **
BOOST_PYTHON_MODULE(SkyBackground)
{
	class_<SkyBackgroundPluginForm>("SkyBackgroundForm", init<>())
		.def("Load", &SkyBackgroundPluginForm::setupSkyBackgroundStr)
		.def("Enable", &SkyBackgroundPluginForm::on_enableCheckBox_toggled)
		.def("ZUp", &SkyBackgroundPluginForm::on_zUpCheckBox_toggled)
	;

	def("GetSkyBackgroundWidget", GetMyWidget, return_value_policy<reference_existing_object>());
}

// Init python module
static void pythonInit(bool destroy)
{
	static bool once = false;

	if(destroy)
	{
		printf("pythonInit: destroy SkyBackground!\n");
		return;
	}

	if(!once)
	{
		initSkyBackground();
		printf("pythonInit: init SkyBackground\n");
		once = true;
	}

	PyRun_SimpleString("from SkyBackground import *");
	PyRun_SimpleString("SkyBackground = GetSkyBackgroundWidget()");
}

static vrPythonInit _pythonInit(pythonInit);

