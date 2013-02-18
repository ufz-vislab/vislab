#pragma once
#include "toolbase.h"

class CMouseToolBase : public virtual CToolBase
{
public:
	CMouseToolBase(void);
	~CMouseToolBase(void);

	virtual void			OnLeftMouseButtonDown(int x, int y);
	virtual void			OnMiddleMouseButtonDown(int x, int y);
	virtual void			OnRightMouseButtonDown(int x, int y);
	virtual void			OnLeftMouseButtonUp(int x, int y);
	virtual void			OnMiddleMouseButtonUp(int x, int y);
	virtual void			OnRightMouseButtonUp(int x, int y);
	virtual void			OnMouseMove(int x, int y);
};
