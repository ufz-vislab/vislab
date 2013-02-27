///////////////////////////////////////////////////////////
//  ScalarCellPropGrid.h
//  Implementation of the Class CScalarCellPropGrid
//  Created on:      08-Sep-2008 16:23:09
///////////////////////////////////////////////////////////

#if !defined(EA_2265002C_9EDD_4bc5_BEB0_534640041BA6__INCLUDED_)
#define EA_2265002C_9EDD_4bc5_BEB0_534640041BA6__INCLUDED_

#include "ScalarCellPropGridState.h"
#include "Subject.h"

/**
 * This class implements the pipeline to display the cells of data set. These can
 * be clipped away dependent on their value (using the vtkThreshold filter and be
 * shrinked to better outline the different cells (using vtkShrinkFilter). The
 * scalar cell value is shown as color. The overall pipeline is vtkReader ->
 * vtkDataSet -> vtkShrinkFilter -> vtkThreshold -> vtkPolyDataMapper ->
 * vtkOsgActor
 */
 
class vtkDataReader;
class vtkXMLReader;
class vtkDataSet;
class vtkShrinkFilter;
class vtkThreshold;
class vtkPolyDataMapper;
class vtkOsgActor;

#include <OpenSG/OSGNode.h>

class CScalarCellPropGrid : public CSubject
{

public:
	CScalarCellPropGrid();
	virtual ~CScalarCellPropGrid();

	/**
	 * This function must return if the subject has created new OpenSG objects and
	 * inserted them into the scenegraph. This information is neede by some observers
	 * (mainly the 3D Canvas) to see if it needs to redistribute the scenegraph.
	 */
	virtual bool GetNewOsgObjectsCreated();
	/**
	 * For observers that want to get access to the 3D scenegraph of this subject. If
	 * this is not available NullFC is returned.
	 */
	virtual OSG::NodePtr GetOpenSGRoot();
	bool SetFilename(const char* filename);
	void SetState(CScalarCellPropGridState state);
	CScalarCellPropGridState GetState();
	
protected:
	bool SetProperStateValsForCurrentAttribute();
	bool InitPipeline();

protected:
	CScalarCellPropGridState m_state;
	std::list<std::string> m_stdlistScalarCellAttributes;
	OSG::NodePtr m_posgRoot;
	vtkDataReader* m_pvtkDataReader;
	vtkXMLReader* m_pvtkXMLReader;
	vtkDataSet* m_pvtkDataSet;
	vtkShrinkFilter* m_pvtkShrinkFilter;
	vtkThreshold* m_pvtkThreshold;
	vtkPolyDataMapper* m_pvtkPolyDataMapper;
	vtkOsgActor* m_pCellActor;
};
#endif // !defined(EA_2265002C_9EDD_4bc5_BEB0_534640041BA6__INCLUDED_)
