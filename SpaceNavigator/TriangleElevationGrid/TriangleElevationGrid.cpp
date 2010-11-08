// Dr. Bjoern Zehner 
// UFZ Centre for Environmental Research Leipzig-Halle 
// Permoserstrasse 15 
// 04318 Leipzig 
// Germany 

#include "TriangleElevationGrid.h"

CTriangleElevationGrid::CTriangleElevationGrid(void){
	m_posgRoot = NullFC;
	m_posgGeo = NullFC;

	m_cells = NULL;
	m_iNumXCells = 0;
	m_iNumYCells = 0;
}

CTriangleElevationGrid::~CTriangleElevationGrid(void){
	ClearCells();
}

bool CTriangleElevationGrid::LoadFile(const char *filename){
	m_posgRoot = SceneFileHandler::the().read(filename, NULL);
	if (m_posgRoot != NullFC){
		m_posgGeo = FindGeometry(m_posgRoot);
		return true;
	}
	else
		return false;
}

void CTriangleElevationGrid::GetXYExtension(double &xMin, double &yMin, double &xMax, double &yMax){
	int i;
	if (m_posgGeo == NullFC){
		xMin = 0.0; yMin = 0.0; xMax = 0.0; yMax = 0.0;
		return;
	}

	TriangleIterator triIter(m_posgGeo);
	Pnt3f p = triIter.getPosition(0);
	xMin = p[0]; xMax = p[0];
	yMin = p[1]; yMax = p[1];

	for (i=0; i<3; i++){
		p = triIter.getPosition(i);
		if (p[0] < xMin) xMin = p[0];
		if (p[1] < yMin) yMin = p[1];
		if (p[0] > xMax) xMax = p[0];
		if (p[1] > yMax) yMax = p[1];
	}

	for (; ! triIter.isAtEnd(); ++triIter){
		for (i=0; i<3; i++){
			p = triIter.getPosition(i);
			if (p[0] < xMin) xMin = p[0];
			if (p[1] < yMin) yMin = p[1];
			if (p[0] > xMax) xMax = p[0];
			if (p[1] > yMax) yMax = p[1];
		}
	}
}

void CTriangleElevationGrid::SortIntoGrid(int numXCells, int numYCells){
	ClearCells();

	double xMin, yMin, xMax, yMax;
	m_iNumXCells = numXCells;
	m_iNumYCells = numYCells;

	GetXYExtension(xMin, yMin, xMax, yMax);
	xMin -= 1.0;
	yMin -= 1.0;
	xMax += 1.0;
	yMax += 1.0;
	std::cout << "extension: lower left, upper right: " << xMin << " " << yMin << "     " << xMax << " " << yMax << std::endl;
	double xLength = xMax - xMin;
	double yLength = yMax - yMin;
	std::cout << "xy extensions: " << xLength << " " << yLength << std::endl;
	double dx = xLength / (double) numXCells;
	double dy = yLength / (double) numYCells;
	std::cout << "dx, dy: " << dx << " " << dy << std::endl;

	m_dXMin = xMin;
	m_dYMin = yMin;
	m_dDX = dx;
	m_dDY = dy;

	int i, j;
	m_cells = new std::list<CTriangleParams*>*[numXCells];
	for (i=0; i<numXCells; i++){
		m_cells[i] = new std::list<CTriangleParams*>[numYCells];
	}

	TriangleIterator triIter(m_posgGeo);
	int trianglecounter = 0;
	for (; ! triIter.isAtEnd(); ++triIter){
		Pnt3f p0 = triIter.getPosition(0);
		Pnt3f p1 = triIter.getPosition(1);
		Pnt3f p2 = triIter.getPosition(2);

		int p0xind = (int) ((p0[0] - xMin) / dx);
		int p1xind = (int) ((p1[0] - xMin) / dx);
		int p2xind = (int) ((p2[0] - xMin) / dx);
		int xindMin = (p0xind < p1xind) ? p0xind : p1xind;
		xindMin = (xindMin < p2xind) ? xindMin : p2xind;
		int xindMax = (p0xind > p1xind) ? p0xind : p1xind;
		xindMax = (xindMax > p2xind) ? xindMax : p2xind;

		int p0yind = (int) ((p0[1] - yMin) / dy);
		int p1yind = (int) ((p1[1] - yMin) / dy);
		int p2yind = (int) ((p2[1] - yMin) / dy);

		int yindMin = (p0yind < p1yind) ? p0yind : p1yind;
		yindMin = (yindMin < p2yind) ? yindMin : p2yind;
		int yindMax = (p0yind > p1yind) ? p0yind : p1yind;
		yindMax = (yindMax > p2yind) ? yindMax : p2yind;

		CTriangleParams *tr = new CTriangleParams(p0, p1, p2);
		m_triangles.push_back(tr);

		for (i=xindMin; i<=xindMax; i++){
			for (j=yindMin; j<=yindMax; j++){
				m_cells[i][j].push_back(tr);
				trianglecounter++;
			}
		}
	}
	std::cout << "number of triangles: " << trianglecounter << std::endl;
	int minNumCells = 10000;
	int maxNumCells = 0;
	for (i=0; i<numXCells; i++){
		for (j=0; j<numYCells; j++){
			if (m_cells[i][j].size() < minNumCells) minNumCells = m_cells[i][j].size();
			if (m_cells[i][j].size() > maxNumCells) maxNumCells = m_cells[i][j].size();
		}
	}
	std::cout << "num cells min max: " << minNumCells << " " << maxNumCells << std::endl;
}

