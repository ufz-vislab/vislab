#pragma once

class vtkDataReader;
class vtkXMLReader;
class vtkDataWriter;
class vtkXMLWriter;
class vtkDataSet;

#include <string>
#include <list>

class CVtkReaderWriterPairConstructor
{
public:
	CVtkReaderWriterPairConstructor(void);
	~CVtkReaderWriterPairConstructor(void);
	bool					LoadFile(std::string filename, bool bForceUseXMLWriter = false);
	bool					WriteFile(std::string filename);
	std::string				GetCorrectFileSuffix();
	std::list<std::string>	GetAttributeNames();
	std::list<std::string>	GetScalarAttributeNames();
	std::list<std::string>	GetVectorAttributeNames();
	std::list<std::string>	GetTensorAttributeNames();
	vtkDataSet*				GetReaderOutput();
	bool					SetWriterInput(vtkDataSet* dataSet);

protected:
	void					ClearAll();
	

protected:
	vtkDataReader*			m_pvtkDataReader;
	vtkXMLReader*			m_pvtkXMLReader;
	vtkDataWriter*			m_pvtkDataWriter;
	vtkXMLWriter*			m_pvtkXMLWriter;
	vtkDataSet*				m_pvtkDataSet;

	std::string					m_stdstrRequiredSuffix;
};
