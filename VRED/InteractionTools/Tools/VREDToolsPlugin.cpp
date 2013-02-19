#include "VREDToolsPlugin.h"

#include <string>

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

#include "QtVREDToolsPanel.h"

//----------------------------------------------------------------------------
// Namespaces
//----------------------------------------------------------------------------

using namespace osg;
using namespace std;

// ACHTUNG muss als letzte static variable hier stehen, sonst werden nach dem
// Konstruktoraufruf andere static variablen die darunter stehen wieder ueberschrieben.
CVREDToolsPlugin    CVREDToolsPlugin::_the;

/*!
 * \brief
 * \param
 * \return
 */
// third parameter are the different message types caught, e.g. VRED_MSG_SELECTED_NODE. See
// vrMessages.h which are available.
CVREDToolsPlugin::CVREDToolsPlugin(void) :
    vrModuleBase("InteractionTools", NULL, VRED_MSG_SELECTED_NODE),
    _main(NULL)
{
	_main = new CQtVREDToolsPanel(vrController::getModuleManager());
    //_main = new QLabel("<B>Hello Tools!</B>", vrController::getModuleManager());
	//_main = new CQTestWidget(0);

    _main->setMinimumWidth(300);
    _main->setMinimumHeight(700);
    //_main->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    vrController::getModuleManager()->add("InteractionTools", _main);

	//von vrModuleBase
	addLoop();
	addUpdate();

	//zum abmelden der loop
	//subLoop();
}

/*!
 * \brief
 * \param
 * \return
 */
CVREDToolsPlugin::~CVREDToolsPlugin(void)
{
    if(_main == NULL)
        return;

    vrController::getModuleManager()->sub(_main);
}

/*!
 * \brief
 * \param
 * \return
 */
QWidget *CVREDToolsPlugin::getWidget(void) const
{
    return _main;
}

/*!
 * \brief
 * \param
 * \return
 */
void CVREDToolsPlugin::init(void){
}

/*!
 * \brief
 * \param
 * \return
 */
bool CVREDToolsPlugin::receivedMessage(vrMessage &msg)
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
void CVREDToolsPlugin::loop(void){
	//std::cout << "this is the loop function" << std::endl;
	_main->Loop();
}

void CVREDToolsPlugin::update(void){
	//std::cout << "this is the update function" << std::endl;
}
