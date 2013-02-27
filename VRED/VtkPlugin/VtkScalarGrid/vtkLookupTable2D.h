#ifndef __VTK_LOOKUP_TABLE_2D
#define __VTK_LOOKUP_TABLE_2D

#include <vtkLookupTable.h>

class vtkPolyDataMapper;

class vtkLookupTable2D : public vtkLookupTable
{
public:
	static vtkLookupTable2D *New();

	//vtkTypeRevisionMacro(vtkLookupTable2D,vtkLookupTable);
	void PrintSelf(ostream& os, vtkIndent indent);

	void MapScalarsThroughTable2(void *input, unsigned char *output,
                               int inputDataType, int numberOfValues,
                               int inputIncrement, int outputIncrement);

	void SetActiveMapper(vtkPolyDataMapper *mapper){m_pvtkPolyDataMapper = mapper;}

	void SetScalarsForSecondaryData(std::string name){m_stdstrSecondaryDataName = name;}
	void SetMinMax(double min, double max){m_dSecondaryMin = min; m_dSecondaryMax = max;}
	std::string GetScalarsForSecondaryDataName(std::string name){return m_stdstrSecondaryDataName;}
	void SetUseSecondaryData(bool use){m_bUseSecondaryDataArray = use;}
	bool GetUseSecondaryData(){return m_bUseSecondaryDataArray;}

protected:
	vtkLookupTable2D(int sze=256, int ext=256);
	~vtkLookupTable2D(void);

	vtkPolyDataMapper *m_pvtkPolyDataMapper;

private:
	vtkLookupTable2D(const vtkLookupTable2D&);
	void operator=(const vtkLookupTable2D&);

	std::string m_stdstrSecondaryDataName;
	bool m_bUseSecondaryDataArray;

	double m_dSecondaryMin;
	double m_dSecondaryMax;

	void RgbToHsv(unsigned char r, unsigned char g, unsigned char b, unsigned char *h, unsigned char *s, unsigned char *v);
	void RgbToHsv(unsigned char r, unsigned char g, unsigned char b, double *h, double *s, double *v);
	void RgbToHsv(double r, double g, double b, double *h, double *s, double *v);

	void HsvToRgb(unsigned char h, unsigned char s, unsigned char v, unsigned char *r, unsigned char *g, unsigned char *b);
	void HsvToRgb(double h, double s, double v, unsigned char *r, unsigned char *g, unsigned char *b);
	void HsvToRgb(double h, double s, double v, double *r, double *g, double *b);

public:
	static double m_dUndefinedHue;
};

#endif