void CTriangleElevationGrid::ClearCells(){
	if (m_cells != NULL){
		for (int i=0; i<m_iNumXCells; i++){
			delete [] m_cells[i];
		}
		delete m_cells;
	}
	if (m_triangles.size() > 0){
		std::list<CTriangleParams*>::iterator iter;
		for (iter = m_triangles.begin(); iter != m_triangles.end(); iter++){
			delete *iter;
		}
		m_triangles.clear();
	}
	m_iNumXCells = 0;
	m_iNumYCells = 0;
}

double CTriangleElevationGrid::GetZ(double x, double y){
	int i = (x - m_dXMin) / m_dDX;
	int j = (y - m_dYMin) / m_dDY;

	if ((i<0) || (i>=m_iNumXCells) || (j<0) || (j>=m_iNumXCells)){
		return 0.0;
	}

	std::list<CTriangleParams*>::iterator iter;
	for (iter = m_cells[i][j].begin(); iter != m_cells[i][j].end(); iter++){
		if ((*iter)->IsInTriangle(x, y)){
			return (*iter)->GetZ(x, y);
		}
	}

	return -999.0;
}

void CTriangleElevationGrid::TestGeometry(){
	GeometryPtr geo = FindGeometry(m_posgRoot);
	if (geo != NullFC){
		std::cout << "found geometry" << std::endl;
	}
	int index = 0;
	TriangleIterator triIter(geo);
	for (;! triIter.isAtEnd(); ++triIter){
		std::cout << "index: " << index++ << std::endl;
	}
}

GeometryPtr CTriangleElevationGrid::FindGeometry(NodePtr node){
	int numChildren = node->getNChildren();
	GeometryPtr geo = GeometryPtr::dcast(node->getCore());
	if (geo != NullFC){
		return geo;
	}else{
		for (int i=0; i<numChildren; i++){
			geo = FindGeometry(node->getChild(i));
			if (geo != NullFC) return geo;
		}
	}
	return NullFC;
}


CTriangleElevationGrid::CTriangleParams::CTriangleParams(Pnt3f p1, Pnt3f p2, Pnt3f p3){
	// P = P1 + lambda1 * (P2-P1) + lambda2 * (P3-P1)
	// P-P1 = [(P2-P1) (P3-P1)] * [lambda1 lambda2]
	// [lambda1 lambda2] = INV_Matrix * [(P-P1)]
	//
	m_osgP1 = p1;
	m_osgP2 = p2;
	m_osgP3 = p3;

	double a11 = p2[0] - p1[0];
	double a12 = p3[0] - p1[0];
	double a21 = p2[1] - p1[1];
	double a22 = p3[1] - p1[1];
	double det = a11 * a22 - a12 * a21;

	m_dInv11 = a22 / det;
	m_dInv12 = (-a12) / det;
	m_dInv21 = (-a21) / det;
	m_dInv22 = a11 / det;
}

CTriangleElevationGrid::CTriangleParams::~CTriangleParams(){
}

bool CTriangleElevationGrid::CTriangleParams::IsInTriangle(double x, double y){
	double dx = x - m_osgP1[0];
	double dy = y - m_osgP1[1];
	double lambda1 = m_dInv11 * dx + m_dInv12 * dy;
	double lambda2 = m_dInv21 * dx + m_dInv22 * dy;

	if ((lambda1 >= 0) && (lambda1 <=1) && (lambda2 >= 0) && (lambda2 <= 1) && (1 >= (lambda1 + lambda2))){
		return true;
	} else {
		return false;
	}
}

double CTriangleElevationGrid::CTriangleParams::GetZ(double x, double y){
	double dx = x - m_osgP1[0];
	double dy = y - m_osgP1[1];
	double lambda1 = m_dInv11 * dx + m_dInv12 * dy;
	double lambda2 = m_dInv21 * dx + m_dInv22 * dy;

	return (m_osgP1[2] + lambda1 * (m_osgP2[2] - m_osgP1[2]) + lambda2 * (m_osgP3[2] - m_osgP1[2]));
}
