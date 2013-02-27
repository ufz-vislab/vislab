///////////////////////////////////////////////////////////
//  VectorGrid.cpp
//  Implementation of the Class CVectorGrid
//  Created on:      05-Aug-2008 14:40:03
///////////////////////////////////////////////////////////

#include "VectorGrid.h"

#include <OpenSG/OSGGroup.h>
#include <OpenSG/OSGNode.h>
#include <OpenSG/OSGSimpleAttachments.h>
#include <OpenSG/OSGGeometry.h>

#include "vtkDataReader.h"
#include "vtkStructuredGridReader.h"
#include "vtkUnstructuredGridReader.h"
#include "vtkStructuredPointsReader.h"
#include "vtkXMLReader.h"
#include "vtkXMLUnstructuredGridReader.h"
#include "vtkXMLStructuredGridReader.h"
#include "vtkDataSet.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"
#include "vtkAbstractArray.h"

#include "vtkMaskPoints.h"
#include "vtkGlyph3D.h"
#include "vtkArrowSource.h"
#include "vtkPolyDataMapper.h"

#include "vtkRungeKutta4.h"
#include "vtkStreamLine.h"
#include "vtkStreamer.h"
#include "vtkTubeFilter.h"
#include "vtkPoints.h"
#include "vtkPointSet.h"

#include "vtkOsgActor.h"

#include "set"

using namespace osg;

CVectorGrid::CVectorGrid(){
	m_posgRoot = makeCoredNode<Group>();
	
	m_pvtkDataReader = NULL;
	m_pvtkXMLReader = NULL;
	
	m_pvtkDataSet = NULL;
	m_pvtkMaskPointsForGlyphs = NULL;
	m_pvtkGlyph3D = NULL;
	m_pGlyphActor = NULL;
	m_pvtkStreamer = NULL;
	m_pvtkPointsForStreamer = NULL;
	m_pvtkTubeFilter = NULL;
	m_pStreamlinesActor = NULL;
	
	m_bNewOsgObjectsCreated = false;
}



CVectorGrid::~CVectorGrid(){

}

bool CVectorGrid::GetNewOsgObjectsCreated(){
	return m_bNewOsgObjectsCreated;
}

/**
 * For observers that want to get access to the 3D scenegraph of this subject. If
 * this is not available NullFC is returned.
 */
OSG::NodePtr CVectorGrid::GetOpenSGRoot(){
	return  m_posgRoot;
}


