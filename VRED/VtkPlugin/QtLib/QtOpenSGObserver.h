///////////////////////////////////////////////////////////
//  QtOpenSGObserver.h
//  Implementation of the Class CQtOpenSGObserver
//  Created on:      15-Jul-2008 17:38:34
///////////////////////////////////////////////////////////

#if !defined(EA_14AD20D1_610B_40de_925C_55BF176D2195__INCLUDED_)
#define EA_14AD20D1_610B_40de_925C_55BF176D2195__INCLUDED_

#include "OpenSGObserver.h"

#include <qgl.h>

class CQtOpenSGObserver : public COpenSGObserver
{

public:
	CQtOpenSGObserver();
	virtual ~CQtOpenSGObserver();

	void Update();
	/**
	 * Sets the subject this object shall observe. The return value indicates if this
	 * observer is suitable to observe the given subject.
	 */
	bool SetSubject(CSubject* sub);
	
	static void SetQGLWidget(QGLWidget* widget);

private:
	static QGLWidget* m_pqGLWidget;

};
#endif // !defined(EA_14AD20D1_610B_40de_925C_55BF176D2195__INCLUDED_)
