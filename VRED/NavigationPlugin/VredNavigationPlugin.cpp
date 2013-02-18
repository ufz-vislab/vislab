#include "VredNavigationPlugin.h"

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

#include "NavigationMainFrame.h"

//----------------------------------------------------------------------------
// Namespaces
//----------------------------------------------------------------------------

using namespace osg;
using namespace std;

// ACHTUNG muss als letzte static variable hier stehen, sonst werden nach dem
// Konstruktoraufruf andere static variablen die darunter stehen wieder ueberschrieben.
CVredNavigationPlugin    CVredNavigationPlugin::_the;

/*!
 * \brief
 * \param
 * \return
 */
// third parameter are the different message types caught, e.g. VRED_MSG_SELECTED_NODE. See
// vrMessages.h which are available.
CVredNavigationPlugin::CVredNavigationPlugin(void) :
    vrModuleBase("NavigationPlugin", NULL, VRED_MSG_SELECTED_NODE),
    _main(NULL)
{
    //_main = new QLabel("<B>Hello World!</B>", vrController::getModuleManager());
	_main = new CNavigationMainFrame;

    _main->setMinimumWidth(900);
    _main->setMinimumHeight(900);
    //_main->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    vrController::getModuleManager()->add("NavigationPlugin", _main);
}

/*!
 * \brief
 * \param
 * \return
 */
CVredNavigationPlugin::~CVredNavigationPlugin(void)
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
QWidget *CVredNavigationPlugin::getWidget(void) const
{
    return _main;
}

/*!
 * \brief
 * \param
 * \return
 */
void CVredNavigationPlugin::init(void)
{
}

/*!
 * \brief
 * \param
 * \return
 */
bool CVredNavigationPlugin::receivedMessage(vrMessage &msg)
{
	switch (msg.id()){
		case VRED_MSG_SELECTED_NODE:{
		}
		break;
	}
    return false;
}