bool CVectorGrid::SetFilename(const char* filename){
	std::string strfilename(filename);
	
	std::string basename, suffix, tmpname;
	std::string::size_type pos = strfilename.rfind(".");
	if (pos != std::string::npos){
		basename = strfilename.substr(0, pos);
		suffix = strfilename.substr(pos);
	} else {
		return false;
	}
	
	if (suffix == std::string(".vts")){
		std::cout << "attempt to read structured grid from xml file" << std::endl;
		m_pvtkXMLReader = vtkXMLStructuredGridReader::New();
		m_pvtkXMLReader->SetFileName(filename);
		m_pvtkXMLReader->Update();
		m_pvtkDataSet = (vtkDataSet*) m_pvtkXMLReader->GetOutputAsDataSet();
		std::cout << ".... done" << std::endl;
	} else if (suffix == std::string(".vtu")){
		std::cout << "attempt to read unstructured grid form xml file" << std::endl;
		m_pvtkXMLReader = vtkXMLUnstructuredGridReader::New();
		m_pvtkXMLReader->SetFileName(filename);
		m_pvtkXMLReader->Update();
		m_pvtkDataSet = (vtkDataSet*) m_pvtkXMLReader->GetOutputAsDataSet();
		std::cout << ".... done" << std::endl;
	} else if (suffix == std::string(".vtk")){
		std::cout << "attempt to read grid from legacy vtk file" << std::endl;	
		vtkDataReader *helpReader = vtkDataReader::New();
		helpReader->SetFileName(filename);
	
		if (helpReader->IsFileStructuredGrid()){
			std::cout << "file contains structured grid" << std::endl;
			m_pvtkDataReader = vtkStructuredGridReader::New();
			m_pvtkDataReader->SetFileName(filename);
			m_pvtkDataReader->Update();
			m_pvtkDataSet = (vtkDataSet*) ((vtkStructuredGridReader*) m_pvtkDataReader)->GetOutput();
		} else if (helpReader->IsFileUnstructuredGrid()){
			std::cout << "file contains unstructured grid" << std::endl;
			m_pvtkDataReader = vtkUnstructuredGridReader::New();
			m_pvtkDataReader->SetFileName(filename);
			m_pvtkDataReader->Update();
			m_pvtkDataSet = (vtkDataSet*) ((vtkUnstructuredGridReader*) m_pvtkDataReader)->GetOutput();
		} else if (helpReader->IsFileStructuredPoints()){
			std::cout << "file contains structured points" << std::endl;
			m_pvtkDataReader = vtkStructuredPointsReader::New();
			m_pvtkDataReader->SetFileName(filename);
			m_pvtkDataReader->Update();
			m_pvtkDataSet = (vtkDataSet*) ((vtkStructuredPointsReader*) m_pvtkDataReader)->GetOutput();
		} else {
			return false;
		}
	} else {
		std::cout << "CVectorGrid: could not read file" << filename << std::endl;
		return false;
	}

	// Checking if we have an active vector in the data. If not we try to find one.
	// If we can not find one we stop loading and return false
	vtkPointData *pointData = m_pvtkDataSet->GetPointData();
	vtkDataArray *vectorArray = pointData->GetVectors();
	if (vectorArray == NULL){
		int numArrays = pointData->GetNumberOfArrays();
		for (int i=0; i<numArrays; i++){
			vtkDataArray *anArray = pointData->GetArray(i);
			const char *arrayName = anArray->GetName();
			int numComponents = anArray->GetNumberOfComponents();
			std::cout << "	" << arrayName << " " << numComponents << std::endl;
			//Why does this not work and crashes?
			//vtkAbstractArray *anAbstractArray = (vtkAbstractArray*) anArray;
			//int arrayIsNumeric = anAbstractArray->IsNumeric();
			if ((numComponents == 3)){
				pointData->SetActiveVectors(arrayName);
				vectorArray = anArray;
				break;
			}
		}
		if (vectorArray == NULL) return false;
	} else {
		std::cout << "got a vector data array with name: " << vectorArray->GetName() << std::endl;
	}
	
	vtkDataArray *scalarArray = pointData->GetScalars();
	if (scalarArray == NULL){
		int numArrays = pointData->GetNumberOfArrays();
		for (int i=0; i<numArrays; i++){
			vtkDataArray *anArray = pointData->GetArray(i);
			const char *arrayName = anArray->GetName();
			int numComponents = anArray->GetNumberOfComponents();
			//Why does this not work and crashes?
			//vtkAbstractArray *anAbstractArray = (vtkAbstractArray*) anArray;
			//int arrayIsNumeric = anAbstractArray->IsNumeric();
			if ((numComponents == 1)){
				pointData->SetActiveScalars(arrayName);
				scalarArray = anArray;
				break;
			}
		}
		if (scalarArray == NULL){
			std::cout << "could not find scalar data for coloring" << std::endl;
		}
	} else {
		std::cout << "got a scalar data array with name: " << scalarArray->GetName() << std::endl;
	}
	
	
	// Retreving some resonable values for the initial state settings
	m_state.m_stdstrActiveVectorName = std::string(pointData->GetVectors()->GetName());
	
	if (scalarArray != NULL){
		m_state.m_stdstrActiveScalarName = std::string(scalarArray->GetName());
	} else {
		std::cout << "point data contain no scalars for coloring" << std::endl;
	}
		
	double bounds[6];
	m_pvtkDataSet->Update();
	m_pvtkDataSet->GetBounds(bounds);
	m_state.m_dXMin = bounds[0];
	m_state.m_dXMax = bounds[1];
	m_state.m_dYMin = bounds[2];
	m_state.m_dYMax = bounds[3];
	m_state.m_dZMin = bounds[4];
	m_state.m_dZMax = bounds[5];
	
	double sidelength = bounds[1] + bounds[3] + bounds[5] - bounds[0] - bounds[2] - bounds[4];
	m_state.m_dTubeRadius = 0.001 * sidelength;
	m_state.m_iNumPoints = m_pvtkDataSet->GetNumberOfPoints();
	if (m_state.m_iNumPoints > 10000){
		m_state.m_iGlyphMaskRatio = (int) m_state.m_iNumPoints / 10000;
	}
	
	double range[2], dx, dy, dz;
	vectorArray->GetRange(range, 0);
	dx = range[1] - range[0];
	vectorArray->GetRange(range, 0);
	dy = range[1] - range[0];
	vectorArray->GetRange(range, 0);
	dz = range[1] - range[0];
	m_state.m_dMaxVecLength = sqrt(dx * dx + dy * dy + dz * dz);
	m_state.m_dGlyphScaleFactor = (1.0 / m_state.m_dMaxVecLength) * 0.1 * sidelength;
	
	std::cout << "maximum vector length: " << m_state.m_dMaxVecLength << std::endl;
	std::cout << "glyph scale factor: " << m_state.m_dGlyphScaleFactor << std::endl;
	std::cout << "glyph mask ratio: " << m_state.m_iGlyphMaskRatio << std::endl;

	InitGlyphPipeline();
	InitStreamlinePipeline();
	
	return true;
}


