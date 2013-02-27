///////////////////////////////////////////////////////////
//  VtkDataSetLoader.h
//  Implementation of the Class CVtkDataSetLoader
//  Created on:      24-Sep-2008 17:28:46
///////////////////////////////////////////////////////////

#if !defined(EA_D1A7A2FE_9077_419e_A446_A352A10D18FB__INCLUDED_)
#define EA_D1A7A2FE_9077_419e_A446_A352A10D18FB__INCLUDED_

#include "VtkDataSetLoaderState.h"
#include "Subject.h"

#include <string>
#include <set>

class vtkDataReader;
class vtkXMLReader;
class vtkDataSet;

class CVtkDataSetLoader : public CSubject
{

public:
	CVtkDataSetLoader();
	virtual ~CVtkDataSetLoader();

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
	vtkDataSet* GetDataSet();
	
	std::list<std::string> GetScalarVertexAttributes();
	bool IsScalarVertexAttribute(std::string attrName);
	std::list<std::string> GetVectorVertexAttributes();
	bool IsVectorVertexAttribute(std::string attrName);
	std::list<std::string> GetTensorVertexAttributes();
	bool IsTensorVertexAttribute(std::string attrName);
	std::list<std::string> GetScalarCellAttributes();
	bool IsScalarCellAttribute(std::string attrName);
	std::list<std::string> GetVectorCellAttributes();
	bool IsVectorCellAttribute(std::string attrName);
	std::list<std::string> GetTensorCellAttributes();
	bool IsTensorCellAttribute(std::string attrName);

protected:
	CVtkDataSetLoaderState m_state;
	std::list<std::string> m_stdstrlistScalarVertexAttributes;
	std::list<std::string> m_stdstrlistVectorVertexAttributes;
	std::list<std::string> m_stdstrlistTensorVertexAttributes;
	std::list<std::string> m_stdstrlistScalarCellAttributes;
	std::list<std::string> m_stdstrlistVectorCellAttributes;
	std::list<std::string> m_stdstrlistTensorCellAttributes;
	vtkDataReader* m_pvtkDataReader;
	vtkXMLReader* m_pvtkXMLReader;

};
#endif // !defined(EA_D1A7A2FE_9077_419e_A446_A352A10D18FB__INCLUDED_)
