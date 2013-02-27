///////////////////////////////////////////////////////////
//  DataVisApp.h
//  Implementation of the Class CDataVisApp
//  Created on:      22-Apr-2008 18:00:49
///////////////////////////////////////////////////////////

#if !defined(EA_44983E47_0AE3_4866_81A1_C6B97BA77C51__INCLUDED_)
#define EA_44983E47_0AE3_4866_81A1_C6B97BA77C51__INCLUDED_

#include "AttrPanelSelector.h"
#include "AttrPanelHolder.h"

#include "OpenSGObserver.h"

#include <OpenSG/OSGNode.h>

/**
 * The general application object
 */
class CDataVisApp
{

public:
	virtual ~CDataVisApp();
	static CDataVisApp* Instance();
	void SetPanelSelector(CAttrPanelSelector* panelSelector);
	void SetPanelHolder(CAttrPanelHolder* panelHolder);
	OSG::NodePtr GetOsgRoot(){return m_posgRoot;}
	
	void LoadTest();
	void LoadScalarUnstructuredGrid(const char* filename);
	void LoadVtkVectorGrid(const char *filename);

protected:
	CAttrPanelSelector* m_pPanelSelector;
	CAttrPanelHolder* m_pPanelHolder;
	static CDataVisApp* m_pInstance;

	CDataVisApp();

	OSG::NodePtr m_posgRoot;

};
#endif // !defined(EA_44983E47_0AE3_4866_81A1_C6B97BA77C51__INCLUDED_)
