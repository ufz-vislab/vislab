///////////////////////////////////////////////////////////
//  VectorGrid.h
//  Implementation of the Class CVectorGrid
//  Created on:      05-Aug-2008 14:40:03
///////////////////////////////////////////////////////////

#if !defined(EA_E98B2146_2841_494e_9758_5E368EEF21F5__INCLUDED_)
#define EA_E98B2146_2841_494e_9758_5E368EEF21F5__INCLUDED_

#include "VectorGridState.h"
#include "Subject.h"

#include <OpenSG/OSGNode.h>

class vtkDataReader;
class vtkXMLReader;
class vtkDataSet;
class vtkMaskPoints;
class vtkGlyph3D;
class vtkOsgActor;
class vtkStreamer;
class vtkPolyData;
class vtkTubeFilter;
class vtkPoints;

class CVectorGrid : public CSubject
{

public:
	CVectorGrid();
	virtual ~CVectorGrid();

	/**
	 * This function must return if the subject has created new OpenSG objects and
	 * inserted them into the scenegraph. This information is neede by some observers
	 * (mainly the 3D Canvas) to see if it needs to redistribute the scenegraph.
	 */
	bool GetNewOsgObjectsCreated();
	/**
	 * For observers that want to get access to the 3D scenegraph of this subject. If
	 * this is not available NullFC is returned.
	 */
	OSG::NodePtr GetOpenSGRoot();
	bool SetFilename(const char* filename);
	void SetState(CVectorGridState state);
	CVectorGridState GetState();

protected:
	CVectorGridState m_state;
	bool m_bNewOsgObjectsCreated;
	OSG::NodePtr m_posgRoot;
	/**
	 * The vtk data reader. The base class for structured and unstructured grids is
	 * used, so that both can be allocated and read.
	 */
	vtkDataReader* m_pvtkDataReader;
	vtkXMLReader* m_pvtkXMLReader;
	/**
	 * Will contain the dataset when this has been successfully loaded
	 */
	vtkDataSet *m_pvtkDataSet;
	/**
	 * Using this filter, only a fraction of the read points is used to show a glyph.
	 * This fraction can be set (via the state).
	 */
	vtkMaskPoints* m_pvtkMaskPointsForGlyphs;
	/**
	 * This filter places a glyph at each point that passes the vtkMaskPoints filter.
	 * The scalefactor for the size can be set.
	 * 
	 * 
	 */
	vtkGlyph3D* m_pvtkGlyph3D;
	/**
	 * The actor that can generate the corresponding OpenSG objects.
	 */
	vtkOsgActor* m_pGlyphActor;
	/**
	 * Generates the streamlines. The base class for vtkStreamLine and
	 * vtkDashedStreamLine is used so that this can later be changed. The streamline
	 * related data (length etc ...) can be set on this filter.
	 */
	vtkStreamer* m_pvtkStreamer;
	/**
	 * Used as input source for the streamer, contains the starting points from which
	 * the streamer shall generate the streamlines.
	 */
	vtkPoints* m_pvtkPointsForStreamer;
	vtkPolyData *m_pvtkPolyDataForStreamer;
	/**
	 * Generates tubes from the streamlines, the tube radius can be set on this filter.
	 */
	vtkTubeFilter* m_pvtkTubeFilter;
	/**
	 * The actor for the streamlines that can generate the corresponding OpenSG
	 * objects.
	 */
	vtkOsgActor* m_pStreamlinesActor;

	bool InitGlyphPipeline();
	bool InitStreamlinePipeline();
	void AddStreamlineSourcesOnRegularGrid(int numXPoints, int numYPoints, int numZPoints);
	void AddStreamlineSourcesFromGeomObjectPoints(std::string objectname);
	OSG::NodePtr FindGeomNodeWithName(OSG::NodePtr node, std::string name);

};
#endif // !defined(EA_E98B2146_2841_494e_9758_5E368EEF21F5__INCLUDED_)
