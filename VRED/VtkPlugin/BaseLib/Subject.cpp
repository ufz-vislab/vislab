///////////////////////////////////////////////////////////
//  Subject.cpp
//  Implementation of the Class CSubject
//  Created on:      19-Apr-2008 15:07:48
///////////////////////////////////////////////////////////

#include "Subject.h"
#include "Observer.h"



CSubject::CSubject(){
}


CSubject::~CSubject(){
}


/**
 * Use obs->SetSubject(this) to set the instance as the subject of the given
 * observer, and if the function returns true add the observers to the
 * m_stdvecObservers vector.
 */
void CSubject::Attach(CObserver* obs){
	if (obs->SetSubject(this)){
		m_stdvecObservers.push_back(obs);
	} else {
	}
}


void CSubject::Detach(CObserver* obs){
	std::vector<CObserver*>::iterator i;
	for (i = m_stdvecObservers.begin(); i != m_stdvecObservers.end(); i++){
		if ( obs == (*i)){
			m_stdvecObservers.erase(i);
			obs->RemoveSubject();
			return;
		}
	}
}


/**
 * Notifiies all connected observers that something changed by calling Update()
 */
void CSubject::Notify(){
	std::vector<CObserver*>::iterator i;
	for (i = m_stdvecObservers.begin(); i != m_stdvecObservers.end(); i++){
		(*i)->Update();
	}
}


/**
 * Notifies all connected  observers that the subject has changed (by calling
 * Update()), except the one that is given as argument. This can be used to
 * prevent loops!
 */
void CSubject::Notify(CObserver* exceptObserver){
	std::vector<CObserver*>::iterator i;
	for (i = m_stdvecObservers.begin(); i != m_stdvecObservers.end(); i++){
		CObserver *obs = *i;
		if (obs != exceptObserver){
			obs->Update();
		}
	}
}