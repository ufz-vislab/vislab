#include "vtkLookupTable2D.h"

#include <vtkObjectFactory.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>

double vtkLookupTable2D::m_dUndefinedHue = -1.0;

vtkStandardNewMacro(vtkLookupTable2D);

vtkLookupTable2D::vtkLookupTable2D(int sze, int ext): vtkLookupTable(sze, ext){
	m_pvtkPolyDataMapper = NULL;
	m_bUseSecondaryDataArray = false;
	m_dSecondaryMin = 0.0;
	m_dSecondaryMax = 0.0;

	this->SetHueRange(0.667, 0.0);
}

vtkLookupTable2D::~vtkLookupTable2D(void){
}

void vtkLookupTable2D::PrintSelf(ostream& os, vtkIndent indent){
	vtkLookupTable::PrintSelf(os, indent);
}

void vtkLookupTable2D::MapScalarsThroughTable2(void *input, unsigned char *output, 
			int inputDataType, int numberOfValues,
			int inputIncrement, int outputIncrement)
{
	vtkDataArray	*secondaryDataArray = NULL;
	bool			bIfUseSecondaryData = false;

	if (m_pvtkPolyDataMapper != NULL){
		std::cout << "vtkLookupTable2D::MapScalarsThroughTable2() called:" << std::endl;
		std::cout << "number of values: " << numberOfValues << std::endl;
		std::cout << "input increment: " << inputIncrement << std::endl;
		std::cout << "outputIncrement: " << outputIncrement << std::endl;

		vtkPolyData *polys = m_pvtkPolyDataMapper->GetInput();
		if (polys != NULL){
			vtkPointData *pdata = polys->GetPointData();
			if (pdata != NULL){
				secondaryDataArray = pdata->GetArray(m_stdstrSecondaryDataName.c_str());
				if ((secondaryDataArray != NULL) && (m_bUseSecondaryDataArray)){
					bIfUseSecondaryData = true;

					if (numberOfValues != secondaryDataArray->GetNumberOfTuples()){
						std::cout << "MapScalarsThroughTable2() secondary data array has the wrong number of tuples" << std::endl;
						bIfUseSecondaryData = false;
					}

					std::cout << "Using secondary data for color saturation" << std::endl;
					std::cout << "Secondary data range: " << m_dSecondaryMin << "  " << m_dSecondaryMax << std::endl;
				}
			}
		}else{
			std::cout << "Polys where NULL" << std::endl;
		}
	} else {
		std::cout << "No color mapping possible, polydata mapper is NULL" << std::endl;
	}

	vtkLookupTable::MapScalarsThroughTable2(input, output, inputDataType, numberOfValues, inputIncrement, outputIncrement);

	int numNaNs = 0;

	double dSecondaryRange = m_dSecondaryMax - m_dSecondaryMin;

	if ((outputIncrement == 4) && m_bUseSecondaryDataArray && bIfUseSecondaryData){
		for (int i=0; i<numberOfValues; i++){
			unsigned char r, g, b, a;
			unsigned char rNew, gNew, bNew, aNew;
			double h, s, v;

			r = output[i * 4 + 0];
			g = output[i * 4 + 1];
			b = output[i * 4 + 2];
			a = output[i * 4 + 3];

			this->RgbToHsv(r, g, b, &h, &s, &v);

			double dataval = secondaryDataArray->GetTuple1(i);
			double fraction = (dataval - m_dSecondaryMin) / dSecondaryRange;

			char dataValAsChar[20];
			sprintf(dataValAsChar, "%lf", fraction);
			if (std::string(dataValAsChar).find("-1.#IND") != std::string::npos){
				fraction = -1.0;
				numNaNs++;
			}

			if (fraction >= 0.0){
				s *= (1.0 - fraction);
				v = 1.0 - fraction;
			} else {
				v = 0.0;
			}

			HsvToRgb(h, s, v, &rNew, &gNew, &bNew);
			aNew = a;

			if (! (i % 100)){
				std::cout << i << "    rgbaold: " << (int) r << " " << (int) g << " " << (int) b << " " << (int) a;
				std::cout << "     rgbanew: " << (int) rNew << " " << (int) gNew << " " << (int) bNew << "    fraction: "  << fraction << std::endl;
			}

			output[i * 4 + 0] = rNew;
			output[i * 4 + 1] = gNew;
			output[i * 4 + 2] = bNew;
			output[i * 4 + 3] = aNew;
		}
	} else {
		std::cout << "output data are not of rgba type" << std::endl;
	}

	std::cout << "Number of NaNs: " << numNaNs << std::endl;
	std::cout << "Number of values: " << numberOfValues << std::endl;
}

