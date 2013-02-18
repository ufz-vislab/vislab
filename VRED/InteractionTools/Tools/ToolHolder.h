#pragma once

#include "ToolBase.h"
#include "MouseToolBase.h"
#include "Vr3DButtonToolBase.h"
#include "Vr3DAnalogToolBase.h"
#include "DefaultTool.h"
#include "OpenSGToolsSceneManager.h"

#include <OpenSG/OSGNode.h>
#include <OpenSG/OSGTransform.h>

#include <map>

//OSG_USING_NAMESPACE

class CToolHolder
{
protected:
	CToolBase*							m_pCurrentTool;
	CMouseToolBase*						m_pCurrentMouseTool;
	CVr3DButtonToolBase*				m_pCurrentVr3DButtonTool;
	CVr3DAnalogToolBase*				m_pCurrentVr3DAnalogTool;
	COpenSGToolsSceneManager*			m_pSceneManager;
	std::map<std::string, CToolBase*>	m_NameToToolMap;
	CToolHolder(void);

	static CToolHolder*					m_pInstance;

	NodePtr								m_posgCursorNode;
	TransformPtr						m_posgCursorTransform;

	UInt32								m_uint32IntersectActionTraverseMask;
	UInt32								m_uint32CursorTraverseMask;

public:
	~CToolHolder(void);

public:
	static								CToolHolder* Instance();

	void								SetSceneManager(COpenSGToolsSceneManager *mgr);
	void								SetTool(CToolBase *tool);

	TransformPtr						GetFlystickTransformPtr(){return m_posgCursorTransform;}

	void								SetIntersectActionTravMask(UInt32 travMask);
	void								Set3DCursorTravMask(UInt32 travMask);

	bool								AddTool(std::string name, CToolBase *tool);
	CToolBase*							RemoveTool(std::string name);
	int									GetNumberOfTools();
	CToolBase*							GetTool(std::string name);
	bool								UseTool(std::string name);
	CToolBase*							GetCurrentTool(){return m_pCurrentTool;}

	std::vector<std::string>			GetAllToolNames();
	void								ClearAndDeleteTools();


	void OnLeftMouseButtonDown(int x, int y){
		if (m_pCurrentMouseTool != NULL) m_pCurrentMouseTool->OnLeftMouseButtonDown(x, y);
	};

	void OnMiddleMouseButtonDown(int x, int y){
		if (m_pCurrentMouseTool != NULL) m_pCurrentMouseTool->OnMiddleMouseButtonDown(x, y);
	};

	void OnRightMouseButtonDown(int x, int y){
		if (m_pCurrentMouseTool != NULL) m_pCurrentMouseTool->OnRightMouseButtonDown(x, y);
	};

	void OnLeftMouseButtonUp(int x, int y){
		if (m_pCurrentMouseTool != NULL) m_pCurrentMouseTool->OnLeftMouseButtonUp(x, y);
	};

	void OnMiddleMouseButtonUp(int x, int y){
		if (m_pCurrentMouseTool != NULL) m_pCurrentMouseTool->OnMiddleMouseButtonUp(x, y);
	};

	void OnRightMouseButtonUp(int x, int y){
		if (m_pCurrentMouseTool != NULL) m_pCurrentMouseTool->OnRightMouseButtonUp(x, y);
	};

	void OnMouseMove(int x, int y){
		if (m_pCurrentMouseTool != NULL) m_pCurrentMouseTool->OnMouseMove(x, y);
	};

	void OnTrackingButtonPressed(Quaternion q, Vec3f position){
		if (m_pCurrentVr3DButtonTool != NULL) m_pCurrentVr3DButtonTool->OnTrackingButtonPressed(q, position);
	};

	void OnTrackingButtonReleased(Quaternion q, Vec3f position){
		if (m_pCurrentVr3DButtonTool != NULL) m_pCurrentVr3DButtonTool->OnTrackingButtonReleased(q, position);
	};

	void OnTrackingMove(Quaternion q, Vec3f position){
		if (m_pCurrentVr3DButtonTool != NULL) m_pCurrentVr3DButtonTool->OnTrackingMove(q, position);
	}

	void OnAnalog0(Quaternion q, Vec3f position, double analogValue){
		if (m_pCurrentVr3DAnalogTool != NULL) m_pCurrentVr3DAnalogTool->OnAnalog0(q, position, analogValue);
	};

	void OnAnalog1(Quaternion q, Vec3f position, double analogValue){
		if (m_pCurrentVr3DAnalogTool != NULL) m_pCurrentVr3DAnalogTool->OnAnalog1(q, position, analogValue);
	};
};
