///////////////////////////////////////////////////////////
//  ScalarUnstructuredGrid.h
//  Implementation of the Class CScalarUnstructuredGrid
//  Created on:      08-Jul-2008 17:59:57
///////////////////////////////////////////////////////////

#if !defined(EA_A4AF62C8_B1AD_4571_9C27_EF7D202CEFEF__INCLUDED_)
#define EA_A4AF62C8_B1AD_4571_9C27_EF7D202CEFEF__INCLUDED_

#include "ScalarGridState.h"
#include "Subject.h"
#include <OpenSG/OSGNode.h>

class vtkOsgActor;
class vtkUnstructuredGridReader;
class vtkUnstructuredGrid;
class vtkDataSet;
class vtkDataReader;
class vtkXMLReader;
class vtkContourFilter;
class vtkPlane;
class vtkLookupTable2D;

#include <OpenSG/OSGNode.h>
//OSG_USING_NAMESPACE

/**
 * This class uses VTK to extract Isosurfaces and cut sections through a
 * vtkUnstructuredGrid using scalar data. The visualization is steered using the
 * state kept in an instance of type CScalarGridState. The vtkOsgActor class can
 * be used to derive OpenSG visualizations of the isosurfaces and sections.
 */
class CScalarUnstructuredGrid : public CSubject
{

public:
	CScalarUnstructuredGrid();
	virtual ~CScalarUnstructuredGrid();

	bool SetFilename(const char* filename);
	bool SetStateForAttributeName(std::string attrName);
	bool SetNewContourAttribute(std::string attrName);
	bool SetDataSet(vtkDataSet*, const char *attributeName);
	/**
	 * Compares the state given to the current state of the visualization. If some of
	 * the states attributes are different the necessary functions are called to adapt
	 * the visualization to the given state and the current state is set properly.
	 */
	void SetState(CScalarGridState state);
	/**
	 * Return the current state of the visualization (CScalarGridState)
	 */
	CScalarGridState GetState();
	/**
	 * This function must return if the subject has created new OpenSG objects and
	 * inserted them into the scenegraph. This information is neede by some observers
	 * (mainly the 3D Canvas) to see if it needs to redistribute the scenegraph.
	 */
	virtual bool GetNewOsgObjectsCreated(){
		return m_bNewOsgObjectsCreated;
	}
	/**
	 * For observers that want to get access to the 3D scenegraph of this subject. If
	 * this is not available NullFC is returned.
	 */
	virtual OSG::NodePtr GetOpenSGRoot();
	
	virtual std::list<std::string> GetAvailableScalars();

protected:
	/**
	 * General state of this visualization
	 * 
	 */
	CScalarGridState m_state;
	bool m_bNewOsgObjectsCreated;
	/**
	 * The root of this part of the scenegraph
	 */
	OSG::NodePtr m_posgRoot;
	/**
	 * //contours of the dataset
	 */
	vtkOsgActor* m_pContourActor;
	/**
	 * A Slice perpendicular to the x axis
	 */
	vtkOsgActor* m_pXCutActor;
	/**
	 * A slice perpendicular to the y axis
	 */
	vtkOsgActor* m_pYCutActor;
	/**
	 * A slice perpendicular to the z axis
	 */
	vtkOsgActor* m_pZCutActor;
	
	//vtkUnstructuredGridReader* m_pvtkGridReader;
	//vtkUnstructuredGrid *m_pvtkGrid;
	vtkDataReader *m_pvtkDataReader;
	vtkXMLReader *m_pvtkXMLReader;
	vtkDataSet *m_pvtkDataSet;
	vtkContourFilter* m_pvtkContourFilter;
	vtkPlane* m_pvtkXCutPlane;
	vtkPlane* m_pvtkYCutPlane;
	vtkPlane* m_pvtkZCutPlane;
	
	vtkLookupTable2D* m_pvtkLookupTable;
	vtkLookupTable2D* m_pvtkXSectionLookupTable;
	vtkLookupTable2D* m_pvtkYSectionLookupTable;
	vtkLookupTable2D* m_pvtkZSectionLookupTable;

	/**
	 * If not done so far, this function sets up the vtk pipeline for looking at
	 * contours, adds the generated geometry to internal OpenSG root and sets the
	 * m_bNewOsgObjectsCreated flag to true
	 */
	bool InitContourPipeline();
	/**
	 * If not done so far, this function sets up the vtk pipeline for looking at at a
	 * section perpendicularl to the x-axis, adds the generated geometry to the
	 * internal OpenSG root and sets the m_bNewOsgObjectsCreated flag to true
	 */
	bool InitXCutPlanePipeline();
	/**
	 * If not done so far, this function sets up the vtk pipeline for looking at at a
	 * section perpendicularl to the y-axis, adds the generated geometry to the
	 * internal OpenSG root and sets the m_bNewOsgObjectsCreated flag to true
	 */
	bool InitYCutPlanePipeline();
	/**
	 * If not done so far, this function sets up the vtk pipeline for looking at at a
	 * section perpendicularl to the z-axis, adds the generated geometry to the
	 * internal OpenSG root and sets the m_bNewOsgObjectsCreated flag to true
	 */
	bool InitZCutPlanePipeline();
	
	bool GetMinMaxForAttribute(std::string attributeName, double &min, double &max);

};
#endif // !defined(EA_A4AF62C8_B1AD_4571_9C27_EF7D202CEFEF__INCLUDED_)