void CVectorGrid::SetState(CVectorGridState state){
	bool bGlyphsChanged = false;
	bool bStreamlinesChanged = false;
	bool bRecomputeStreamlineSources = false;
	
	if (m_state.m_iGlyphMaskRatio != state.m_iGlyphMaskRatio){
		m_state.m_iGlyphMaskRatio = state.m_iGlyphMaskRatio;
		m_pvtkMaskPointsForGlyphs->SetOnRatio(m_state.m_iGlyphMaskRatio);
		bGlyphsChanged = true;
	}
	
	if (m_state.m_dGlyphScaleFactor != state.m_dGlyphScaleFactor){
		m_state.m_dGlyphScaleFactor = state.m_dGlyphScaleFactor;
		m_pvtkGlyph3D->SetScaleFactor(m_state.m_dGlyphScaleFactor);
		bGlyphsChanged = true;
	}
	
	if (m_state.m_bShowGlyphs != state.m_bShowGlyphs){
		m_state.m_bShowGlyphs = state.m_bShowGlyphs;
		NodePtr glyphsRoot = m_pGlyphActor->GetOsgRoot();
		beginEditCP(glyphsRoot, Node::TravMaskFieldMask);{
			if (m_state.m_bShowGlyphs){
				glyphsRoot->setTravMask(1);
			} else {
				glyphsRoot->setTravMask(0);
			}
		};endEditCP(glyphsRoot, Node::TravMaskFieldMask);
	}
	
	if (m_state.m_dStepLength != state.m_dStepLength){
		m_state.m_dStepLength = state.m_dStepLength;
		m_pvtkStreamer->SetIntegrationStepLength(m_state.m_dStepLength);
		
		vtkStreamLine *myStreamer = (vtkStreamLine*) m_pvtkStreamer;
		myStreamer->SetStepLength(m_state.m_dStepLength);
		//m_pvtkStreamer->SetStepLength(m_state.m_dStepLength);
		bStreamlinesChanged = true;
	}
	
	if (m_state.m_dMaxPropagationLength != state.m_dMaxPropagationLength){
		m_state.m_dMaxPropagationLength = state.m_dMaxPropagationLength;
		m_pvtkStreamer->SetMaximumPropagationTime(m_state.m_dMaxPropagationLength);
		bStreamlinesChanged = true;
	}
	
	if (m_state.m_dTubeRadius != state.m_dTubeRadius){
		m_state.m_dTubeRadius = state.m_dTubeRadius;
		m_pvtkTubeFilter->SetRadius(m_state.m_dTubeRadius);
		bStreamlinesChanged = true;
	}
	
	if (m_state.m_iIntegrationDirection != state.m_iIntegrationDirection){
		m_state.m_iIntegrationDirection = state.m_iIntegrationDirection;
		if (m_state.m_iIntegrationDirection == CVectorGridState::FORWARD){
			m_pvtkStreamer->SetIntegrationDirectionToForward();
		} else if (m_state.m_iIntegrationDirection == CVectorGridState::BACKWARD){
			m_pvtkStreamer->SetIntegrationDirectionToBackward();
		} else {
			m_pvtkStreamer->SetIntegrationDirectionToIntegrateBothDirections();
		}
		bStreamlinesChanged = true;
	}
	
	if (m_state.m_bShowStreamlines != state.m_bShowStreamlines){
		m_state.m_bShowStreamlines = state.m_bShowStreamlines;
		NodePtr streamlinesRoot = m_pStreamlinesActor->GetOsgRoot();
		beginEditCP(streamlinesRoot, Node::TravMaskFieldMask);{
			if (m_state.m_bShowStreamlines){
				streamlinesRoot->setTravMask(1);
			} else {
				streamlinesRoot->setTravMask(0);
			}
		};endEditCP(streamlinesRoot, Node::TravMaskFieldMask);
	}
	
	if (m_state.m_bUseGridAsSourceForStreamlines != state.m_bUseGridAsSourceForStreamlines){
		m_state.m_bUseGridAsSourceForStreamlines = state.m_bUseGridAsSourceForStreamlines;
		bRecomputeStreamlineSources = true;
	}
	
	if (m_state.m_iNumStreamlineSourcesXDir != state.m_iNumStreamlineSourcesXDir){
		m_state.m_iNumStreamlineSourcesXDir = state.m_iNumStreamlineSourcesXDir;
		bRecomputeStreamlineSources = true;
	}
	
	if (m_state.m_iNumStreamlineSourcesYDir != state.m_iNumStreamlineSourcesYDir){
		m_state.m_iNumStreamlineSourcesYDir = state.m_iNumStreamlineSourcesYDir;
		bRecomputeStreamlineSources = true;
	}
	
	if (m_state.m_iNumStreamlineSourcesZDir != state.m_iNumStreamlineSourcesZDir){
		m_state.m_iNumStreamlineSourcesZDir = state.m_iNumStreamlineSourcesZDir;
		bRecomputeStreamlineSources = true;
	}
	
	if (m_state.m_bUseGeomObjectAsSourceForStreamlines != state.m_bUseGeomObjectAsSourceForStreamlines){
		m_state.m_bUseGeomObjectAsSourceForStreamlines = state.m_bUseGeomObjectAsSourceForStreamlines;
		bRecomputeStreamlineSources = true;
	}
		
	if (m_state.m_stdstrStreamlinesGeomObjectName != state.m_stdstrStreamlinesGeomObjectName){
		m_state.m_stdstrStreamlinesGeomObjectName = state.m_stdstrStreamlinesGeomObjectName;
		bRecomputeStreamlineSources = true;
	}
	
	if (bRecomputeStreamlineSources){
		m_pvtkPointsForStreamer->Delete();
		m_pvtkPointsForStreamer = vtkPoints::New();
		
		if (m_state.m_bUseGridAsSourceForStreamlines){
			std::cout << "adding streamline sources on regular grid" << std::endl;
			AddStreamlineSourcesOnRegularGrid(
				m_state.m_iNumStreamlineSourcesXDir,
				m_state.m_iNumStreamlineSourcesYDir,
				m_state.m_iNumStreamlineSourcesZDir);
		}
		
		if (m_state.m_bUseGeomObjectAsSourceForStreamlines){
			AddStreamlineSourcesFromGeomObjectPoints(m_state.m_stdstrStreamlinesGeomObjectName);
		}
		
		m_pvtkPolyDataForStreamer->SetPoints(m_pvtkPointsForStreamer);
			
		bStreamlinesChanged = true;
	}
				
			
	
	
	if (bGlyphsChanged == true){
		m_pGlyphActor->UpdateOsg();
	}
	
	if (bStreamlinesChanged == true){
		m_pStreamlinesActor->UpdateOsg();
	}
}


