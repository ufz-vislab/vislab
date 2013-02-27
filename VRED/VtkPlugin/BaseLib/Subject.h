///////////////////////////////////////////////////////////
//  Subject.h
//  Implementation of the Class CSubject
//  Created on:      19-Apr-2008 15:07:48
///////////////////////////////////////////////////////////

#if !defined(EA_5D00FD58_EF52_425a_B78B_81614320DD86__INCLUDED_)
#define EA_5D00FD58_EF52_425a_B78B_81614320DD86__INCLUDED_

#include <OpenSG/OSGNode.h>

class CObserver;

/**
 * Base class that implements the Subject int the Subject-Observer Design Pattern
 */
class CSubject
{

public:
	CSubject();
	virtual ~CSubject();
	/**
	 * Use obs->SetSubject(this) to set the instance as the subject of the given
	 * observer, and if the function returns true add the observers to the
	 * m_stdvecObservers vector.
	 */
	virtual void Attach(CObserver* obs);
	virtual void Detach(CObserver* obs);
	/**
	 * Notifiies all connected observers that something changed by calling Update()
	 */
	virtual void Notify();
	/**
	 * Notifies all connected  observers that the subject has changed (by calling
	 * Update()), except the one that is given as argument. This can be used to
	 * prevent loops!
	 */
	virtual void Notify(CObserver* exceptObserver);
	/**
	 * This function must return if the subject has created new OpenSG objects and
	 * inserted them into the scenegraph. This information is neede by some observers
	 * (mainly the 3D Canvas) to see if it needs to redistribute the scenegraph.
	 */
	virtual bool GetNewOsgObjectsCreated() =0;
	/**
	 * For observers that want to get access to the 3D scenegraph of this subject. If
	 * this is not available NullFC is returned.
	 */
	virtual OSG::NodePtr GetOpenSGRoot() =0;

protected:
	std::vector<CObserver*> m_stdvecObservers;
	bool m_bNewOsgObjectsCreated;

};
#endif // !defined(EA_5D00FD58_EF52_425a_B78B_81614320DD86__INCLUDED_)
