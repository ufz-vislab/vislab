#pragma once

#include <map>
#include <OpenSG/OSGConfig.h>
#include <OpenSG/OSGSimpleGeometry.h>

class CScreenIntersector
{
protected:
	class CScreenConfig {
	public:
		CScreenConfig(){};
		CScreenConfig(OSG::Vec3f lowerLeft, OSG::Vec3f upperLeft, OSG::Vec3f upperRight, OSG::Vec3f lowerRight, int id);

		OSG::Vec3f	m_osgvec3fLowerLeft;
		OSG::Vec3f	m_osgvec3fUpperLeft;
		OSG::Vec3f	m_osgvec3fUpperRight;
		OSG::Vec3f	m_osgvec3fLowerRight;
		int			m_iId;

		bool TestIntersect(OSG::Pnt3f pos, OSG::Vec3f dir);
	};

	std::map<int, CScreenConfig> m_Screens;

public:
	CScreenIntersector(void);
	~CScreenIntersector(void);

	bool AddScreen(OSG::Vec3f lowerLeft, OSG::Vec3f upperLeft, OSG::Vec3f upperRight, OSG::Vec3f lowerRight, int id);
	bool RemoveScreen(int id);
	void Clear();
	bool TestIntersectScreens(OSG::Pnt3f pos, OSG::Vec3f dir, int &foundIntersectionId);

	enum {UFZ_NONE = 0, UFZ_LEFT = 1, UFZ_FRONT = 2, UFZ_RIGHT = 3, UFZ_FLOOR = 4};
	void ConfigForUFZDisplay();
};