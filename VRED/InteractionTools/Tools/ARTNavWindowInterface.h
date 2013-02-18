#pragma once

class CARTNavWindowInterface
{
public:
	CARTNavWindowInterface(void);
	~CARTNavWindowInterface(void);

	virtual void SetFromAtWithZUp(float fromX, float fromY, float atX, float atY, float height) = 0;
};