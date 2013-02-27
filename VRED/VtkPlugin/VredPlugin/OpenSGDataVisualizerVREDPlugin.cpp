#include "OpenSGDataVisualizerVREDPlugin.h"

#include <qlabel.h>
#include <qmessagebox.h>

#include "vrLog.h"
#include "vrScenegraph.h"
#include "vrController.h"
#include "vrNodeUtils.h"
#include "vrFileIO.h"
#include "vrFile.h"

#include "vrController.h"
#include "vrPreferences.h"
#include "vrMessageRW.h"

#include "exWindowManager.h"

#include "DataVisualizerVREDPanel.h"

using namespace osg;

// ACHTUNG muss als letzte static variable hier stehen, sonst werden nach dem
// Konstruktoraufruf andere static variablen die darunter stehen wieder ueberschrieben.
COpenSGDataVisualizerVREDPlugin    COpenSGDataVisualizerVREDPlugin::_the;

COpenSGDataVisualizerVREDPlugin::COpenSGDataVisualizerVREDPlugin(void):
    vrModuleBase("VtkDataVis", NULL, VRED_MSG_SELECTED_NODE),
    _main(NULL)
{
    //_main = new QLabel("<B>Hello World!</B>", vrController::getModuleManager());
	_main = new CDataVisualizerVREDPanel(0);

    _main->setMinimumWidth(340);
    _main->setMinimumHeight(770);
    //_main->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    vrController::getModuleManager()->add("VtkDataVis", _main);

	//von vrModuleBase
	addLoop();
	addUpdate();

	//zum abmelden der loop
	//subLoop();
}

COpenSGDataVisualizerVREDPlugin::~COpenSGDataVisualizerVREDPlugin(void){
	if(_main == NULL)
        return;

    vrController::getModuleManager()->sub(_main);
}

/*!
 * \brief
 * \param
 * \return
 */
QWidget *COpenSGDataVisualizerVREDPlugin::getWidget(void) const
{
    return _main;
}

/*!
 * \brief
 * \param
 * \return
 */
void COpenSGDataVisualizerVREDPlugin::init(void)
{
}

/*!
 * \brief
 * \param
 * \return
 */
bool COpenSGDataVisualizerVREDPlugin::receivedMessage(vrMessage &msg)
{
	switch (msg.id()){
		case VRED_MSG_SELECTED_NODE:{
			NodePtr selectedNode;

			// one way
			// getFromMessage(msg, "node", selectedNode);

			// other way
			//selectedNode = vrScenegraph::getSelectedNode();
			//std::string name = vrNodeUtils::getName(selectedNode);

			//vrLog::info("Node has been picked %s", name.c_str());
			//vrLog::warning("Node has been picked %s", name.c_str());
			//vrLog::error("Node has been picked %s", name.c_str());
		}
		break;
	}
    return false;
}

//geerbt von vrModuleBase
void COpenSGDataVisualizerVREDPlugin::loop(void){
	//std::cout << "this is the loop function" << std::endl;
}

void COpenSGDataVisualizerVREDPlugin::update(void){
	//std::cout << "this is the update function" << std::endl;
}
