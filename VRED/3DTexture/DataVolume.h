#pragma once

//#define _UNICODE
#include <fstream>
#include <math.h>
#include <assert.h>
#include <iostream>

#include <TCHAR.h>

using namespace std;

typedef unsigned char GLubyte;
typedef unsigned int GLuint;

class CDataVolume
{
private:
	int _xDim;
	int _yDim;
	int _zDim;
	int _slicesize;
	int _components;
	int _border;

	GLubyte *_texels;
	GLuint	_texName;

	int GetIndex(int xInd, int yInd, int zInd);

public:
	CDataVolume(void);
	~CDataVolume(void);

	unsigned char *GetTexelPointer(){return (unsigned char*) _texels;};
	int GetXDim(){return _xDim;};
	int GetYDim(){return _yDim;};
	int GetZDim(){return _zDim;};

	void SetVolume(int xdim, int ydim, int zdim, int components, unsigned char *texels){
		_xDim = xdim; _yDim = ydim; _zDim = zdim; _components = components;
		_slicesize = _xDim * _yDim;
		_texels = (GLubyte*) texels;
	};

	void CreateVolume(int xdim, int ydim, int zdim, int components){
		_xDim = xdim; _yDim = ydim; _zDim = zdim; _components = components;
		_slicesize = _xDim * _yDim;
		_texels = new GLubyte[_xDim * _yDim * _zDim * _components];
	};
	
	void	ReadTexture(const char *filename);
	void	WriteTexture(const char *filename);
	void	ReadTextureBin(const char *filename);
	void	ReadTextureBin(const wchar_t *filename);
	void	ReadTextureBin(FILE *fpointer);

	void	WriteTextureBin(const char *filename);
	void	ReadTextureBinIRFormat(const char *filename);
	void	ReadSubTextureIRFormat(const char *filename, int xdim, int ydim, int zdim, int xoffset, int yoffset, int zoffset);
	void	ReadSubTextureBinFormat(const char *filename, int xdim, int ydim, int zdim, int xoffset, int yoffset, int zoffset);
	void	AdjustToDataRange1Comp(int lowClamp, int highClamp);
	void	GetStatistics(int comp, int *frequencies);
	void	WriteStatistics(int comp, const char *filename);
	void	EqualizeHistogram(int comp);
	void	SetTextureToPowersOfTwo1Comp();
	void	SwapBytesInt(void *theint);
	int		GetNewDimPowersOfTwo(int dim);
	int		GetNumComponents();

	void	SetToGradient(int comp, float texScaleX, float texScaleY, float texScaleZ);
	void	SetToGradientSobel(int comp, float texScaleX, float texScaleY, float texScaleZ);
	void	SetToGradientSobelComp2Alpha(int comp, float texScaleX, float texScaleY, float texScaleZ);
	void	SetToBlurredGradientSobelComp2Alpha(int comp, float texScaleX, float texScaleY, float texScaleZ);
	
	// returns a pointer to the allocated convolution result
	float	*Convolve3x3x3(int comp, float *kernel); // kernel length musst be 27 !
	float	*Convolve3x3x3(float *inputImage, int numComponents, int comp, float *kernel);
	void	CopyBorderTexels(float *input, float *output);
	void	CopyBorderTexelsFromInside(float *data);

	//void	GlInitTexture();
	//GLuint	GetTextureName();

	void SetTextureCheckerBoard(int xdim, int ydim, int zdim);
	void SetTextureCube(int xdim, int ydim, int zdim);
	void SetTextureSphere(int xdim, int ydim, int zdim);
};
