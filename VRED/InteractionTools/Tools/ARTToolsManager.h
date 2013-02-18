#pragma once

#include "VrpnArtTrackingClient.h"
#include "ToolHolder.h"

#include <OpenSG/OSGTransform.h>

class CARTToolsManager
{
protected:
	static CARTToolsManager *m_pInstance;

	CARTToolsManager(void);

public:
	static CARTToolsManager *Instance();
	~CARTToolsManager(void);

	void SetProjectionCameraUserTransformPtr(osg::TransformPtr trans);

	void StartVRPNTracking(const char* deviceName);
	void StopVRPNTracking();
	void VRPNLoop();

	void SetDefaultTool(std::string name){
		std::cout << "new default tool set: " << name << std::endl;
		m_stdstrToolNames[0] = name;
	}
	void SetArtButton1ToolName(std::string name){
		std::cout << "new tool set for art button 1: " << name << std::endl;
		m_stdstrToolNames[1] = name;
	}
	void SetArtButton2ToolName(std::string name){
		std::cout << "new tool set for art button 2: " << name << std::endl;
		m_stdstrToolNames[2] = name;
	}
	void SetArtButton3ToolName(std::string name){
		std::cout << "new tool set for art button 3: " << name << std::endl;
		m_stdstrToolNames[3] = name;
	}
	void SetArtButton4ToolName(std::string name){
		std::cout << "new tool set for art button 4: " << name << std::endl;
		m_stdstrToolNames[4] = name;
	}

	std::string GetDefaultToolName(){return m_stdstrToolNames[0];}
	std::string GetArtButton1ToolName(){return m_stdstrToolNames[1];}
	std::string GetArtButton2ToolName(){return m_stdstrToolNames[2];}
	std::string GetArtButton3ToolName(){return m_stdstrToolNames[3];}
	std::string GetArtButton4ToolName(){return m_stdstrToolNames[4];}

	void UseTool(int index);

protected:
	std::string m_stdstrToolNames[5];

	//This should be the transformation node of the head for the ProjectionCameraDecorator
	osg::TransformPtr m_posgUserTransform;

	bool m_bVRPNButtonData[10];
};