CVectorGridState CVectorGrid::GetState(){
	return  m_state;
}


bool CVectorGrid::InitGlyphPipeline(){
	m_state.m_bShowGlyphs = true;
	
	m_pvtkMaskPointsForGlyphs = vtkMaskPoints::New();
	m_pvtkMaskPointsForGlyphs->SetInput(m_pvtkDataSet);
	m_pvtkMaskPointsForGlyphs->SetOnRatio(m_state.m_iGlyphMaskRatio);
	
	vtkArrowSource *arrowSource = vtkArrowSource::New();
	m_pvtkGlyph3D = vtkGlyph3D::New();
	m_pvtkGlyph3D->SetInput(m_pvtkMaskPointsForGlyphs->GetOutput());
	m_pvtkGlyph3D->SetSource(arrowSource->GetOutput());
	m_pvtkGlyph3D->SetScaleModeToScaleByVector();
	m_pvtkGlyph3D->SetColorModeToColorByScalar();
	m_pvtkGlyph3D->SetVectorModeToUseVector();
	m_pvtkGlyph3D->SetScaleFactor(m_state.m_dGlyphScaleFactor);
	
	vtkPolyDataMapper *polyDataMapper = vtkPolyDataMapper::New();
	double range[2];
	m_pvtkDataSet->GetPointData()->GetScalars()->GetRange(range);
	polyDataMapper->SetInput(m_pvtkGlyph3D->GetOutput());
	polyDataMapper->SetScalarRange(range);
	
	m_pGlyphActor = vtkOsgActor::New();
	m_pGlyphActor->SetVerbose(true);
	m_pGlyphActor->SetMapper(polyDataMapper);
	
	m_pGlyphActor->UpdateOsg();
	
	NodePtr	glyphRoot = m_pGlyphActor->GetOsgRoot();
	setName(glyphRoot, "glyphs");
	
	beginEditCP(m_posgRoot, Node::ChildrenFieldMask);{
		m_posgRoot->addChild(glyphRoot);
	};endEditCP(m_posgRoot, Node::ChildrenFieldMask);
	
	return true;
}


