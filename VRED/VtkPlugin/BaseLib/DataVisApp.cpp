///////////////////////////////////////////////////////////
//  DataVisApp.cpp
//  Implementation of the Class CDataVisApp
//  Created on:      22-Apr-2008 18:00:49
///////////////////////////////////////////////////////////

#include "DataVisApp.h"

#include <OpenSG/OSGGroup.h>
#include <OpenSG/OSGSimpleAttachments.h>

#include "ScalarUnstructuredGrid.h"
#include "ScalarGridState.h"

#include "VectorGrid.h"
#include "VectorGridState.h"

using namespace osg;

CDataVisApp* CDataVisApp::m_pInstance = NULL;

CDataVisApp::CDataVisApp(){
	m_pPanelSelector = NULL;
	m_pPanelHolder = NULL;

	//m_posgRoot = OSG::NullFC;
	osgInit(0, NULL);
	m_posgRoot = makeCoredNode<Group>();
	setName(m_posgRoot, "VTK_Data");
	addRefCP(m_posgRoot);
}


CDataVisApp::~CDataVisApp(){
	subRefCP(m_posgRoot);
}


CDataVisApp* CDataVisApp::Instance(){
	if (m_pInstance == NULL){
		m_pInstance = new CDataVisApp();
		return m_pInstance;
	} else {
		return m_pInstance;
	}
}


void CDataVisApp::SetPanelSelector(CAttrPanelSelector* panelSelector){
	m_pPanelSelector = panelSelector;
}


void CDataVisApp::SetPanelHolder(CAttrPanelHolder* panelHolder){
	m_pPanelHolder = panelHolder;
}


void CDataVisApp::LoadTest(){
	if ((m_pPanelSelector == NULL) || (m_pPanelHolder == NULL)) return;
	CAttrPanelObserver* panel = m_pPanelHolder->GetAttrPanel(std::string("TestPanel"));
	m_pPanelSelector->AddItem(std::string("Test panels"), std::string("a test panel"), panel);
}

void CDataVisApp::LoadScalarUnstructuredGrid(const char *filename){
	if ((m_pPanelSelector == NULL) || (m_pPanelHolder == NULL)) return;

	//if (m_posgRoot == OSG::NullFC){
	//	m_posgRoot = makeCoredNode<Group>();
	//	COpenSGCanvas::Instance()->AddGeometry(m_posgRoot);
	//}

	CScalarUnstructuredGrid *grid = new CScalarUnstructuredGrid();
	if (grid->SetFilename(filename)){
		CAttrPanelObserver* panel = m_pPanelHolder->GetAttrPanel(std::string("ScalarGrid"));
		m_pPanelSelector->AddItem(std::string("Scalar grids"), std::string("a scalar grid"), panel);
		grid->Attach(panel);
		beginEditCP(m_posgRoot, Node::ChildrenFieldMask);{
			m_posgRoot->addChild(grid->GetOpenSGRoot());
		};endEditCP(m_posgRoot, Node::ChildrenFieldMask);

		COpenSGObserver *obs = m_pPanelHolder->Get3DObserver();
		grid->Attach(obs);

		CScalarGridState state = grid->GetState();
		state.m_bShowContours = true;
		state.m_bShowXSection = true;
		state.m_bShowYSection = true;
		state.m_bShowZSection = true;
		grid->SetState(state);

		panel->SetInitialValues();
	}
}

void CDataVisApp::LoadVtkVectorGrid(const char *filename){
	if ((m_pPanelSelector == NULL) || (m_pPanelHolder == NULL)) return;
	
	CVectorGrid *grid = new CVectorGrid();
	if (grid->SetFilename(filename)){
		CAttrPanelObserver* panel = m_pPanelHolder->GetAttrPanel(std::string("VectorGrid"));
		m_pPanelSelector->AddItem(std::string("vector grids"), std::string("a vector grid"), panel);
		grid->Attach(panel);
		beginEditCP(m_posgRoot, Node::ChildrenFieldMask);{
			m_posgRoot->addChild(grid->GetOpenSGRoot());
		};endEditCP(m_posgRoot, Node::ChildrenFieldMask);

		COpenSGObserver *obs = m_pPanelHolder->Get3DObserver();
		grid->Attach(obs);
		
		panel->SetInitialValues();
	}
}