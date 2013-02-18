#include "XmlToolLoader.h"

#include <iostream>
#include <string>
#include "tinyxml.h"

#include "ToolHolder.h"
#include "ToolFactory.h"
#include "ARTToolsManager.h"

CXmlToolLoader::CXmlToolLoader(void)
{
}

CXmlToolLoader::~CXmlToolLoader(void)
{
}

void CXmlToolLoader::LoadFile(const char *filename){
	CToolHolder *toolHolder = CToolHolder::Instance();
	CToolFactory *toolFactory = CToolFactory::Instance();
	CARTToolsManager *artToolManager = CARTToolsManager::Instance();

	toolHolder->ClearAndDeleteTools();

	TiXmlDocument *doc = new TiXmlDocument(filename);
	if (doc->LoadFile()){
		std::cout << "LoadToolsFile(), could load file: " << filename << std::endl;
	} else {
		std::cout << "LoadToolsFile(), could not load file: " << filename << std::endl;
		return;
	}

	TiXmlElement *root = doc->FirstChildElement();
	TiXmlElement *toolsNode = root->FirstChildElement("Tools");
	TiXmlElement *flystickNode = root->FirstChildElement("ARTFlyStick");

	if (toolsNode != NULL){
		for (TiXmlElement *tool = toolsNode->FirstChildElement(); tool != NULL; tool = tool->NextSiblingElement()){
			const char *elementName = tool->Value();
			std::string strElementName;
			if (elementName != NULL) strElementName = std::string(elementName);
			if (strElementName == std::string("Tool")){
				bool isValidTool = false;
				const char *toolName = tool->Attribute("name");
				const char *toolType = tool->Attribute("type");
				if ((toolName != NULL) && (toolType != NULL)) isValidTool = true;

				CToolBase *toolBase;
				if (isValidTool){
					toolBase = toolFactory->GenerateTool(std::string(toolType));
					if (toolBase == NULL) isValidTool = false;
				}

				if (isValidTool){
					toolHolder->AddTool(std::string(toolName), toolBase);
					std::cout << "New tool: " << toolName << "  of type: " << toolType << std::endl;
					for (TiXmlElement *var = tool->FirstChildElement(); var != NULL; var = var->NextSiblingElement()){
						const char* varElementName = var->Value();
						std::string strVarElementName;
						if (varElementName != NULL) strVarElementName = std::string(varElementName);
						if (strVarElementName == std::string("ToolAttr")){
							const char* varName = var->Attribute("name");
							const char* varValue = var->Attribute("value");
							if ((varName != NULL) && (varValue != NULL)){
								std::cout << "	var: " << varName << "  " << varValue << std::endl;
								toolBase->SetAttr(std::string(varName), std::string(varValue));
							}
						}
					}
				} else {
					std::cout << "Could not create tool: " << toolName << "  of type: ";
					if (toolType != NULL){
						std::cout << toolType << std::endl;
					} else {
						std::cout << "type has not been given!" << std::endl;
					}
				}
			}
		}
	}

	if (flystickNode != NULL){
		for (TiXmlElement *buttons = flystickNode->FirstChildElement(); buttons != NULL; buttons = buttons->NextSiblingElement()){
			const char *flystickElementName = buttons->Value();
			std::string strFlystickElementName;
			if (flystickElementName != NULL)strFlystickElementName = std::string(flystickElementName);
			if (strFlystickElementName == std::string("FlystickButton")){
				const char* numberAsChar = buttons->Attribute("number");
				const char* toolName = buttons->Attribute("toolname");
				if ((numberAsChar != NULL) && (toolName != NULL)){
					int number = atoi(buttons->Attribute("number"));
					std::cout << "Flystick button number " << number << " is tool " << toolName << std::endl;
					if (number == 0){
						artToolManager->SetDefaultTool(std::string(toolName));
					} else if (number == 1){
						artToolManager->SetArtButton1ToolName(std::string(toolName));
					} else if (number == 2){
						artToolManager->SetArtButton2ToolName(std::string(toolName));
					} else if (number == 3){
						artToolManager->SetArtButton3ToolName(std::string(toolName));
					} else if (number == 4){
						artToolManager->SetArtButton4ToolName(std::string(toolName));
					}
				}
			}
		}
	}
}

