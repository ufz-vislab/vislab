///////////////////////////////////////////////////////////
//  OpenSGObserver.h
//  Implementation of the Class COpenSGObserver
//  Created on:      15-Jul-2008 16:48:27
///////////////////////////////////////////////////////////

#if !defined(EA_6759BE93_4F74_475a_AC79_269B4D31254B__INCLUDED_)
#define EA_6759BE93_4F74_475a_AC79_269B4D31254B__INCLUDED_

#include "Observer.h"

#include <OpenSG/OSGNode.h>

class COpenSGObserver : public CObserver
{

public:
	COpenSGObserver();
	virtual ~COpenSGObserver();

	/**
	 * Function is abstract. In derived classes this function shall call the necessary
	 * functions to update the 3D view. For example calling the render action from
	 * OpenSG or updateGL of a QGlWidget.
	 */
	virtual void Update()=0;
	/**
	 * Sets the subject this object shall observe. The return value indicates if this
	 * observer is suitable to observe the given subject.
	 */
	virtual bool SetSubject(CSubject* sub)=0;
};
#endif // !defined(EA_6759BE93_4F74_475a_AC79_269B4D31254B__INCLUDED_)
