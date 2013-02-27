///////////////////////////////////////////////////////////
//  AttrPanelHolder.h
//  Implementation of the Class CAttrPanelHolder
//  Created on:      22-Apr-2008 14:18:52
///////////////////////////////////////////////////////////

#if !defined(EA_1A95BD48_EB41_4d38_BF6F_332B0A6088DE__INCLUDED_)
#define EA_1A95BD48_EB41_4d38_BF6F_332B0A6088DE__INCLUDED_

#include "AttrPanelObserver.h"
#include "OpenSGObserver.h"

#include <string>

class CAttrPanelHolder
{

public:
	CAttrPanelHolder();
	virtual ~CAttrPanelHolder();

	/**
	 * Function is abstract and must be implemented in a derived GUI class. The
	 * function generates and returns the attribute panel specified in the given
	 * string.
	 */
	virtual CAttrPanelObserver* GetAttrPanel(std::string type) =0;
	
	virtual COpenSGObserver* Get3DObserver()=0;

};
#endif // !defined(EA_1A95BD48_EB41_4d38_BF6F_332B0A6088DE__INCLUDED_)
