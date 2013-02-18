#pragma once

#include "ART2DWindowInterface.h"

#include "ScreenIntersector.h"

#include <map>

class CART2DWindowManagerItem : public std::map<std::string, std::string> {
	//std::map<std::string, std::string> m_stdmapScreenToFilename;
public:
	void InsertView(std::string screen, std::string filename){
		insert(std::map<std::string, std::string>::value_type(screen, filename));
	}
};

class CART2DWindowManager
{
protected:
	static CART2DWindowManager *m_pInstance;
	CART2DWindowManager(void);
	~CART2DWindowManager(void);

	CART2DWindowInterface*	m_pInterfaceToLeftScreen;
	CART2DWindowInterface*	m_pInterfaceToRightScreen;

	std::map<std::string, CART2DWindowManagerItem> m_stdmapNodeNameToItem;

public:
	static CART2DWindowManager* Instance();

	void SetLeftScreenInterface(CART2DWindowInterface *screen){m_pInterfaceToLeftScreen = screen;}
	void SetRightScreenInterface(CART2DWindowInterface *screen){m_pInterfaceToRightScreen = screen;}

	void Move(int screen, int x, int y);
	void Zoom(int screen, double factor);
	bool Show(std::string nodename);

	void LoadNodeToVisMap(const char* filename);
	void WriteNodeToVisMapFileExample(const char* filename);
};
