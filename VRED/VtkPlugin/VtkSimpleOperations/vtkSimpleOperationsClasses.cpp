// vtkSimpleOperationsClasses.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "vtkReaderWriterPairConstructor.h"
#include "VtkVectorRescaler.h"
#include "VtkAttributeExtractor.h"
#include "VtkScalarsToTensor.h"

#include <iostream>

#include <vtkDataSet.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>
#include <vtkDoubleArray.h>

#include <string>
#include <list>

#include <vtkPointLoad.h>
#include <vtkExtractTensorComponents.h>
#include <vtkFloatArray.h>
#include <vtkStructuredPointsReader.h>
#include <vtkStructuredPoints.h>


#include <vtkDataSetWriter.h>
#include <vtkStructuredPointsWriter.h>
#include <vtkStructuredGridWriter.h>
#include <vtkMergeDataObjectFilter.h>

#include "vtkMoveDataFromCellToPoint.h"

int _tmain(int argc, _TCHAR* argv[])
{

	CVtkMoveDataFromCellToPoint *c2p = new CVtkMoveDataFromCellToPoint();
	if (c2p->LoadFile(std::string("staub.vtk"))){
		std::cout << "could load file" << std::endl;
		c2p->WriteFile(std::string("staub_pointdata.vtk"));
	} else {
		std::cout << "could not load file" << std::endl;
	}
	return 0;

	vtkPointLoad *ptLoad = vtkPointLoad::New();
	ptLoad->SetLoadValue(100.0);
	ptLoad->SetSampleDimensions(20, 20, 20);
	ptLoad->ComputeEffectiveStressOn();
	ptLoad->SetModelBounds(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0);
	vtkStructuredPointsWriter *writer = vtkStructuredPointsWriter::New();
	writer->SetInputConnection(ptLoad->GetOutputPort());
	writer->SetFileName("tensor_output_merged_new.vtk");
	writer->Write();

	vtkStructuredPointsReader *reader = vtkStructuredPointsReader::New();
	reader->SetFileName("tensor_output_merged_new.vtk");
	reader->Update();

	vtkExtractTensorComponents *tensorCompExtractor1 = vtkExtractTensorComponents::New();
	tensorCompExtractor1->SetInput(reader->GetOutput());
	tensorCompExtractor1->ExtractScalarsOn();
	tensorCompExtractor1->SetScalarModeToDeterminant();
	//tensorCompExtractor1->PassTensorsToOutputOn();
	tensorCompExtractor1->Update();
	tensorCompExtractor1->DebugOn();

	vtkDataSet *dataSet = tensorCompExtractor1->GetOutput();
	vtkPointData *pointData = dataSet->GetPointData();
	int numArrays = pointData->GetNumberOfArrays(); //numArrays is one
	std::cout << "array name: " << pointData->GetArray(0)->GetName() << std::endl;
	return 0;

	//determinantArray->SetName("determinat");

	//vtkExtractTensorComponents *tensorCompExtractor2 = vtkExtractTensorComponents::New();
	//tensorCompExtractor2->SetInputConnection(ptLoad->GetOutputPort());
	//tensorCompExtractor2->ExtractScalarsOn();
	//tensorCompExtractor2->SetScalarModeToEffectiveStress();
	//tensorCompExtractor1->Update();
	//vtkDataArray *effectiveStressArray = tensorCompExtractor2->GetOutput()->GetPointData()->GetScalars();
	//effectiveStressArray->SetName("effective_stress");
	
	
	return 0;

	//CVtkScalarsToTensor st;
	////if (st.LoadFile("urach_mit_vector.vtk")){
	//if (st.LoadFile("urach_domain_tet30.vtk")){
	//	std::cout << "could load file" << std::endl;
	//} else {
	//	std::cout << "could not load file" << std::endl;
	//}

	//std::string suffix = st.GetCorrectFileSuffix();
	//std::cout << "file suffix: " << suffix << std::endl;
	//std::string outputname = std::string("with_tensor") + suffix;
	//std::cout << "output to: " << outputname << std::endl;

	//std::list<std::string> comps;
	//comps.push_back(std::string("PRESSURE1"));
	//comps.push_back(std::string("PRESSURE1"));
	//comps.push_back(std::string("PRESSURE1"));
	//comps.push_back(std::string("PRESSURE1"));
	//comps.push_back(std::string("PRESSURE1"));
	//comps.push_back(std::string("PRESSURE1"));
	//comps.push_back(std::string("PRESSURE1"));
	//comps.push_back(std::string("PRESSURE1"));
	//comps.push_back(std::string("PRESSURE1"));
	//
	//st.SetTensorComponents(std::string("tensor"), comps);
	////st.WriteFile(outputname);
	//return 0;

	

	//CVTKSkalarsToVector conv;
	//conv.ReadVTKGridFromFile("urach_domain_tet30.vtk");
	//
	//std::string vxstr("VELOCITY_X1");
	//std::string vystr("VELOCITY_Y1");
	//std::string vzstr("VELOCITY_Z1");

	//conv.SetVectorComponents(std::string("myVector"), vxstr, vystr, vzstr);
	//conv.WriteVTKGridToFile("testoutput_1.vtk");

	//conv.SetVectorComponents(std::string("yourVector"), vxstr, vystr, vzstr);
	//conv.WriteVTKGridToFile("testoutput_2.vtk");

	//conv.SetVectorComponents(std::string("hisVector"), vxstr, vystr, vzstr);
	//conv.WriteVTKGridToFile("testoutput_3.vtk");

	//std::list<std::string> scalarNames = conv.GetAvailableScalars();
	//std::list<std::string>::iterator iter;
	//for (iter = scalarNames.begin(); iter != scalarNames.end(); iter++){
	//	std::cout << *iter << std::endl;
	//}

	return 0;
}

