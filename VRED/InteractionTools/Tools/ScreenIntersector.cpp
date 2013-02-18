#include "ScreenIntersector.h"

using namespace osg;

CScreenIntersector::CScreenIntersector(void){
}

CScreenIntersector::~CScreenIntersector(void){
}

bool CScreenIntersector::AddScreen(OSG::Vec3f lowerLeft, OSG::Vec3f upperLeft, OSG::Vec3f upperRight, OSG::Vec3f lowerRight, int id){
	CScreenConfig c(lowerLeft, upperLeft, upperRight, lowerRight, id);
	return (m_Screens.insert(std::map<int, CScreenConfig>::value_type(id, c))).second;
}

bool CScreenIntersector::RemoveScreen(int id){
	std::map<int, CScreenConfig>::iterator iter = m_Screens.find(id);
	if (iter != m_Screens.end()){
		m_Screens.erase(iter);
		return true;
	} else {
		return false;
	}
}

void CScreenIntersector::Clear(){
	m_Screens.clear();
}

bool CScreenIntersector::TestIntersectScreens(Pnt3f pos, Vec3f dir, int &foundIntersectionId){
	//std::cout << "testing screen intersections for:" << std::endl;
	//std::cout << "    pos: " << pos[0] << " " << pos[1] << " " << pos[2] << std::endl;
	//std::cout << "    dir: " << dir[0] << " " << dir[1] << " " << dir[2] << std::endl;

	std::map<int, CScreenConfig>::iterator iter;
	for (iter = m_Screens.begin(); iter != m_Screens.end(); iter++){
		//std::cout << "	Testing screen" << std::endl;
		bool ifIntersect = (*iter).second.TestIntersect(pos, dir);
		if (ifIntersect){
			foundIntersectionId = (*iter).second.m_iId;
			return true;
		}
	}
	foundIntersectionId = 0;
	return false;
}

void CScreenIntersector::ConfigForUFZDisplay(){
	//LEFT Screen
	this->AddScreen(Vec3f(-4.16, -1.42, -0.04),
		Vec3f(-4.16, 1.42, -0.04),
		Vec3f(-2.7, 1.42, -1.5),
		Vec3f(-2.7, -1.42, -1.5),
		CScreenIntersector::UFZ_LEFT);
	//m_osgvec3WindowLowerLeft = Vec3f(-4.16, -1.42, -0.04);
	//m_osgvec3WindowLowerRight = Vec3f(-2.7, -1.42, -1.5);
	//m_osgvec3WindowUpperRight = Vec3f(-2.7, 1.42, -1.5);
	//m_osgvec3WindowUpperLeft = Vec3f(-4.16, 1.42, -0.04);

	//Front Screen
	this->AddScreen(Vec3f(-2.7, -1.42, -1.5),
		Vec3f(-2.7, 1.42, -1.5),
		Vec3f(2.7, 1.42, -1.5),
		Vec3f(2.7, -1.42, -1.5),
		CScreenIntersector::UFZ_FRONT);
	//m_osgvec3WindowLowerLeft = Vec3f(-2.7, -1.42, -1.5);
	//m_osgvec3WindowLowerRight = Vec3f(2.7, -1.42, -1.5);
	//m_osgvec3WindowUpperRight = Vec3f(2.7, 1.42, -1.5);
	//m_osgvec3WindowUpperLeft = Vec3f(-2.7, 1.42, -1.5);

	//Right Screen
	this->AddScreen(Vec3f(2.7, -1.42, -1.5),
		Vec3f(2.7, 1.42, -1.5),
		Vec3f(4.16, 1.42, -0.04),
		Vec3f(4.16, -1.42, -0.04),
		CScreenIntersector::UFZ_RIGHT);
	//m_osgvec3WindowLowerLeft = Vec3f(2.7, -1.42, -1.5);
	//m_osgvec3WindowLowerRight = Vec3f(4.16, -1.42, -0.04);
	//m_osgvec3WindowUpperRight = Vec3f(4.16, 1.42, -0.04);
	//m_osgvec3WindowUpperLeft = Vec3f(2.7, 1.42, -1.5);

	//Floor Screen
	this->AddScreen(Vec3f(-2.7, -1.42, 0.0),
		Vec3f(-2.7, -1.42, -1.5),
		Vec3f(2.7, -1.42, -1.5),
		Vec3f(2.7, -1.42, 0.0),
		CScreenIntersector::UFZ_FLOOR);
	//m_osgvec3WindowLowerLeft = Vec3f(-2.7, -1.42, 0.0);
	//m_osgvec3WindowLowerRight = Vec3f(2.7, -1.42, 0.0);
	//m_osgvec3WindowUpperRight = Vec3f(2.7, -1.42, -1.5);
	//m_osgvec3WindowUpperLeft = Vec3f(-2.7, -1.42, -1.5);
}

