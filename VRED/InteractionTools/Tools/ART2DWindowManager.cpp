#include "ART2DWindowManager.h"

#include "tinyxml.h"

CART2DWindowManager* CART2DWindowManager::m_pInstance = NULL;

CART2DWindowManager::CART2DWindowManager(void){
	m_pInterfaceToLeftScreen = NULL;
	m_pInterfaceToRightScreen = NULL;
}

CART2DWindowManager::~CART2DWindowManager(void){
}

CART2DWindowManager* CART2DWindowManager::Instance(){
	if (m_pInstance == NULL){
		m_pInstance = new CART2DWindowManager();
	}
	return m_pInstance;
}

void CART2DWindowManager::Move(int screen, int x, int y){
	if ((m_pInterfaceToLeftScreen != NULL) && (screen == CScreenIntersector::UFZ_LEFT)){
		m_pInterfaceToLeftScreen->Move(x, y);
	} else if ((m_pInterfaceToRightScreen != NULL) && (screen == CScreenIntersector::UFZ_RIGHT)){
		m_pInterfaceToRightScreen->Move(x, y);
	}
}

void CART2DWindowManager::Zoom(int screen, double factor){
	if ((m_pInterfaceToLeftScreen != NULL) && (screen == CScreenIntersector::UFZ_LEFT)){
		m_pInterfaceToLeftScreen->Zoom(factor);
	} else if ((m_pInterfaceToRightScreen != NULL) && (screen == CScreenIntersector::UFZ_RIGHT)){
		m_pInterfaceToRightScreen->Zoom(factor);
	}
}

bool CART2DWindowManager::Show(std::string nodename){
	std::map<std::string, CART2DWindowManagerItem>::iterator itemIter;
	itemIter = m_stdmapNodeNameToItem.find(nodename);
	if (itemIter == m_stdmapNodeNameToItem.end()){
		std::cout << "CART2DWindowManager::Show(node=" << nodename << "), no item to show for this node!" << std::endl;
		return false;
	} else {
		std::cout << "CART2DWindowManager::Show(node=" << nodename << "), showing item for node!" << std::endl;
	}

	CART2DWindowManagerItem item = (*itemIter).second;
	std::map<std::string, std::string>::iterator fileIter;

	bool ifScreenError;
	bool ifError = false;
	std::string errorString;

	for (fileIter = item.begin(); fileIter != item.end(); fileIter++){
		ifScreenError = false;
		std::string screen = (*fileIter).first;
		std::string filename = (*fileIter).second;
		std::cout << "   displaying item " << filename << " on screen " << screen << std::endl;
		if (screen == std::string("left")){
			if (m_pInterfaceToLeftScreen != NULL){
				m_pInterfaceToLeftScreen->Show2DViewFor(filename);
			} else {
				ifScreenError = true;
			}
		} else if (screen == std::string("right")){
			if (m_pInterfaceToRightScreen != NULL){
				m_pInterfaceToRightScreen->Show2DViewFor(filename);
			} else {
				ifScreenError = true;
			}
		} else {
			ifScreenError = true;
		}
		if (ifScreenError){
			errorString += std::string("Request for displaying item on unknown screen") + screen + std::string("\n");
			ifError = true;
		}
	}

	if (ifError){
		std::cout << "CART2DWindowManager::Show(nodename = " << nodename << ") error:" << std::endl;
		std::cout << errorString << std::endl;
		return false;
	}

	return true;
}

void CART2DWindowManager::LoadNodeToVisMap(const char* filename){
	TiXmlDocument *doc = new TiXmlDocument(filename);
	if (doc->LoadFile()){
		std::cout << "LoadNodeToVisMapFile(), could load file: " << filename << std::endl;
	} else {
		std::cout << "LoadNodeToVisMapFile(), could not load file: " << filename << std::endl;
		return;
	}

	TiXmlElement *root = doc->FirstChildElement();
	TiXmlElement *viewItems = root->FirstChildElement("ViewItems");

	if (viewItems != NULL){
		for (TiXmlElement *viewItem = viewItems->FirstChildElement(); viewItem != NULL; viewItem = viewItem->NextSiblingElement()){
			std::string strViewItemElementName = std::string(viewItem->Value());
			const char* pcNodeName = viewItem->Attribute("nodename");
			if ((pcNodeName != NULL) && (strViewItemElementName == std::string("ViewItem"))){
				std::string strNodeName = std::string(pcNodeName);
				std::cout << "Found view item for node: " << strNodeName << std::endl;
				CART2DWindowManagerItem windowManagerItem;
				for (TiXmlElement *view = viewItem->FirstChildElement(); view != NULL; view = view->NextSiblingElement()){
					std::string strViewElementName = std::string(view->Value());
					const char* screenName = view->Attribute("screen");
					const char* fileName = view->Attribute("file");
					if ((strViewElementName == std::string("View")) && (screenName != NULL) && (fileName != NULL)){
						std::string strScreenName = std::string(screenName);
						std::string strFileName = std::string(fileName);
						std::cout << "	found view for screen/file: " << strScreenName << " " << strFileName << std::endl;
						windowManagerItem.InsertView(strScreenName, strFileName);
					}
				}
				m_stdmapNodeNameToItem.insert(std::map<std::string, CART2DWindowManagerItem>::value_type(strNodeName, windowManagerItem));
			}
		}
	}
}

void CART2DWindowManager::WriteNodeToVisMapFileExample(const char* filename){
	TiXmlDocument doc;
	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	TiXmlElement *root = new TiXmlElement("Root");
	doc.LinkEndChild(root);

	TiXmlElement *viewItems = new TiXmlElement("ViewItems");
	root->LinkEndChild(viewItems);

	TiXmlElement *viewItem;
	TiXmlElement *view;

	viewItem = new TiXmlElement("ViewItem");
	viewItem->SetAttribute("nodename", "some_node_name");
	view = new TiXmlElement("View");
	view->SetAttribute("screen", "left");
	view->SetAttribute("file", "someFilePathForLeft.jpg");
	viewItem->LinkEndChild(view);
	view = new TiXmlElement("View");
	view->SetAttribute("screen", "right");
	view->SetAttribute("file", "someFilePathForRight.jpg");
	viewItem->LinkEndChild(view);
	viewItems->LinkEndChild(viewItem);

	viewItem = new TiXmlElement("ViewItem");
	viewItem->SetAttribute("nodename", "some_node_name");
	view = new TiXmlElement("View");
	view->SetAttribute("screen", "right");
	view->SetAttribute("file", "someFilePathForRight.jpg");
	viewItem->LinkEndChild(view);
	viewItems->LinkEndChild(viewItem);

	viewItem = new TiXmlElement("ViewItem");
	viewItem->SetAttribute("nodename", "some_node_name");
	view = new TiXmlElement("View");
	view->SetAttribute("screen", "left");
	view->SetAttribute("file", "someOtherFilePath.jpg");
	viewItem->LinkEndChild(view);

	viewItems->LinkEndChild(viewItem);

	viewItem = new TiXmlElement("ViewItem");
	viewItem->SetAttribute("nodename", "some_node_name");
	view = new TiXmlElement("View");
	view->SetAttribute("screen", "left");
	view->SetAttribute("file", "someEvenOtherFilePath.jpg");
	viewItem->LinkEndChild(view);
	viewItems->LinkEndChild(viewItem);

	doc.SaveFile(filename);
}