void vtkLookupTable2D::RgbToHsv(unsigned char r, unsigned char g, unsigned char b, unsigned char *h, unsigned char *s, unsigned char *v){
	double dr, dg, db, dh, ds, dv;
	int ir, ig, ib, ih, is, iv;

	dr = ((double) ((int) r)) / 255.0;
	dg = ((double) ((int) g)) / 255.0;
	db = ((double) ((int) b)) / 255.0;

	RgbToHsv(dr, dg, db, &dh, &ds, &dv);

	ih = (int) (dh * 255.0);
	is = (int) (ds * 255.0);
	iv = (int) (dv * 255.0);

	*h = (unsigned char) ih;
	*s = (unsigned char) is;
	*v = (unsigned char) iv;
}

void vtkLookupTable2D::RgbToHsv(unsigned char r, unsigned char g, unsigned char b, double *h, double *s, double *v){
	double dr, dg, db;
	dr = ((double) ((int) r)) / 255.0;
	dg = ((double) ((int) g)) / 255.0;
	db = ((double) ((int) b)) / 255.0;

	RgbToHsv(dr, dg, db, h, s, v);
}

void vtkLookupTable2D::RgbToHsv(double r, double g, double b, double *h, double *s, double *v){
	double max = ((r >= g) && (r >= b)) ? r : ((g >= b) ? g : b);
	double min = ((r <= g) && (r <= b)) ? r : ((g <= b) ? g : b);
	
	*v = max;
	*s = (max != 0.0) ? ((max - min) / max) : 0.0;
	if (*s == 0.0){
		*h = m_dUndefinedHue;
	} else {
		double delta = max - min;
		if (r == max){
			*h = (g-b)/delta;
		} else if (g == max){
			*h = 2.0 + (b - r) / delta;
		} else if (b == max){
			*h = 4.0 + (r - g) / delta;
		} else {
			std::cout << "Error during conversion from rgb to hsv color space" << std::endl;
		}
		*h *= 60.0;
		if (*h < 0.0) *h += 360.0;
	}
}

void vtkLookupTable2D::HsvToRgb(unsigned char h, unsigned char s, unsigned char v, unsigned char *r, unsigned char *g, unsigned char *b){
	double dh, ds, dv, dr, dg, db;
	int ih, is, iv, ir, ig, ib;

	dh = ((double) ((int) h)) / 255.0;
	ds = ((double) ((int) s)) / 255.0;
	dv = ((double) ((int) v)) / 255.0;

	HsvToRgb(dh, ds, dv, &dr, &dg, &db);

	*r = (unsigned char)((int)(dr * 255.0));
	*g = (unsigned char)((int)(dg * 255.0));
	*b = (unsigned char)((int)(db * 255.0));
}

void vtkLookupTable2D::HsvToRgb(double h, double s, double v, unsigned char *r, unsigned char *g, unsigned char *b){
	double dr, dg, db;

	HsvToRgb(h, s, v, &dr, &dg, &db);

	*r = (unsigned char)((int)(dr * 255.0));
	*g = (unsigned char)((int)(dg * 255.0));
	*b = (unsigned char)((int)(db * 255.0));
}

void vtkLookupTable2D::HsvToRgb(double h, double s, double v, double *r, double *g, double *b){
	if (s == 0.0){
		if (h == m_dUndefinedHue){
			*r = v;
			*g = v;
			*b = v;
		} else {
			std::cout << "HsvToRgb: dodgy conversion, s == 0.0 but h has a value!" << std::endl;
		}
	} else {
		double f, p, q, t;
		int i;

		if (h == 360.0) h = 0.0;
		h /= 60.0;
		i = (int) floor(h);
		f = h - (double) i;
		p = v * (1.0 - s);
		q = v * (1.0 - (s * f));
		t = v * (1.0 - (s * (1.0 - f)));
		switch(i){
			case 0: *r = v; *g = t; *b = p; break;
			case 1: *r = q; *g = v; *b = p; break;
			case 2: *r = p; *g = v; *b = t; break;
			case 3: *r = p; *g = q; *b = v; break;
			case 4: *r = t; *g = p; *b = v; break;
			case 5: *r = v; *g = p; *b = q; break;
		}
	}
}