bool CVectorGrid::InitStreamlinePipeline(){
	//double xCenter = 0.5 * (m_state.m_dXMin + m_state.m_dXMax);
	//double yCenter = 0.5 * (m_state.m_dYMin + m_state.m_dYMax);
	//double zCenter = 0.5 * (m_state.m_dZMin + m_state.m_dZMax);
	//double dx = 0.2 * (m_state.m_dXMax - m_state.m_dXMin);
	//double dy = 0.2 * (m_state.m_dYMax - m_state.m_dYMin);
	//double dz = 0.2 * (m_state.m_dZMax - m_state.m_dZMin);
	//double xPos, yPos, zPos;
	
	m_state.m_bShowStreamlines = true;
	
	vtkRungeKutta4 *rungeKuttaIntegrator = vtkRungeKutta4::New();
	
	m_pvtkStreamer = vtkStreamLine::New();
	m_pvtkStreamer->SetInput(m_pvtkDataSet);
	if (m_state.m_iIntegrationDirection == CVectorGridState::FORWARD){
		m_pvtkStreamer->SetIntegrationDirectionToForward();
	} else if (m_state.m_iIntegrationDirection == CVectorGridState::BACKWARD){
		m_pvtkStreamer->SetIntegrationDirectionToBackward();
	} else {
		m_pvtkStreamer->SetIntegrationDirectionToIntegrateBothDirections();
	}
	m_pvtkStreamer->SetMaximumPropagationTime(m_state.m_dMaxPropagationLength);
	m_pvtkStreamer->SetIntegrationStepLength(m_state.m_dStepLength);
	m_pvtkStreamer->SetIntegrator(rungeKuttaIntegrator);
	
	m_pvtkPointsForStreamer = vtkPoints::New();
	
	//m_pvtkPointsForStreamer->InsertNextPoint(xCenter, yCenter, zCenter);
	
	m_pvtkPolyDataForStreamer = vtkPolyData::New();
	m_pvtkPolyDataForStreamer->SetPoints(m_pvtkPointsForStreamer);
	m_pvtkStreamer->SetSource(m_pvtkPolyDataForStreamer);
	
	m_state.m_bUseGridAsSourceForStreamlines = true;
	m_state.m_iNumStreamlineSourcesXDir = 2;
	m_state.m_iNumStreamlineSourcesXDir = 2;
	m_state.m_iNumStreamlineSourcesXDir = 2;
	AddStreamlineSourcesOnRegularGrid(2, 2, 2);	
	
	m_pvtkTubeFilter = vtkTubeFilter::New();
	m_pvtkTubeFilter->SetInput(m_pvtkStreamer->GetOutput());
	m_pvtkTubeFilter->SetRadius(m_state.m_dTubeRadius);
	m_pvtkTubeFilter->SetNumberOfSides(6);
	
	vtkPolyDataMapper *streamLinesMapper = vtkPolyDataMapper::New();
	streamLinesMapper->SetInput(m_pvtkTubeFilter->GetOutput());
	
	m_pStreamlinesActor = vtkOsgActor::New();
	m_pStreamlinesActor->SetMapper(streamLinesMapper);
	m_pStreamlinesActor->UpdateOsg();
	
	NodePtr streamLinesRoot = m_pStreamlinesActor->GetOsgRoot();
	setName(streamLinesRoot, "streamlines");
	beginEditCP(m_posgRoot, Node::ChildrenFieldMask);{
		m_posgRoot->addChild(streamLinesRoot);
	};endEditCP(m_posgRoot, Node::ChildrenFieldMask);
	
	return true;
}

