///////////////////////////////////////////////////////////
//  TensorGrid.h
//  Implementation of the Class CTensorGrid
//  Created on:      03-Sep-2008 11:48:43
///////////////////////////////////////////////////////////

#if !defined(EA_22D28FE2_A676_41bd_BD81_DE6B67F37EEC__INCLUDED_)
#define EA_22D28FE2_A676_41bd_BD81_DE6B67F37EEC__INCLUDED_

#include "TensorGridState.h"
#include "Subject.h"

#include <OpenSG/OSGNode.h>

class vtkDataReader;
class vtkXMLReader;
class vtkDataSet;
class vtkMaskPoints;
class vtkSphereSource;
class vtkAxes;
class vtkTensorGlyph;
class vtkOsgActor;
class vtkHyperStreamline;

class CTensorGrid : public CSubject
{

public:
	CTensorGrid();
	virtual ~CTensorGrid();

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
	/**
	 * Function loads data from the VTK file in several formats and sets up the
	 * necessary pipelines
	 */
	bool SetFilename( const char* filename);
	/**
	 * Comparing the state to the local m_state variable and performing the necessary
	 * actions and changes
	 */
	void SetState(CTensorGridState state);
	/**
	 * Return the current state, stored in m_state
	 */
	CTensorGridState GetState();

protected:
	CTensorGridState m_state;
	bool m_bNewOsgObjectsCreated;
	OSG::NodePtr m_posgRoot;
	OSG::NodePtr m_posgGlyphsRoot;
	/**
	 * For reading legacy vtk files
	 */
	vtkDataReader* m_pvtkDataReader;
	/**
	 * For reading xml-style vtk files
	 */
	vtkXMLReader* m_pvtkXMLReader;
	/**
	 * Contains after the successful reading process the actual dataset
	 */
	vtkDataSet* m_pvtkDataSet;
	/**
	 * Masks a given fraction (by Integer) so that glyphs are only drawn at a part of
	 * the vertices. (Every "n-th").
	 */
	vtkMaskPoints* m_pvtkMaskPointsForGlyphs;
	vtkSphereSource* m_pvtkSphereSource;
	vtkAxes* m_pvtkAxes;
	vtkTensorGlyph* m_pvtkTensorGlyphs;
	vtkOsgActor* m_pGlyphActor;
	vtkHyperStreamline* m_pvtkHyperStreamline1;
	vtkOsgActor* m_pHyperStreamline1Actor;
	vtkHyperStreamline* m_pvtkHyperStreamline2;
	vtkOsgActor* m_pHyperStreamline2Actor;

	/**
	 * Set up the pipeline to represent the tensors as tensor-glyphs
	 */
	bool InitGlyphPipeline();
	/**
	 * Setting up the pipeline to 
	 */
	bool InitHyperstreamlinePipeline();

};
#endif // !defined(EA_22D28FE2_A676_41bd_BD81_DE6B67F37EEC__INCLUDED_)
