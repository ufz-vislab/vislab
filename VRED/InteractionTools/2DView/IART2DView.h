#pragma once

#include <string>

class IART2DView
{
public:
	IART2DView(void);
	~IART2DView(void);

	virtual void ZoomIn() = 0;
	virtual void ZoomOut() = 0;
	virtual void Zoom(double factor) = 0;
	virtual void Scroll(int x, int y) = 0;
	virtual void ScrollUp() = 0;
	virtual void ScrollDown() = 0;
	virtual void ScrollLeft() = 0;
	virtual void ScrollRight() = 0;
	virtual void ShowInfoFor(std::string name) = 0;
};