void CVectorGrid::AddStreamlineSourcesOnRegularGrid(int numXPoints, int numYPoints, int numZPoints){
	if (numXPoints < 2) numXPoints = 2;
	if (numYPoints < 2) numYPoints = 2;
	if (numZPoints < 2) numZPoints = 2;
	
	double dXLength = m_state.m_dXMax - m_state.m_dXMin;
	double dYLength = m_state.m_dYMax - m_state.m_dYMin;
	double dZLength = m_state.m_dZMax - m_state.m_dZMin;
	
	double dx = dXLength / (double) (numXPoints - 1);
	double dy = dYLength / (double) (numYPoints - 1);
	double dz = dZLength / (double) (numZPoints - 1);
	
	for (int i=0; i<numXPoints; i++){
		for (int j=0; j<numYPoints; j++){
			for (int k=0; k<numZPoints; k++){
				double xPos = m_state.m_dXMin + dx * (double) i;
				double yPos = m_state.m_dYMin + dy * (double) j;
				double zPos = m_state.m_dZMin + dz * (double) k;
				
				if (i == 0) xPos += 0.1 * dx;
				if (j == 0) yPos += 0.1 * dy;
				if (k == 0) zPos += 0.1 * dz;
				
				if (i == (numXPoints - 1)) xPos -= 0.1 * dx;
				if (j == (numYPoints - 1)) yPos -= 0.1 * dy;
				if (k == (numZPoints - 1)) zPos -= 0.1 * dz; 
				
				std::cout << "adding point to : " << xPos << " " << yPos << " " << zPos << std::endl;
				m_pvtkPointsForStreamer->InsertNextPoint(xPos, yPos, zPos);
			}
		}
	}
}

void CVectorGrid::AddStreamlineSourcesFromGeomObjectPoints(std::string objectname){
	//find the root of the whole scene
	NodePtr sceneRoot = m_posgRoot;
	while (NullFC != sceneRoot->getParent()){
		sceneRoot = sceneRoot->getParent();
	}
	
	//find the geometry node with the given name and abort if it does not exist
	NodePtr geomNode = FindGeomNodeWithName(sceneRoot, objectname);
	if (geomNode == NullFC) return;
	
	//find the transformation matrix that transforms from the objects coord system
	//to the coord system of the streamlines
	Matrix mObjectToWorld;
	geomNode->getToWorld(mObjectToWorld);
	
	Matrix mStreamlinesToWorld;
	NodePtr streamlinesNode = m_pStreamlinesActor->GetOsgRoot();
	streamlinesNode->getToWorld(mStreamlinesToWorld);
	
	Matrix mWorldToStreamlines;
	mStreamlinesToWorld.inverse(mWorldToStreamlines);
	
	Matrix mObjectToStreamlines = mWorldToStreamlines;
	mObjectToStreamlines.mult(mObjectToWorld);
	
	//retreiving the point coordinates from the geometry node
	GeometryPtr geometry = GeometryPtr::dcast(geomNode->getCore());
	GeoPositionsPtr positions = geometry->getPositions();
	int numPoints = positions->getSize();
	std::cout << "Adding streamline sources from geometry object" << std::endl;
	std::cout << "Number of points: " << numPoints << std::endl;
	std::set<Vec3f> points;
	std::set<Vec3f>::iterator pointsIter;
	for (int i=0; i<numPoints; i++){
		Vec3f vec = positions->getValue(i);
		std::cout << "Inserting source point into set" << std::endl;
		points.insert(vec);
	}
	
	std::cout << "Number of unique points: " << points.size() << std::endl;
	for (pointsIter = points.begin(); pointsIter != points.end(); pointsIter++){
		Vec3f vec = *pointsIter;
		Pnt3f p(vec);
		Pnt3f pos;
		mObjectToStreamlines.mult(p, pos);
		std::cout << "adding streamline source: " << pos[0] << " " << pos[1] << " " << pos[2] << std::endl;
		m_pvtkPointsForStreamer->InsertNextPoint(pos[0], pos[1], pos[2]);
	}
		
}

OSG::NodePtr CVectorGrid::FindGeomNodeWithName(OSG::NodePtr node, std::string name){
	UInt32 iNumChildren = node->getNChildren();
	
	if (getName(node)){
		if (getName(node) == name){
			GeometryPtr geom = GeometryPtr::dcast(node->getCore());
			if (geom != NullFC){
				std::cout << "found a geometry node with name: " << name << std::endl;
				return node;
			}
		}
	}
	
	for (int i=0; i<iNumChildren; i++){
		NodePtr r = FindGeomNodeWithName(node->getChild(i), name);
		if (r != NullFC) return r;
	}
	
	return NullFC;
}