void CXmlToolLoader::GenerateDefaultFile(const char *filename){
	TiXmlDocument doc;
	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	TiXmlElement *root = new TiXmlElement("Root");
	doc.LinkEndChild(root);

	TiXmlElement *tools = new TiXmlElement("Tools");
	root->LinkEndChild(tools);

	TiXmlElement *someXmlElement;

	//CFlystickPickTool
	TiXmlElement *flystickPickTool = new TiXmlElement("Tool");
	flystickPickTool->SetAttribute("name", "MyPickTool");
	flystickPickTool->SetAttribute("type", "CFlystickPickTool");
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "CursorFile");
	someXmlElement->SetAttribute("value", "some/file/name.osb");
	flystickPickTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "MaxPickDistance");
	someXmlElement->SetAttribute("value", "1.0");
	flystickPickTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "PickOnlyNodesInList");
	someXmlElement->SetAttribute("value", "false");
	flystickPickTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "AddNodeNameToPickableList");
	someXmlElement->SetAttribute("value", "some_node_name");
	flystickPickTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "AddNodeNameToPickableList");
	someXmlElement->SetAttribute("value", "some_other_node_name");
	flystickPickTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "AddNodeNameToPickableList");
	someXmlElement->SetAttribute("value", "some_different_node_name");
	flystickPickTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "RemoveNodeNameFromPickableList");
	someXmlElement->SetAttribute("value", "some_node_name");
	flystickPickTool->LinkEndChild(someXmlElement);
	tools->LinkEndChild(flystickPickTool);

	//CFlystickTranslateTool
	TiXmlElement *flystickTranslateTool = new TiXmlElement("Tool");
	flystickTranslateTool->SetAttribute("name", "MyTranslateTool");
	flystickTranslateTool->SetAttribute("type", "CFlystickTranslateTool");
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "CursorFile");
	someXmlElement->SetAttribute("value", "some/file/name.osb");
	flystickTranslateTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "MaxPickDistance");
	someXmlElement->SetAttribute("value", "1.0");
	flystickTranslateTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "PickOnlyNodesInList");
	someXmlElement->SetAttribute("value", "false");
	flystickTranslateTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "AddNodeNameToPickableList");
	someXmlElement->SetAttribute("value", "some_node_name");
	flystickTranslateTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "AddNodeNameToPickableList");
	someXmlElement->SetAttribute("value", "some_other_node_name");
	flystickTranslateTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "AddNodeNameToPickableList");
	someXmlElement->SetAttribute("value", "some_different_node_name");
	flystickTranslateTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "RemoveNodeNameFromPickableList");
	someXmlElement->SetAttribute("value", "some_node_name");
	flystickTranslateTool->LinkEndChild(someXmlElement);
	tools->LinkEndChild(flystickTranslateTool);

	//CFlystickTranslationResetTool
	TiXmlElement *flystickTranslationResetTool = new TiXmlElement("Tool");
	flystickTranslationResetTool->SetAttribute("name", "MyTranslationResetTool");
	flystickTranslationResetTool->SetAttribute("type", "CFlystickTranslationResetTool");
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "CursorFile");
	someXmlElement->SetAttribute("value", "some/file/name.osb");
	flystickTranslationResetTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "MaxPickDistance");
	someXmlElement->SetAttribute("value", "1.0");
	flystickTranslationResetTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "PickOnlyNodesInList");
	someXmlElement->SetAttribute("value", "false");
	flystickTranslationResetTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "AddNodeNameToPickableList");
	someXmlElement->SetAttribute("value", "some_node_name");
	flystickTranslationResetTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "AddNodeNameToPickableList");
	someXmlElement->SetAttribute("value", "some_other_node_name");
	flystickTranslationResetTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "AddNodeNameToPickableList");
	someXmlElement->SetAttribute("value", "some_different_node_name");
	flystickTranslationResetTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "RemoveNodeNameFromPickableList");
	someXmlElement->SetAttribute("value", "some_node_name");
	flystickTranslationResetTool->LinkEndChild(someXmlElement);
	tools->LinkEndChild(flystickTranslationResetTool);

	//CFlystickWalkNavigationTool
	TiXmlElement *flystickWalkNavigationTool = new TiXmlElement("Tool");
	flystickWalkNavigationTool->SetAttribute("name", "MyWalkNavTool");
	flystickWalkNavigationTool->SetAttribute("type", "CFlystickWalkNavigationTool");
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "FlySpeed");
	someXmlElement->SetAttribute("value", "1.0");
	flystickWalkNavigationTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "RotationSpeed");
	someXmlElement->SetAttribute("value", "1.0");
	flystickWalkNavigationTool->LinkEndChild(someXmlElement);
	tools->LinkEndChild(flystickWalkNavigationTool);

	//CFlystickCageFilterBBoxTool for bounding boxes
	TiXmlElement *flystickCageFilterBBoxTool = new TiXmlElement("Tool");
	flystickCageFilterBBoxTool->SetAttribute("name", "MyCageFilterBBoxTool");
	flystickCageFilterBBoxTool->SetAttribute("type", "CFlystickCageFilterBBoxTool");
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "MaxPickDistance");
	someXmlElement->SetAttribute("value", "1.0");
	flystickCageFilterBBoxTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "InteractionType");
	someXmlElement->SetAttribute("value", "BoundingBox");
	flystickCageFilterBBoxTool->LinkEndChild(someXmlElement);
	tools->LinkEndChild(flystickCageFilterBBoxTool);

	//CFlystickCageFilterBBoxTool for clip planes
	TiXmlElement *flystickCageFilterClipPlanesTool = new TiXmlElement("Tool");
	flystickCageFilterClipPlanesTool->SetAttribute("name", "MyCageFilterClipPlanesTool");
	flystickCageFilterClipPlanesTool->SetAttribute("type", "CFlystickCageFilterBBoxTool");
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "MaxPickDistance");
	someXmlElement->SetAttribute("value", "1.0");
	flystickCageFilterClipPlanesTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "InteractionType");
	someXmlElement->SetAttribute("value", "ClipPlanes");
	flystickCageFilterClipPlanesTool->LinkEndChild(someXmlElement);
	tools->LinkEndChild(flystickCageFilterClipPlanesTool);

	//CFlystickWalkCageFilterTool for bounding boxes
	TiXmlElement *flystickWalkCageFilterBBoxTool = new TiXmlElement("Tool");
	flystickWalkCageFilterBBoxTool->SetAttribute("name", "MyWalkCageFilterBBoxTool");
	flystickWalkCageFilterBBoxTool->SetAttribute("type", "CFlystickWalkCageFilterTool");
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "FlySpeed");
	someXmlElement->SetAttribute("value", "1.0");
	flystickWalkCageFilterBBoxTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "RotationSpeed");
	someXmlElement->SetAttribute("value", "1.0");
	flystickWalkCageFilterBBoxTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "MaxPickDistance");
	someXmlElement->SetAttribute("value", "1.0");
	flystickWalkCageFilterBBoxTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "InteractionType");
	someXmlElement->SetAttribute("value", "BoundingBox");
	flystickWalkCageFilterBBoxTool->LinkEndChild(someXmlElement);
	tools->LinkEndChild(flystickWalkCageFilterBBoxTool);

	//CFlystickWalkCageFilterTool for clip planes
	TiXmlElement *flystickWalkCageFilterClipPlanesTool = new TiXmlElement("Tool");
	flystickWalkCageFilterClipPlanesTool->SetAttribute("name", "MyWalkCageFilterClipPlanesTool");
	flystickWalkCageFilterClipPlanesTool->SetAttribute("type", "CFlystickWalkCageFilterTool");
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "FlySpeed");
	someXmlElement->SetAttribute("value", "1.0");
	flystickWalkCageFilterClipPlanesTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "RotationSpeed");
	someXmlElement->SetAttribute("value", "1.0");
	flystickWalkCageFilterClipPlanesTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "MaxPickDistance");
	someXmlElement->SetAttribute("value", "1.0");
	flystickWalkCageFilterClipPlanesTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "InteractionType");
	someXmlElement->SetAttribute("value", "ClipPlanes");
	flystickWalkCageFilterClipPlanesTool->LinkEndChild(someXmlElement);
	tools->LinkEndChild(flystickWalkCageFilterClipPlanesTool);

	//CFlystickWalkNavigationPickTool
	TiXmlElement *flystickWalkNavPickTool = new TiXmlElement("Tool");
	flystickWalkNavPickTool->SetAttribute("name", "MyWalkNavPickTool");
	flystickWalkNavPickTool->SetAttribute("type", "CFlystickWalkNavigationPickTool");
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "CursorFile");
	someXmlElement->SetAttribute("value", "some/file/name.osb");
	flystickWalkNavPickTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "MaxPickDistance");
	someXmlElement->SetAttribute("value", "1.0");
	flystickWalkNavPickTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "PickOnlyNodesInList");
	someXmlElement->SetAttribute("value", "false");
	flystickWalkNavPickTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "AddNodeNameToPickableList");
	someXmlElement->SetAttribute("value", "some_node_name");
	flystickWalkNavPickTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "AddNodeNameToPickableList");
	someXmlElement->SetAttribute("value", "some_other_node_name");
	flystickWalkNavPickTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "AddNodeNameToPickableList");
	someXmlElement->SetAttribute("value", "some_different_node_name");
	flystickWalkNavPickTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "RemoveNodeNameFromPickableList");
	someXmlElement->SetAttribute("value", "some_node_name");
	flystickWalkNavPickTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("TooAttr");
	someXmlElement->SetAttribute("name", "FlySpeed");
	someXmlElement->SetAttribute("value", "1.0");
	flystickWalkNavPickTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "RotationSpeed");
	someXmlElement->SetAttribute("value", "1.0");
	flystickWalkNavPickTool->LinkEndChild(someXmlElement);
	tools->LinkEndChild(flystickWalkNavPickTool);

	//CFlystickWalkNavigationTranslateTool
	TiXmlElement *flystickWalkNavTranslateTool = new TiXmlElement("Tool");
	flystickWalkNavTranslateTool->SetAttribute("name", "MyWalkNavTranslateTool");
	flystickWalkNavTranslateTool->SetAttribute("type", "CFlystickWalkNavigationTranslateTool");
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "CursorFile");
	someXmlElement->SetAttribute("value", "some/file/name.osb");
	flystickWalkNavTranslateTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "MaxPickDistance");
	someXmlElement->SetAttribute("value", "1.0");
	flystickWalkNavTranslateTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "PickOnlyNodesInList");
	someXmlElement->SetAttribute("value", "false");
	flystickWalkNavTranslateTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "AddNodeNameToPickableList");
	someXmlElement->SetAttribute("value", "some_node_name");
	flystickWalkNavTranslateTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "AddNodeNameToPickableList");
	someXmlElement->SetAttribute("value", "some_other_node_name");
	flystickWalkNavTranslateTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "AddNodeNameToPickableList");
	someXmlElement->SetAttribute("value", "some_different_node_name");
	flystickWalkNavTranslateTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "RemoveNodeNameFromPickableList");
	someXmlElement->SetAttribute("value", "some_node_name");
	flystickWalkNavTranslateTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "FlySpeed");
	someXmlElement->SetAttribute("value", "1.0");
	flystickWalkNavTranslateTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "RotationSpeed");
	someXmlElement->SetAttribute("value", "1.0");
	flystickWalkNavTranslateTool->LinkEndChild(someXmlElement);
	tools->LinkEndChild(flystickWalkNavTranslateTool);

	//CFlystickWalkNavigationTranslationResetTool
	TiXmlElement *flystickWalkNavTranslationResetTool = new TiXmlElement("Tool");
	flystickWalkNavTranslationResetTool->SetAttribute("name", "MyWalkNavTranslationResetTool");
	flystickWalkNavTranslationResetTool->SetAttribute("type", "CFlystickWalkNavigationTranslationResetTool");
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "CursorFile");
	someXmlElement->SetAttribute("value", "some/file/name.osb");
	flystickWalkNavTranslationResetTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "MaxPickDistance");
	someXmlElement->SetAttribute("value", "1.0");
	flystickWalkNavTranslationResetTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "PickOnlyNodesInList");
	someXmlElement->SetAttribute("value", "false");
	flystickWalkNavTranslationResetTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "AddNodeNameToPickableList");
	someXmlElement->SetAttribute("value", "some_node_name");
	flystickWalkNavTranslationResetTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "AddNodeNameToPickableList");
	someXmlElement->SetAttribute("value", "some_other_node_name");
	flystickWalkNavTranslationResetTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "AddNodeNameToPickableList");
	someXmlElement->SetAttribute("value", "some_different_node_name");
	flystickWalkNavTranslationResetTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "RemoveNodeNameFromPickableList");
	someXmlElement->SetAttribute("value", "some_node_name");
	flystickWalkNavTranslationResetTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "FlySpeed");
	someXmlElement->SetAttribute("value", "1.0");
	flystickWalkNavTranslationResetTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "RotationSpeed");
	someXmlElement->SetAttribute("value", "1.0");
	flystickWalkNavTranslationResetTool->LinkEndChild(someXmlElement);
	tools->LinkEndChild(flystickWalkNavTranslationResetTool);

	//CMouseTrackballNavigationTool
	TiXmlElement *mouseTrackballNavTool = new TiXmlElement("Tool");
	mouseTrackballNavTool->SetAttribute("name", "MyMouseTrackballNavTool");
	mouseTrackballNavTool->SetAttribute("type", "CMouseTrackballNavigationTool");
	tools->LinkEndChild(mouseTrackballNavTool);

	//CMouseTrackballFlystickWalkNavigationTool
	TiXmlElement *mouseTrackballFlystickWalkNavTool = new TiXmlElement("Tool");
	mouseTrackballFlystickWalkNavTool->SetAttribute("name", "MyMouseTrackballFlystickWalkNavTool");
	mouseTrackballFlystickWalkNavTool->SetAttribute("type", "CMouseTrackballFlystickWalkNavigationTool");
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "CursorFile");
	someXmlElement->SetAttribute("value", "some/file/name.osb");
	mouseTrackballFlystickWalkNavTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "FlySpeed");
	someXmlElement->SetAttribute("value", "1.0");
	mouseTrackballFlystickWalkNavTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "RotationSpeed");
	someXmlElement->SetAttribute("value", "1.0");
	mouseTrackballFlystickWalkNavTool->LinkEndChild(someXmlElement);
	tools->LinkEndChild(mouseTrackballFlystickWalkNavTool);

	//CFlystickMixedModePickTool
	TiXmlElement *flystickMixedModePickTool = new TiXmlElement("Tool");
	flystickMixedModePickTool->SetAttribute("name", "MyFlystickMixedModePickTool");
	flystickMixedModePickTool->SetAttribute("type", "CFlystickMixedModePickTool");
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "CursorFile");
	someXmlElement->SetAttribute("value", "some/file/name.osb");
	flystickMixedModePickTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "MaxPickDistance");
	someXmlElement->SetAttribute("value", "1.0");
	flystickMixedModePickTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "PickOnlyNodesInList");
	someXmlElement->SetAttribute("value", "false");
	flystickMixedModePickTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "AddNodeNameToPickableList");
	someXmlElement->SetAttribute("value", "some_node_name");
	flystickMixedModePickTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "AddNodeNameToPickableList");
	someXmlElement->SetAttribute("value", "some_other_node_name");
	flystickMixedModePickTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "AddNodeNameToPickableList");
	someXmlElement->SetAttribute("value", "some_different_node_name");
	flystickMixedModePickTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "RemoveNodeNameFromPickableList");
	someXmlElement->SetAttribute("value", "some_node_name");
	flystickMixedModePickTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "FlySpeed");
	someXmlElement->SetAttribute("value", "1.0");
	flystickMixedModePickTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "RotationSpeed");
	someXmlElement->SetAttribute("value", "1.0");
	flystickMixedModePickTool->LinkEndChild(someXmlElement);
	someXmlElement = new TiXmlElement("ToolAttr");
	someXmlElement->SetAttribute("name", "Use2D3DMixedMode");
	someXmlElement->SetAttribute("value", "false");
	flystickMixedModePickTool->LinkEndChild(someXmlElement);
	tools->LinkEndChild(flystickMixedModePickTool);

	//ART setups
	TiXmlElement *flystick = new TiXmlElement("ARTFlyStick");
	root->LinkEndChild(flystick);

	TiXmlElement *button0 = new TiXmlElement("FlystickButton");
	button0->SetAttribute("number", "0");
	button0->SetAttribute("toolname", "MyMouseTrackballNavTool");
	flystick->LinkEndChild(button0);

	TiXmlElement *button1 = new TiXmlElement("FlystickButton");
	button1->SetAttribute("number", "1");
	button1->SetAttribute("toolname", "MyWalkCageFilterBBoxTool");
	flystick->LinkEndChild(button1);

	TiXmlElement *button2 = new TiXmlElement("FlystickButton");
	button2->SetAttribute("number", "2");
	button2->SetAttribute("toolname", "MyWalkCageFilterClipPlanesTool");
	flystick->LinkEndChild(button2);

	TiXmlElement *button3 = new TiXmlElement("FlystickButton");
	button3->SetAttribute("number", "3");
	button3->SetAttribute("toolname", "MyWalkNavTranslateTool");
	flystick->LinkEndChild(button3);

	TiXmlElement *button4 = new TiXmlElement("FlystickButton");
	button4->SetAttribute("number", "4");
	button4->SetAttribute("toolname", "MyWalkNavTranslationResetTool");
	flystick->LinkEndChild(button4);

	doc.SaveFile(filename);
}