#include "VredWindparkPlugin.h"

#include <string>

#include <qlabel.h>
#include <qdatetime.h>
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

#include "WPEProjectController.h"

//----------------------------------------------------------------------------
// Namespaces
//----------------------------------------------------------------------------

using namespace osg;
using namespace std;

QTime *CVredWindparkPlugin::m_pTime = NULL;
bool CVredWindparkPlugin::m_bUseLoopFunction = false;

// ACHTUNG muss als letzte static variable hier stehen, sonst werden nach dem
// Konstruktoraufruf andere static variablen die darunter stehen wieder ueberschrieben.
CVredWindparkPlugin    CVredWindparkPlugin::_the;

/*!
 * \brief
 * \param
 * \return
 */
// third parameter are the different message types caught, e.g. VRED_MSG_SELECTED_NODE. See
// vrMessages.h which are available.
CVredWindparkPlugin::CVredWindparkPlugin(void) :
    vrModuleBase("WindparkPlugin", NULL, VRED_MSG_SELECTED_NODE),
    _main(NULL)
{
    //_main = new QLabel("<B>Hello World!</B>", vrController::getModuleManager());
	//_main = new CQTestWidget(0);
	//_main = new CWindparkPluginPanel();
	_main = new CWPEMainFrame();

    _main->setMinimumWidth(900);
    _main->setMinimumHeight(900);
    //_main->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    vrController::getModuleManager()->add("WindparkPlugin", _main);

	addLoop();

	m_pTime = new QTime();
	m_pTime->start();
}

/*!
 * \brief
 * \param
 * \return
 */
CVredWindparkPlugin::~CVredWindparkPlugin(void)
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
QWidget *CVredWindparkPlugin::getWidget(void) const
{
    return _main;
}

/*!
 * \brief
 * \param
 * \return
 */
void CVredWindparkPlugin::init(void){
}

/*!
 * \brief
 * \param
 * \return
 */
bool CVredWindparkPlugin::receivedMessage(vrMessage &msg)
{
	switch (msg.id()){
		case VRED_MSG_SELECTED_NODE:{
			//NodePtr selectedNode;

			//// one way
			//// getFromMessage(msg, "node", selectedNode);

			//// other way
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

void CVredWindparkPlugin::loop(){
	float timeInMilliseconds = (float) m_pTime->elapsed();
	float degrees = 180.0 * timeInMilliseconds / 1000.0;
	CWPEProjectController::Instance()->SetRotorRotation(degrees);
}