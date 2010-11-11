// Dr. Bjoern Zehner 
// UFZ Centre for Environmental Research Leipzig-Halle 
// Permoserstrasse 15 
// 04318 Leipzig 
// Germany 

#pragma once
#include "ElevationGridBase.h"

#include <OpenSG/OSGConfig.h>
#include <OpenSG/OSGSimpleGeometry.h>
#include <OpenSG/OSGSceneFileHandler.h>
#include <OpenSG/OSGSimpleAttachments.h>
#include <OpenSG/OSGTriangleIterator.h>

OSG_USING_NAMESPACE

class CTriangleElevationGrid : public CElevationGridBase
{
private:
	class CTriangleParams;
private:
	NodePtr m_posgRoot;
	GeometryPtr m_posgGeo;

	std::list<CTriangleParams*> **m_cells;
	std::list<CTriangleParams*> m_triangles;

	int m_iNumXCells;
	int m_iNumYCells;

	double m_dXMin;
	double m_dYMin;
	double m_dDX;
	double m_dDY;
public:
	CTriangleElevationGrid(void);
	~CTriangleElevationGrid(void);

	bool LoadFile(const char *filename);
	bool LoadNode(NodePtr node);
	NodePtr GetRoot(){return m_posgRoot;}
	void GetXYExtension(double &xMin, double &yMin, double &xMax, double &yMax);
	void SortIntoGrid(int numXCells, int numYCells);
	void ClearCells();
	double GetZ(double x, double y);

	void TestGeometry();

private:
	GeometryPtr FindGeometry(NodePtr node);

private:
	class CTriangleParams {
	private:
		double m_dInv11;
		double m_dInv12;
		double m_dInv21;
		double m_dInv22;
		Pnt3f  m_osgP1;
		Pnt3f  m_osgP2;
		Pnt3f  m_osgP3;
	public:
		CTriangleParams(Pnt3f p1, Pnt3f p2, Pnt3f p3);
		~CTriangleParams();
		bool IsInTriangle(double x, double y);
		double GetZ(double x, double y);
	};
};
