///////////////////////////////////////////////////////////
//  Observer.h
//  Implementation of the Class CObserver
//  Created on:      19-Apr-2008 11:51:56
///////////////////////////////////////////////////////////

#if !defined(EA_EF4AD981_2C3E_44da_A3A8_DEEE30E792E4__INCLUDED_)
#define EA_EF4AD981_2C3E_44da_A3A8_DEEE30E792E4__INCLUDED_
class CSubject;

/**
 * Base class that implements the observer in the subject observer design pattern.
 */
class CObserver
{

public:
	CObserver();
	virtual ~CObserver();
	virtual void Update() =0;
	
	//Sets the subject this object shall observe. The return value indicates if
	//this observer is suitable to observe the given subject.
	virtual bool SetSubject(CSubject* sub) =0;

	virtual void RemoveSubject();

protected:
	CSubject* m_pSubject;

};
#endif // !defined(EA_EF4AD981_2C3E_44da_A3A8_DEEE30E792E4__INCLUDED_)
