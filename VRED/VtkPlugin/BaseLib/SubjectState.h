///////////////////////////////////////////////////////////
//  SubjectState.h
//  Implementation of the Class CSubjectState
//  Created on:      19-Apr-2008 12:04:17
///////////////////////////////////////////////////////////

#if !defined(EA_86ACA40A_FFB1_4ab7_AC89_1D9116340EED__INCLUDED_)
#define EA_86ACA40A_FFB1_4ab7_AC89_1D9116340EED__INCLUDED_

/**
 * Objects of the derived classes shall contain the state of the subject (the
 * visualization). They are used to store and exchange this state. It is very
 * important that they provide a copy contstructor.
 */
class CSubjectState
{

public:
	CSubjectState();
	virtual ~CSubjectState();

};
#endif // !defined(EA_86ACA40A_FFB1_4ab7_AC89_1D9116340EED__INCLUDED_)
