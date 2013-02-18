// Dr. Bjoern Zehner 
// UFZ Centre for Environmental Research Leipzig-Halle 
// Permoserstrasse 15 
// 04318 Leipzig 
// Germany 

#pragma once

class CElevationGridBase
{
public:
	CElevationGridBase(void);
	~CElevationGridBase(void);

	virtual double GetZ(double x, double y){return 0.1;}
};