CScreenIntersector::CScreenConfig::CScreenConfig(OSG::Vec3f lowerLeft, OSG::Vec3f upperLeft, OSG::Vec3f upperRight, OSG::Vec3f lowerRight, int id){
	m_osgvec3fLowerLeft = lowerLeft;
	m_osgvec3fUpperLeft = upperLeft;
	m_osgvec3fUpperRight = upperRight;
	m_osgvec3fLowerRight = lowerRight;
	m_iId = id;
}

bool CScreenIntersector::CScreenConfig::TestIntersect(Pnt3f pos, Vec3f dir){
	//Calculating the general representation of the plane and the point of intersection
	//of line and plane
	Vec3f a = m_osgvec3fLowerRight - m_osgvec3fLowerLeft;
	Vec3f b = m_osgvec3fUpperLeft - m_osgvec3fLowerLeft;
	//std::cout << "a: " << a[0] << " " << a[1] << " " << a[2] << std::endl;
	//std::cout << "b: " << b[0] << " " << b[1] << " " << b[2] << std::endl;
	Vec3f n;
	n[0] = a[1] * b[2] - a[2] * b[1];
	n[1] = a[2] * b[0] - a[0] * b[2];
	n[2] = a[0] * b[1] - a[1] * b[0];
	n.normalize();

	float d = -n.dot(m_osgvec3fLowerLeft);
	//std::cout << "n: " << n[0] << " " << n[1] << " " << n[2] << std::endl;
	//std::cout << "d: " << d << std::endl;

	//Assumptions for calculating the intersection point:
	//Description of the plane: <n, P(t)> + d> == 0
	//Description of the line: P(t) = pos + t * dir
	//Calculations see "Eric Lengyel, Mathematics for 3D Game Programming and
	//Computer Graphics, Page 107
	float t = - (n.dot(pos) + d) / n.dot(dir);
	//std::cout << "t: " << t << std::endl;

	Vec3f intersection = pos + t * dir;
	//std::cout << "Intersection point " << intersection[0] << " " << intersection[1] << " " << intersection[2] << std::endl;

	if (t < 0.0) return false;

	//P = m_osgvec3fLowerLeft + M * [u, v] with
	//     a0 b0
	// M = a1 b1
	//     a2 b2
	//This system is overdetermined, solved using P0/P1 and controlled using P2
	// [u,v] = MInv * (P - m_osgvec3fLowerLeft)
	// 
	// MInv = |b1    -b0|  * 1 / Det(M) 
	//        |-a1   a0 |

	Vec3f rightHand = intersection - m_osgvec3fLowerLeft;
	float DetM = a[0] * b[1] - a[1] * b[0];
	float u = (b[1] * rightHand[0] - b[0] * rightHand[1]) / DetM;
	float v = (-a[1] * rightHand[0] + a[0] * rightHand[1]) / DetM;

	if ((u >= 0.0) && (u <= 1.0) && (v >= 0.0) && (v <= 1.0)){
		//std::cout << "found an intersection" << std::endl;
		return true;
	} else {
		return false;
	}
}