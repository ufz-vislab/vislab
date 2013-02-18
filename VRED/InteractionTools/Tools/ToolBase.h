#pragma once

#include <OpenSG/OSGBaseFunctions.h>
#include <OpenSG/OSGPerspectiveCamera.h>
#include <OpenSG/OSGViewport.h>
#include <OpenSG/OSGSceneFileHandler.h>
#include <OpenSG/OSGQuaternion.h>

#include <string>
#include <map>

#include "OpenSGToolsSceneManager.h"

//using namespace osg;

//Handled variables:
//CursorFile std::string

class CToolBase
{
protected:
	COpenSGToolsSceneManager	*m_pSceneManager;

	NodePtr						m_posg3DCursor;

	UInt32						m_uint32IntersectActionTraverseMask;
	UInt32						m_uint32CursorTraverseMask;

	std::map<std::string, std::string>	m_stdmapVarsAndTypes;

public:
	CToolBase(void);
	~CToolBase(void);

	//Indentifying the class
	virtual std::string							GetToolName(){return std::string("CToolBase");}
	virtual std::string							GetToolDescription(){return std::string("The base for all tools");}

	//Interface for setting and getting variables
	virtual std::vector<std::string>			GetAttrNames();
	virtual std::map<std::string, std::string>	GetAttrNamesAndTypes();
	virtual bool								SetAttr(std::string attr, std::string value);
	virtual bool								SetAttr(std::string attr, int value);
	virtual bool								SetAttr(std::string attr, double value);

	virtual void								Load3DCursor(const char* filename);
	virtual NodePtr								Get3DCursor(){return m_posg3DCursor;}

	virtual void								SetIntersectActionTravMask(UInt32 travMask);
	virtual void								Set3DCursorTravMask(UInt32 travMask);

	virtual void								SetSceneManager(COpenSGToolsSceneManager *mgr);
	virtual bool								UseWalkNavigatorMode();
	virtual bool								UseTrackballNavigatorMode();

	virtual void								CalcScreenProjection(Pnt3f point, Pnt2f &projection);
	Line										CalcViewRay(int x, int y);
	void										GetLocalUnityVectorsScreenProj(Matrix toWorld, Pnt3f pickedVolCenter,
													Vec2f &screenE1, Vec2f &screenE2, Vec2f &screenE3);

	NodePtr										GetParentTransformNode(NodePtr start);
	void										SetNear(double near);

protected:
	bool										AddAttrNameAndType(std::string attr, std::string type);
};
