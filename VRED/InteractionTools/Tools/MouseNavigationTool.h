#pragma once
#include "MouseToolBase.h"

//Handled attributes:
//Only the inherited ones

class CMouseNavigationTool : public CMouseToolBase
{
public:
	CMouseNavigationTool(void);
	~CMouseNavigationTool(void);

	virtual std::string		GetToolName(){return std::string("MouseNavigationTool");}
	virtual std::string		GetToolDescription(){return std::string("Use the mouse to navigate through the scene (trackball)");}

	//Inherited Interface for setting variables as string
	virtual bool			SetAttr(std::string attr, std::string value);

	virtual void			OnLeftMouseButtonDown(int x, int y);
	virtual void			OnMiddleMouseButtonDown(int x, int y);
	virtual void			OnRightMouseButtonDown(int x, int y);
	virtual void			OnLeftMouseButtonUp(int x, int y);
	virtual void			OnMiddleMouseButtonUp(int x, int y);
	virtual void			OnRightMouseButtonUp(int x, int y);
	virtual void			OnMouseMove(int x, int y);
};
