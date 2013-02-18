#pragma once

#include <string>

class CART2DWindowInterface
{
public:
	CART2DWindowInterface(void);
	~CART2DWindowInterface(void);

	virtual void Move(int x, int y) = 0;
	virtual void Zoom(double factor) = 0;
	virtual void Show2DViewFor(std::string filename) = 0;
};
