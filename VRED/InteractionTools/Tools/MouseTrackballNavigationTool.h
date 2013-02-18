#pragma once
#include "MouseToolBase.h"

//Handled attributes:
//Only the inherited ones!

class CMouseTrackballNavigationTool : public virtual CMouseToolBase {
public:
	CMouseTrackballNavigationTool(void);
	~CMouseTrackballNavigationTool(void);

	virtual std::string		GetToolName(){return std::string("CMouseTrackballNavigationTool");}
	virtual std::string		GetToolDescription(){return std::string("Use the mouse to navigate (trackball)");}

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