#include "datavolume.h"

CDataVolume::CDataVolume(void)
{
	_xDim = 0;
	_yDim = 0;
	_zDim = 0;
	_slicesize = 0;
	_components = 0;
	_border = 0;

	_texels = NULL;

	_texName = 0;
}

CDataVolume::~CDataVolume(void)
{
	if (_texels != NULL){
		delete [] _texels;
	}
}

int CDataVolume::GetIndex(int xInd, int yInd, int zInd){
	return (zInd * _slicesize + yInd * _xDim + xInd);
}

void CDataVolume::ReadTexture(const char *filename){
	//std::ifstream in(filename, std::ios_base::binary);
	std::ifstream in(filename);
	in >> _xDim;
	in >> _yDim;
	in >> _zDim;
	in >> _components;
	_slicesize = _xDim * _yDim;

	int numVals = _xDim * _yDim * _zDim * _components;
	if (_texels != NULL){
		delete [] _texels;
	}
	_texels = new GLubyte[numVals];
	int number;

	for (int i=0; i<numVals; i++){
		in >> number;
		_texels[i] = (GLubyte) number;
	}

	in.close();
}

void CDataVolume::WriteTexture(const char *filename){
	//this->SetTextureCheckerBoard(128, 64, 64);
	//std::ofstream out(filename, std::ios_base::binary);
	std::ofstream out(filename);
	out << _xDim << "\n";
	out << _yDim << "\n";
	out << _zDim << "\n";
	out << _components << "\n";

	int numVals = _xDim * _yDim * _zDim * _components;
	int number;
	for (int i=0; i<numVals; i++){
		number = (int) _texels[i];
		out << number << " ";
	}
	out.close();
}

void CDataVolume::ReadTextureBin(const char *filename){
	FILE *fpointer;
	fpointer = fopen(filename, "rb");
	ReadTextureBin(fpointer);
}

void CDataVolume::ReadTextureBin(const wchar_t *filename){
	FILE *fpointer;
	fpointer = _wfopen(filename, L"rb");
	ReadTextureBin(fpointer);
}

void CDataVolume::ReadTextureBin(FILE *fpointer){
	bool ERROR_WHILE_READING_TEXTURE = false;
	if (fpointer == NULL) assert(ERROR_WHILE_READING_TEXTURE);
	fread(&_xDim, sizeof(int), 1, fpointer);
	fread(&_yDim, sizeof(int), 1, fpointer);
	fread(&_zDim, sizeof(int), 1, fpointer);
	fread(&_components, sizeof(int), 1, fpointer);
	
	_slicesize = _xDim * _yDim;
	int numVals = _xDim * _yDim * _zDim * _components;
	if (_texels != NULL){
		delete [] _texels;
	}
	_texels = new GLubyte[numVals];

	if (numVals != fread(_texels, sizeof(GLubyte), numVals, fpointer)){
		assert(ERROR_WHILE_READING_TEXTURE);
	}

	fclose(fpointer);

/*
	std::ofstream debug("readIRFormatDebug.txt");
	debug << "sizeof int: " << sizeof(int) << "\n";
	debug << "xDim: " << _xDim << "\n";
	debug << "yDim: " << _yDim << "\n";
	debug << "zDim: " << _zDim << "\n";
	numVals = _xDim * _yDim * _zDim;
	for (int i=0; i<numVals; i++){
		debug.width(8);
		debug << (int) _texels[i * 4 + 0];
		debug.width(8);
		debug << (int) _texels[i * 4 + 1];
		debug.width(8);
		debug << (int) _texels[i * 4 + 2];
		debug.width(8);
		debug << (int) _texels[i * 4 + 3];
		debug << "\n";
	}
	debug.close();
	*/
}

void CDataVolume::ReadTextureBinIRFormat(const char *filename){
	bool ERROR_WHILE_READING_IR_TEXTURE = false;
	FILE *fpointer;
	int numberOfBitsPerVoxel;
	fpointer = fopen(filename, "rb");
	fseek(fpointer, 332L, SEEK_SET);
	fread(&numberOfBitsPerVoxel, sizeof(int), 1, fpointer);
	fread(&_xDim, sizeof(int), 1, fpointer);
	fread(&_yDim, sizeof(int), 1, fpointer);
	fread(&_zDim, sizeof(int), 1, fpointer);

	SwapBytesInt(&numberOfBitsPerVoxel);
	SwapBytesInt(&_xDim);
	SwapBytesInt(&_yDim);
	SwapBytesInt(&_zDim);

	_components = 1;
	_slicesize = _xDim * _yDim;

	int numvals = _xDim * _yDim * _zDim;

	if (_texels != NULL){
		delete [] _texels;
	}
	_texels = new GLubyte[numvals];

	fseek(fpointer, 3072L, SEEK_SET);
	if (numvals != fread(_texels, sizeof(GLubyte), numvals, fpointer)){
		assert(ERROR_WHILE_READING_IR_TEXTURE);
	}

	fclose(fpointer);
	
	std::ofstream debug("readIRFormatDebug.txt");
	debug << "sizeof int: " << sizeof(int) << "\n";
	debug << "bits per pixel: " << numberOfBitsPerVoxel << "\n";
	debug << "xDim: " << _xDim << "\n";
	debug << "yDim: " << _yDim << "\n";
	debug << "zDim: " << _zDim << "\n";

	debug << "Correction texturesize:\n";

	AdjustToDataRange1Comp(5, 250);

	SetTextureToPowersOfTwo1Comp();

	debug << "xDim: " << _xDim << "\n";
	debug << "yDim: " << _yDim << "\n";
	debug << "zDim: " << _zDim << "\n";

	debug.close();
}

void CDataVolume::ReadSubTextureIRFormat(const char *filename, int xdim, int ydim, int zdim, int xoffset, int yoffset, int zoffset){
	bool ERROR_WHILE_READING_IR_SUB_TEXTURE = false;
	FILE *fpointer;
	int numberOfBitsPerVoxel;
	int xDimFull, yDimFull, zDimFull;
	int sliceSizeFull;
	long headerLength = 3072L;
	int i, j, k;

	fpointer = fopen(filename, "rb");
	fseek(fpointer, 332L, SEEK_SET);
	fread(&numberOfBitsPerVoxel, sizeof(int), 1, fpointer);
	fread(&xDimFull, sizeof(int), 1, fpointer);
	fread(&yDimFull, sizeof(int), 1, fpointer);
	fread(&zDimFull, sizeof(int), 1, fpointer);
	SwapBytesInt(&numberOfBitsPerVoxel);
	SwapBytesInt(&xDimFull);
	SwapBytesInt(&yDimFull);
	SwapBytesInt(&zDimFull);

	sliceSizeFull = xDimFull * yDimFull;

	assert((xDimFull >= (xdim + xoffset)) && (yDimFull >= (ydim + yoffset)) && (zDimFull >= (zdim + zoffset)));

	if (_texels != NULL) delete [] _texels;
	_xDim = xdim;
	_yDim = ydim;
	_zDim = zdim;
	_components = 1;
	_slicesize = _xDim * _yDim;
	int numvals = _xDim * _yDim * _zDim;

	_texels = new GLubyte[numvals];
	
	GLubyte *byteline = new GLubyte[_xDim];

	for (k=0; k<_zDim; k++){
		for (j=0; j<_yDim; j++){
			long ijkFull = headerLength + (long)((zoffset + k) * sliceSizeFull + (yoffset + j) * xDimFull + xoffset);
			int ijkSub = k * _slicesize + j * _xDim;

			fseek(fpointer, ijkFull, SEEK_SET);
			fread(byteline, sizeof(GLubyte), _xDim, fpointer);

			for (i=0; i<_xDim; i++){
				_texels[ijkSub + i] = byteline[i];
			}
		}
	}

	fclose(fpointer);
}


void CDataVolume::ReadSubTextureBinFormat(const char *filename, int xdim, int ydim, int zdim, int xoffset, int yoffset, int zoffset){
	bool ERROR_WHILE_READING_BIN_SUB_TEXTURE = false;
	FILE *fpointer;
	int numberOfBitsPerVoxel;
	int xDimFull, yDimFull, zDimFull, components;
	int sliceSizeFull;
	long headerLength = (long int) (4 * sizeof(int));
	int i, j, k;

	fpointer = fopen(filename, "rb");
	fread(&xDimFull, sizeof(int), 1, fpointer);
	fread(&yDimFull, sizeof(int), 1, fpointer);
	fread(&zDimFull, sizeof(int), 1, fpointer);
	fread(&components, sizeof(int), 1, fpointer);

	sliceSizeFull = xDimFull * yDimFull * components;

	assert((xDimFull >= (xdim + xoffset)) && (yDimFull >= (ydim + yoffset)) && (zDimFull >= (zdim + zoffset)));

	if (_texels != NULL) delete [] _texels;
	_xDim = xdim;
	_yDim = ydim;
	_zDim = zdim;
	_components = components;
	_slicesize = _xDim * _yDim * _components;
	int numvals = _xDim * _yDim * _zDim * _components;

	_texels = new GLubyte[numvals];
	
	GLubyte *byteline = new GLubyte[_xDim * components];

	for (k=0; k<_zDim; k++){
		for (j=0; j<_yDim; j++){
			long ijkFull = headerLength + (long)((zoffset + k) * sliceSizeFull + (yoffset + j) * xDimFull * _components + xoffset * _components);
			int ijkSub = k * _slicesize + j * _xDim * _components;

			fseek(fpointer, ijkFull, SEEK_SET);
			fread(byteline, sizeof(GLubyte), _xDim, fpointer);

			int xlength = _xDim * _components;
			for (i=0; i<xlength; i++){
				_texels[ijkSub + i] = byteline[i];
			}
		}
	}

	fclose(fpointer);
}

void CDataVolume::AdjustToDataRange1Comp(int lowClamp, int highClamp){
	int i;
	GLubyte minVal, maxVal;
	int numvals = _xDim * _yDim * _zDim;
	minVal = maxVal = _texels[0];
	for (i=1; i<numvals; i++){
		if (_texels[i] < minVal) minVal = _texels[i];
		if (_texels[i] > maxVal) maxVal = _texels[i];
	}

	float dataRange = (float) (maxVal - minVal);
	float tableRange = (float) (highClamp - lowClamp);
	float factor = tableRange / dataRange;
	float fLowClamp = (float) lowClamp;
	float fHighClamp = (float) highClamp;

	for (i=0; i<numvals; i++){
		int val = (int)(fLowClamp + factor * (float) _texels[i]);
		_texels[i] = (GLubyte) val;
	}
}


void CDataVolume::GetStatistics(int comp, int *frequencies){
	int i, numvals;
	numvals = _xDim * _yDim * _zDim;
	for (i=0; i<256; i++) frequencies[i] = 0;
	for (i=0; i<numvals; i++){
		int index = (int) _texels[i * _components + comp];
		frequencies[index] += 1;
	}
}

void CDataVolume::WriteStatistics(int comp, const char *filename){
	int frequ[256];
	GetStatistics(comp, frequ);

	std::ofstream out(filename);;
	
	out << "xDim: " << _xDim << "\n";
	out << "yDim: " << _yDim << "\n";
	out << "zDim: " << _zDim << "\n";
	out << "Comp: " << _components << "\n\n";

	int cumulativeSum = 0;
	float numval = (float) (_xDim * _yDim * _zDim);

	for (int i=0; i<256; i++){
		cumulativeSum += frequ[i];

		out.width(8);
		out << i;

		out.width(8);
		out << frequ[i];

		out << "	";
		out.width(10);
		out << ((float)cumulativeSum)/((float)numval);

		out << "	";
		out << ((float) i) / 256.0f;

		out << "\n";
	}
	out.close();
}

void CDataVolume::EqualizeHistogram(int comp){
	// Equalizes the histogram as described in Gonzales & Wintz
	// it is assumed that invalid data have the value 128 !!!!!
	int i;
	int curFrequencies[256];
	int transferFunction[256];
	float cumHistogram[256];   // Cummulative Histogram

	this->GetStatistics(comp, curFrequencies);

	int invalidValues = curFrequencies[128];
	curFrequencies[128] = curFrequencies[127];

	int numvals = _xDim * _yDim * _zDim - invalidValues + curFrequencies[127];

	cumHistogram[0] = ((float) curFrequencies[0]) / ((float) numvals);
	for (i=1; i<256; i++){
		cumHistogram[i] = cumHistogram[i-1] + ((float) curFrequencies[i]) / ((float) numvals);
	}
	for (i=0; i<256; i++){
		transferFunction[i] = (int)(255.0f * cumHistogram[i]);
		//transferFunction[i] = i;
	}
	transferFunction[128] = 128;

	std::ofstream out("histogramFunction.txt");
	for (i=0; i<256; i++){
		out << "cum: " << cumHistogram[i] << "			" << i << " -> " << transferFunction[i] << "\n";
	}
	for (i=0; i<numvals; i++){
		int texelInd = i * _components + comp;
		int val = (int) _texels[texelInd];
		_texels[texelInd] = (GLubyte) transferFunction[val];
	}

	int newFrequencies[256];
	this->GetStatistics(comp, newFrequencies);

	out << "\n\n\n";
	curFrequencies[128] = invalidValues;
	for (i=0; i<256; i++){
		out.width(10);
		out << curFrequencies[i] << "	";
		out.width(10);
		out << newFrequencies[i] << "\n";
	}
	out.close();
}

void CDataVolume::SetTextureToPowersOfTwo1Comp(){
	int i, j, k;
	int newXDim = GetNewDimPowersOfTwo(_xDim);
	int newYDim = GetNewDimPowersOfTwo(_yDim);
	int newZDim = GetNewDimPowersOfTwo(_zDim);

	if ((newXDim != _xDim) || (newYDim != _yDim) || (newZDim != _zDim)){
		int newNumVals = newXDim * newYDim * newZDim;
		int newSliceSize = newXDim * newYDim;
		GLubyte *newTexels = new GLubyte[newNumVals];
		for (k=0; k<newZDim; k++){
			for (j=0; j<newYDim; j++){
				for (i=0; i<newXDim; i++){
					int newInd = k * newSliceSize + j * newXDim + i;
					if ((i<_xDim)&&(j<_yDim)&&(k<_zDim)){
						newTexels[newInd] = _texels[k * _slicesize + j * _xDim + i];
					} else {
						newTexels[newInd] = (GLubyte) 253;
					}
				}
			}
		}

		delete [] _texels;
		_texels = newTexels;
		_xDim = newXDim;
		_yDim = newYDim;
		_zDim = newZDim;
		_components = 1;
		_slicesize = newSliceSize;
	}
}



void CDataVolume::SwapBytesInt(void *theint){
	GLubyte *inbytes = (GLubyte*) theint;
	GLubyte outbytes[4];

	outbytes[0] = inbytes[3];
	outbytes[1] = inbytes[2];
	outbytes[2] = inbytes[1];
	outbytes[3] = inbytes[0];

	inbytes[0] = outbytes[0];
	inbytes[1] = outbytes[1];
	inbytes[2] = outbytes[2];
	inbytes[3] = outbytes[3];
}

int CDataVolume::GetNewDimPowersOfTwo(int dim){
	int newdim = 2;
	for (int i=0; i<12; i++){
		newdim *= 2;
		if (dim <= newdim) return newdim;
	}
	return 0;
}

int CDataVolume::GetNumComponents(){
	return _components;
}


void CDataVolume::WriteTextureBin(const char *filename){
	bool ERROR_WHILE_WRITING_TEXTURE = false;
	FILE *fpointer;
	
	fpointer = fopen(filename, "wb");
	fwrite(&_xDim, sizeof(int), 1, fpointer);
	fwrite(&_yDim, sizeof(int), 1, fpointer);
	fwrite(&_zDim, sizeof(int), 1, fpointer);
	fwrite(&_components, sizeof(int), 1, fpointer);

	int numVals = _xDim * _yDim * _zDim * _components;

	if (numVals != fwrite(_texels, sizeof(GLubyte), numVals, fpointer)){
		assert(ERROR_WHILE_WRITING_TEXTURE);
	}

	fclose(fpointer);
}

void CDataVolume::SetToGradient(int comp, float texScaleX, float texScaleY, float texScaleZ){
	int sliceSizeWComp, yLineSizeWComp;
	int xInd, yInd, zInd, i;

	float dx = texScaleX / (float) _xDim;
	float dy = texScaleY / (float) _yDim;
	float dz = texScaleZ / (float) _zDim;

	float dxyzMax = (dx > dy) ? dx : dy;
	dxyzMax = (dz > dxyzMax) ? dz : dxyzMax;

	dx /= dxyzMax;
	dy /= dxyzMax;
	dz /= dxyzMax;

	float dx2 = dx * 2.0f;
	float dy2 = dy * 2.0f;
	float dz2 = dz * 2.0f;

	float gradX, gradY, gradZ, gradABS;

	sliceSizeWComp = _slicesize * _components;
	yLineSizeWComp = _xDim * _components;

	float *newTexels = new float[_xDim * _yDim * _zDim * 4];
	int numTexels = _xDim * _yDim * _zDim;
	int numVals = _xDim * _yDim * _zDim * 4;

	float X0, X2, Y0, Y2, Z0, Z2;
	for (zInd = 0; zInd < _zDim; zInd++){
		for (yInd = 0; yInd < _yDim; yInd++){
			for (xInd = 0; xInd < _xDim; xInd++){
				int ijk = GetIndex(xInd, yInd, zInd);
				int ijkcomp = ijk * _components;

				if (xInd == 0){
					X0 = (float) _texels[ijkcomp + comp];
					X2 = (float) _texels[ijkcomp + _components + comp];
					gradX = (X2 - X0) / dx;
					
				} else if (xInd == (_xDim -1)){
					X0 = (float) _texels[ijkcomp - _components + comp];
					X2 = (float) _texels[ijkcomp + comp];
					gradX = (X2 - X0) / dx;
				} else {
					X0 = (float) _texels[ijkcomp - _components + comp];
					X2 = (float) _texels[ijkcomp + _components + comp];
					gradX = (X2 - X0) / dx2;
				}

				if (yInd == 0){
					Y0 = (float) _texels[ijkcomp + comp];
					Y2 = (float) _texels[ijkcomp + yLineSizeWComp + comp];
					gradY = (Y2 - Y0) / dy;
				} else if (yInd == (_yDim -1)){
					Y0 = (float) _texels[ijkcomp - yLineSizeWComp + comp];
					Y2 = (float) _texels[ijkcomp + comp];
					gradY = (Y2 - Y0) / dy;
				} else {
					Y0 = (float) _texels[ijkcomp - yLineSizeWComp + comp];
					Y2 = (float) _texels[ijkcomp + yLineSizeWComp + comp];
					gradY = (Y2 - Y0) / dy2;
				}

				if (zInd == 0){
					Z0 = (float) _texels[ijkcomp + comp];
					Z2 = (float) _texels[ijkcomp + sliceSizeWComp + comp];
					gradZ = (Z2 - Z0) / dz;
				} else if (zInd == (_zDim -1)){
					Z0 = (float) _texels[ijkcomp - sliceSizeWComp + comp];
					Z2 = (float) _texels[ijkcomp + comp];
					gradZ = (Z2 - Z0) / dz;
				} else {
					Z0 = (float) _texels[ijkcomp - sliceSizeWComp + comp];
					Z2 = (float) _texels[ijkcomp + sliceSizeWComp + comp];
					gradZ = (Z2 - Z0) / dz2;
				}

				gradABS = sqrtf(gradX * gradX + gradY * gradY + gradZ * gradZ);

				if (gradABS > 0.00001){
					newTexels[ijkcomp + 0] = gradX / gradABS;
					newTexels[ijkcomp + 1] = gradY / gradABS;
					newTexels[ijkcomp + 2] = gradZ / gradABS;
				} else {
					newTexels[ijkcomp + 0] = newTexels[ijkcomp + 1] = newTexels[ijkcomp + 2] = 0.0f;
				}
				newTexels[ijkcomp + 3] = gradABS;
			}
		}
	}

	//normalizing gradABS
	float gradABSMax = newTexels[3];
	for (i=3; i<numVals; i+=4){
		if (newTexels[i] > gradABSMax) gradABSMax = newTexels[i];
	}

	for (i=3; i<numVals; i+=4){
		newTexels[i] = newTexels[i] / gradABSMax;
	}

	// Setting the origin of the normalized gradient to (0.5, 0.5, 0.5) pointing in all 8 octants
	for (i=0; i<numVals; i+=4){
		newTexels[i + 0] = 0.5f + newTexels[i + 0] / 2.0f;
		newTexels[i + 1] = 0.5f + newTexels[i + 1] / 2.0f;
		newTexels[i + 2] = 0.5f + newTexels[i + 2] / 2.0f;
	}

	delete [] _texels;
	_texels = new GLubyte[_xDim * _yDim * _zDim * 4];
	_components = 4;

	for (i=0; i<numVals; i++){
		_texels[i] = (GLubyte)((int)(newTexels[i] * 255.0f));
	}
}

void CDataVolume::SetToGradientSobel(int comp, float texScaleX, float texScaleY, float texScaleZ){
	int i;

	float sobelX[27] = {-2.0f, 0.0f, 2.0f, -3.0f, 0.0f, 3.0f, -2.0f, 0.0f, 2.0f,
		-3.0f, 0.0f, 3.0f, -6.0f, 0.0f, 6.0f, -3.0f, 0.0f, 3.0f,
		-2.0f, 0.0f, 2.0f, -3.0f, 0.0f, 3.0f, -2.0f, 0.0f, 2.0f};

	float sobelY[27] = {-2.0f, -3.0f, -2.0f, 0.0f, 0.0f, 0.0f, 2.0f, 3.0f, 2.0f,
		-3.0f, -6.0f, -3.0f, 0.0f, 0.0f, 0.0f, 3.0f, 6.0f, 3.0f,
		-2.0f, -3.0f, -2.0f, 0.0f, 0.0f, 0.0f, 2.0f, 3.0f, 2.0f};

	float sobelZ[27] = {-2.0f, -3.0f, -2.0f, -3.0f, -6.0f, -3.0f, -2.0f, -3.0f, -2.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		2.0f, 3.0f, 2.0f, 3.0f, 6.0f, 3.0f, 2.0f, 3.0f, 2.0f};

	float nullop[27] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

	float differX[27] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

	float differY[27] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

	float differZ[27] = {0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f};

	float blur[27] = {0.1f, 0.1f, 0.1f, 0.1f, 0.3f, 0.1f, 0.1f, 0.1f, 0.1f,
		0.1f, 0.3f, 0.1f, 0.3f, 1.0f, 0.3f, 0.1f, 0.3f, 0.1f,
		0.1f, 0.1f, 0.1f, 0.1f, 0.3f, 0.1f, 0.1f, 0.1f, 0.1f};


	int numvals = _xDim * _yDim * _zDim;

	float *gradX, *gradY, *gradZ;
	float *gradABS = new float[numvals];

	gradX = Convolve3x3x3(comp, sobelX);
	gradY = Convolve3x3x3(comp, sobelY);
	gradZ = Convolve3x3x3(comp, sobelZ);

	for (i=0; i<numvals; i++){
		float gradientABS = gradX[i] * gradX[i] + gradY[i] * gradY[i] + gradZ[i] * gradZ[i];
		gradABS[i] = sqrtf(gradientABS);
	}

	for (i=0; i<numvals; i++){
		if (gradABS[i] > 0.001){
			gradX[i] /= gradABS[i];
			gradY[i] /= gradABS[i];
			gradZ[i] /= gradABS[i];

			gradX[i] = 0.5f + 0.5f * gradX[i];
			gradY[i] = 0.5f + 0.5f * gradY[i];
			gradZ[i] = 0.5f + 0.5f * gradZ[i];
		}else{
			gradX[i] = 0.0f;
			gradY[i] = 0.0f;
			gradZ[i] = 0.0f;
		}
	}

	float absMax = gradABS[0];
	for (i=0; i<numvals; i++){
		if (gradABS[i] > absMax){
			absMax = gradABS[i];
		}
	}
	for (i=0; i<numvals; i++){
		gradABS[i] /= absMax;
	}

	for (i=0; i<numvals; i++){
		_texels[i * 4 + 0] = (GLubyte)((int) (gradX[i] * 255.0f));
		_texels[i * 4 + 1] = (GLubyte)((int) (gradY[i] * 255.0f));
		_texels[i * 4 + 2] = (GLubyte)((int) (gradZ[i] * 255.0f));
		_texels[i * 4 + 3] = (GLubyte)((int) (gradABS[i] * 255.0f));
	}
}

void CDataVolume::SetToGradientSobelComp2Alpha(int comp, float texScaleX, float texScaleY, float texScaleZ){
	int i, j, k;

	float sobelX[27] = {-2.0f, 0.0f, 2.0f, -3.0f, 0.0f, 3.0f, -2.0f, 0.0f, 2.0f,
		-3.0f, 0.0f, 3.0f, -6.0f, 0.0f, 6.0f, -3.0f, 0.0f, 3.0f,
		-2.0f, 0.0f, 2.0f, -3.0f, 0.0f, 3.0f, -2.0f, 0.0f, 2.0f};

	float sobelY[27] = {-2.0f, -3.0f, -2.0f, 0.0f, 0.0f, 0.0f, 2.0f, 3.0f, 2.0f,
		-3.0f, -6.0f, -3.0f, 0.0f, 0.0f, 0.0f, 3.0f, 6.0f, 3.0f,
		-2.0f, -3.0f, -2.0f, 0.0f, 0.0f, 0.0f, 2.0f, 3.0f, 2.0f};

	float sobelZ[27] = {-2.0f, -3.0f, -2.0f, -3.0f, -6.0f, -3.0f, -2.0f, -3.0f, -2.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		2.0f, 3.0f, 2.0f, 3.0f, 6.0f, 3.0f, 2.0f, 3.0f, 2.0f};

	int numvals = _xDim * _yDim * _zDim;

	float *gradX, *gradY, *gradZ;
	float *gradABS = new float[numvals];

	cout << "Computing gradient in x direction .." << endl;
	gradX = Convolve3x3x3(comp, sobelX);

	cout << "Computing gradient in y direction .." << endl;
	gradY = Convolve3x3x3(comp, sobelY);

	cout << "Computing gradient in z direction .." << endl,
	gradZ = Convolve3x3x3(comp, sobelZ);

	cout << ".. done" << endl;

	for (i=0; i<numvals; i++){
		float gradientABS = gradX[i] * gradX[i] + gradY[i] * gradY[i] + gradZ[i] * gradZ[i];
		gradABS[i] = sqrtf(gradientABS);
	}

	//for (i=0; i<numvals; i++){
	//	if (gradABS[i] > 0.001){
	//		gradX[i] /= gradABS[i];
	//		gradY[i] /= gradABS[i];
	//		gradZ[i] /= gradABS[i];

	//		gradX[i] = 0.5f + 0.5f * gradX[i];
	//		gradY[i] = 0.5f + 0.5f * gradY[i];
	//		gradZ[i] = 0.5f + 0.5f * gradZ[i];
	//	}else{
	//		gradX[i] = 0.0f;
	//		gradY[i] = 0.0f;
	//		gradZ[i] = 0.0f;
	//	}
	//}

	for (i=0; i<numvals; i++){
		if (gradABS[i] > 0.001){
			gradX[i] /= gradABS[i];
			gradY[i] /= gradABS[i];
			gradZ[i] /= gradABS[i];
		}else{
			gradX[i] = 0.0f;
			gradY[i] = 0.0f;
			gradZ[i] = 0.0f;
		}
	}

	int slicesize = _xDim * _yDim;

	
	for (i=0; i<_xDim; i++){
		for (j=0; j<_yDim; j++){
			for (k=0; k<_zDim; k++){
				if (i == 0){
					int index = k * slicesize + j * _xDim + i;
					gradX[index] = -1.0f;
				} 
				if (i == (_xDim -1)){
					int index = k * slicesize + j * _xDim + i;
					gradX[index] = 1.0f;
				} 
				if (j == 0){
					int index = k * slicesize + j * _xDim + i;
					gradY[index] = -1.0f;
				}
				if (j == (_yDim -1)){
					int index = k * slicesize + j * _xDim + i;
					gradY[index] = 1.0f;
				}
				if (k == 0){
					int index = k * slicesize + j * _xDim + i;
					gradZ[index] = -1.0f;
				}
				if (k == (_zDim -1)){
					int index = k * slicesize + j * _xDim + i;
					gradZ[index] = 1.0f;
				}
			}
		}
	}	

	for (i=0; i<numvals; i++){
		gradX[i] = 0.5f + 0.5f * gradX[i];
		gradY[i] = 0.5f + 0.5f * gradY[i];
		gradZ[i] = 0.5f + 0.5f * gradZ[i];
	}

	GLubyte *compVals = new GLubyte[numvals];
	for (i=0; i<numvals; i++){
		compVals[i] = _texels[i * _components + comp];
	}

	if (_components != 4){
		delete [] _texels;
		_components = 4;
		_texels = new GLubyte[numvals * 4];
	}

	for (i=0; i<numvals; i++){
		_texels[i * 4 + 0] = (GLubyte)((int) (gradX[i] * 255.0f));
		_texels[i * 4 + 1] = (GLubyte)((int) (gradY[i] * 255.0f));
		_texels[i * 4 + 2] = (GLubyte)((int) (gradZ[i] * 255.0f));
		_texels[i * 4 + 3] = compVals[i];
	}

	delete [] gradX;
	delete [] gradY;
	delete [] gradZ;
	delete [] gradABS;
	delete [] compVals;
}

void CDataVolume::SetToBlurredGradientSobelComp2Alpha(int comp, float texScaleX, float texScaleY, float texScaleZ){
	int i, j, k;
	float *gradX, *gradY, *gradZ, *gradABS, *inputVals, *blurredInputVals;
	
	//float fac = 0.023076923f;
	//float center = 0.4f;
	
	float fac = 0.037037f;
	float center = 0.037037f;
	
	float blurOp[27] = {fac, fac, fac, fac, fac, fac, fac, fac, fac,
		fac, fac, fac, fac, center, fac, fac, fac, fac,
		fac, fac, fac, fac, fac, fac, fac, fac, fac};

	float sobelX[27] = {-2.0f, 0.0f, 2.0f, -3.0f, 0.0f, 3.0f, -2.0f, 0.0f, 2.0f,
		-3.0f, 0.0f, 3.0f, -6.0f, 0.0f, 6.0f, -3.0f, 0.0f, 3.0f,
		-2.0f, 0.0f, 2.0f, -3.0f, 0.0f, 3.0f, -2.0f, 0.0f, 2.0f};

	float sobelY[27] = {-2.0f, -3.0f, -2.0f, 0.0f, 0.0f, 0.0f, 2.0f, 3.0f, 2.0f,
		-3.0f, -6.0f, -3.0f, 0.0f, 0.0f, 0.0f, 3.0f, 6.0f, 3.0f,
		-2.0f, -3.0f, -2.0f, 0.0f, 0.0f, 0.0f, 2.0f, 3.0f, 2.0f};

	float sobelZ[27] = {-2.0f, -3.0f, -2.0f, -3.0f, -6.0f, -3.0f, -2.0f, -3.0f, -2.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		2.0f, 3.0f, 2.0f, 3.0f, 6.0f, 3.0f, 2.0f, 3.0f, 2.0f};

	int numvals = _xDim * _yDim * _zDim;
	int numvalscomp = numvals * _components;
	
	inputVals = new float[numvalscomp];
	for (i=0; i<numvalscomp; i++){
		inputVals[i] = (float)((int)_texels[i]);
	}
	blurredInputVals = Convolve3x3x3(inputVals, 4, comp, blurOp);
	//CopyBorderTexels(inputVals, blurredInputVals);
	CopyBorderTexelsFromInside(blurredInputVals);

	cout << "Computing Gradient for x direction" << endl;
	gradX = Convolve3x3x3(blurredInputVals, 1, comp, sobelX);

	cout << "Computing Gradient for y direction" << endl;
	gradY = Convolve3x3x3(blurredInputVals, 1, comp, sobelY);

	cout << "Computing Gradient for z direction" << endl;
	gradZ = Convolve3x3x3(blurredInputVals, 1, comp, sobelZ);
	gradABS = new float[numvals];

	for (i=0; i<numvals; i++){
		float gradientABS = gradX[i] * gradX[i] + gradY[i] * gradY[i] + gradZ[i] * gradZ[i];
		gradABS[i] = sqrtf(gradientABS);
	}

	for (i=0; i<numvals; i++){
		if (gradABS[i] > 100.0){
			gradX[i] /= gradABS[i];
			gradY[i] /= gradABS[i];
			gradZ[i] /= gradABS[i];
			//gradX[i] = 0.5f + 0.5f * gradX[i];
			//gradY[i] = 0.5f + 0.5f * gradY[i];
			//gradZ[i] = 0.5f + 0.5f * gradZ[i];
		}else{
			gradX[i] = 0.0f;
			gradY[i] = 0.0f;
			gradZ[i] = 0.0f;
		}
	}

	int slicesize = _xDim * _yDim;

	
	for (i=0; i<_xDim; i++){
		for (j=0; j<_yDim; j++){
			for (k=0; k<_zDim; k++){
				if (i == 0){
					int index = k * slicesize + j * _xDim + i;
					gradX[index] = -1.0f;
				} 
				if (i == (_xDim -1)){
					int index = k * slicesize + j * _xDim + i;
					gradX[index] = 1.0f;
				} 
				if (j == 0){
					int index = k * slicesize + j * _xDim + i;
					gradY[index] = -1.0f;
				}
				if (j == (_yDim -1)){
					int index = k * slicesize + j * _xDim + i;
					gradY[index] = 1.0f;
				}
				if (k == 0){
					int index = k * slicesize + j * _xDim + i;
					gradZ[index] = -1.0f;
				}
				if (k == (_zDim -1)){
					int index = k * slicesize + j * _xDim + i;
					gradZ[index] = 1.0f;
				}
			}
		}
	}

	for (i=0; i<numvals; i++){
		gradX[i] = 0.5f + 0.5f * gradX[i];
		gradY[i] = 0.5f + 0.5f * gradY[i];
		gradZ[i] = 0.5f + 0.5f * gradZ[i];
	}

	GLubyte *compVals = new GLubyte[numvals];
	for (i=0; i<numvals; i++){
		compVals[i] = _texels[i * _components + comp];
	}

	if (_components != 4){
		delete [] _texels;
		_components = 4;
		_texels = new GLubyte[numvals * 4];
	}

	//CopyBorderTexelsFromInside(gradX);
	//CopyBorderTexelsFromInside(gradY);
	//CopyBorderTexelsFromInside(gradZ);

	for (i=0; i<numvals; i++){
		_texels[i * 4 + 0] = (GLubyte)((int) (gradX[i] * 255.0f));
		_texels[i * 4 + 1] = (GLubyte)((int) (gradY[i] * 255.0f));
		_texels[i * 4 + 2] = (GLubyte)((int) (gradZ[i] * 255.0f));
		_texels[i * 4 + 3] = (GLubyte)((int) (blurredInputVals[i]));
		//_texels[i * 4 + 3] = compVals[i];
	}

	//GLubyte debugtexR[100000];
	//GLubyte debugtexG[100000];
	//GLubyte debugtexB[100000];
	//GLubyte debugtexA[100000];

	//for (i=0; i<100000; i++){
	//	debugtexR[i] = _texels[i * 4 + 0];
	//	debugtexG[i] = _texels[i * 4 + 1];
	//	debugtexB[i] = _texels[i * 4 + 2];
	//	debugtexA[i] = _texels[i * 4 + 3];
	//}

	delete [] gradX;
	delete [] gradY;
	delete [] gradZ;
	delete [] gradABS;
	delete [] compVals;
	delete [] inputVals;
	delete [] blurredInputVals;
}


float *CDataVolume::Convolve3x3x3(int comp, float *kernel){
	//Convolves the specified component of the current image with the 26 Neighbour Kernel
	int sliceSizeWComp, yLineSizeWComp;
	int xInd, yInd, zInd, i;

	_slicesize = _xDim * _yDim;

	sliceSizeWComp = _slicesize * _components;
	yLineSizeWComp = _xDim * _components;

	float vals[27];

	int curInd;
	int numvals = _xDim * _yDim * _zDim;
	float *newTexels = new float[_xDim * _yDim * _zDim];
	for (i=0; i<numvals; i++){
		newTexels[i] = 0.0f;
	}
	
	for (zInd = 1; zInd < _zDim-1; zInd++){
		for (yInd = 1; yInd < _yDim-1; yInd++){
			for (xInd = 1; xInd < _xDim-1; xInd++){
				for (i=0; i<27; i++) vals[i] = 0.0f;
				int ijk = GetIndex(xInd, yInd, zInd);
				int ijkcomp = ijk * _components;

				// Lower xy slice of the filter
				
				curInd = ijkcomp - _components - yLineSizeWComp - sliceSizeWComp + comp;

				vals[0] = (float) _texels[curInd];
				curInd += _components;
				vals[1] = (float) _texels[curInd];
				curInd += _components;
				vals[2] = (float) _texels[curInd];

				curInd -= _components;
				curInd -= _components;
				curInd += yLineSizeWComp;
				vals[3] = (float) _texels[curInd];
				curInd += _components;
				vals[4] = (float) _texels[curInd];
				curInd += _components;
				vals[5] = (float) _texels[curInd];

				curInd -= _components;
				curInd -= _components;
				curInd += yLineSizeWComp;
				vals[6] = (float) _texels[curInd];
				curInd += _components;
				vals[7] = (float) _texels[curInd];
				curInd += _components;
				vals[8] = (float) _texels[curInd];
				

				// Middle xy slice
				
				curInd = ijkcomp - _components - yLineSizeWComp + comp;

				vals[9] = (float) _texels[curInd];
				curInd += _components;
				vals[10] = (float) _texels[curInd];
				curInd += _components;
				vals[11] = (float) _texels[curInd];

				curInd -= _components;
				curInd -= _components;
				curInd += yLineSizeWComp;
				vals[12] = (float) _texels[curInd];
				curInd += _components;
				vals[13] = (float) _texels[curInd];
				curInd += _components;
				vals[14] = (float) _texels[curInd];

				curInd -= _components;
				curInd -= _components;
				curInd += yLineSizeWComp;
				vals[15] = (float) _texels[curInd];
				curInd += _components;
				vals[16] = (float) _texels[curInd];
				curInd += _components;
				vals[17] = (float) _texels[curInd];
				

				// Top slice
				
				curInd = ijkcomp - _components - yLineSizeWComp + sliceSizeWComp + comp;
				vals[18] = (float) _texels[curInd];
				curInd += _components;
				vals[19] = (float) _texels[curInd];
				curInd += _components;
				vals[20] = (float) _texels[curInd];

				curInd -= _components;
				curInd -= _components;
				curInd += yLineSizeWComp;
				vals[21] = (float) _texels[curInd];
				curInd += _components;
				vals[22] = (float) _texels[curInd];
				curInd += _components;
				vals[23] = (float) _texels[curInd];

				curInd -= _components;
				curInd -= _components;
				curInd += yLineSizeWComp;
				vals[24] = (float) _texels[curInd];
				curInd += _components;
				vals[25] = (float) _texels[curInd];
				curInd += _components;
				vals[26] = (float) _texels[curInd];

				newTexels[ijk] = 0.0f;
				for (i=0; i<27; i++){
					newTexels[ijk] += kernel[i] * vals[i];
				}
			}
		}
	}
	return newTexels;
}

float *CDataVolume::Convolve3x3x3(float *inputImage, int numComponents, int comp, float *kernel){
	int sliceSizeWComp, yLineSizeWComp;
	int xInd, yInd, zInd, i;

	int tmpcomponents = _components;
	_components = numComponents;

	_slicesize = _xDim * _yDim;

	sliceSizeWComp = _slicesize * _components;
	yLineSizeWComp = _xDim * _components;

	float vals[27];

	int curInd;
	int numvals = _xDim * _yDim * _zDim;
	float *newTexels = new float[_xDim * _yDim * _zDim];
	for (i=0; i<numvals; i++){
		newTexels[i] = 0.0f;
	}
	
	for (zInd = 1; zInd < _zDim-1; zInd++){
		for (yInd = 1; yInd < _yDim-1; yInd++){
			for (xInd = 1; xInd < _xDim-1; xInd++){
				for (i=0; i<27; i++) vals[i] = 0.0f;
				int ijk = GetIndex(xInd, yInd, zInd);
				int ijkcomp = ijk * _components;

				// Lower xy slice of the filter
				
				curInd = ijkcomp - _components - yLineSizeWComp - sliceSizeWComp + comp;

				vals[0] = (float) inputImage[curInd];
				curInd += _components;
				vals[1] = (float) inputImage[curInd];
				curInd += _components;
				vals[2] = (float) inputImage[curInd];

				curInd -= _components;
				curInd -= _components;
				curInd += yLineSizeWComp;
				vals[3] = (float) inputImage[curInd];
				curInd += _components;
				vals[4] = (float) inputImage[curInd];
				curInd += _components;
				vals[5] = (float) inputImage[curInd];

				curInd -= _components;
				curInd -= _components;
				curInd += yLineSizeWComp;
				vals[6] = (float) inputImage[curInd];
				curInd += _components;
				vals[7] = (float) inputImage[curInd];
				curInd += _components;
				vals[8] = (float) inputImage[curInd];
				

				// Middle xy slice
				
				curInd = ijkcomp - _components - yLineSizeWComp + comp;

				vals[9] = (float) inputImage[curInd];
				curInd += _components;
				vals[10] = (float) inputImage[curInd];
				curInd += _components;
				vals[11] = (float) inputImage[curInd];

				curInd -= _components;
				curInd -= _components;
				curInd += yLineSizeWComp;
				vals[12] = (float) inputImage[curInd];
				curInd += _components;
				vals[13] = (float) inputImage[curInd];
				curInd += _components;
				vals[14] = (float) inputImage[curInd];

				curInd -= _components;
				curInd -= _components;
				curInd += yLineSizeWComp;
				vals[15] = (float) inputImage[curInd];
				curInd += _components;
				vals[16] = (float) inputImage[curInd];
				curInd += _components;
				vals[17] = (float) inputImage[curInd];
				

				// Top slice
				
				curInd = ijkcomp - _components - yLineSizeWComp + sliceSizeWComp + comp;
				vals[18] = (float) inputImage[curInd];
				curInd += _components;
				vals[19] = (float) inputImage[curInd];
				curInd += _components;
				vals[20] = (float) inputImage[curInd];

				curInd -= _components;
				curInd -= _components;
				curInd += yLineSizeWComp;
				vals[21] = (float) inputImage[curInd];
				curInd += _components;
				vals[22] = (float) inputImage[curInd];
				curInd += _components;
				vals[23] = (float) inputImage[curInd];

				curInd -= _components;
				curInd -= _components;
				curInd += yLineSizeWComp;
				vals[24] = (float) inputImage[curInd];
				curInd += _components;
				vals[25] = (float) inputImage[curInd];
				curInd += _components;
				vals[26] = (float) inputImage[curInd];

				newTexels[ijk] = 0.0f;
				for (i=0; i<27; i++){
					newTexels[ijk] += kernel[i] * vals[i];
				}
			}
		}
	}

	_components = tmpcomponents;
	return newTexels;
}

void CDataVolume::CopyBorderTexels(float *input, float *output){
	int i, j, k;
	int slicesize = _xDim * _yDim;
	int xMaxInd = _xDim - 1;
	int yMaxInd = _yDim - 1;
	int zMaxInd = _zDim - 1;

	for (i=0; i<_xDim; i++){
		for (j=0; j<_yDim; j++){
			for (k=0; k<_zDim; k++){
				if ((i==0) || (i==xMaxInd) ||
					(j==0) || (j==yMaxInd) ||
					(k==0) || (k==zMaxInd)){
					int ind = k * slicesize + j * _xDim + i;
					output[ind] = input[ind];
				}
			}
		}
	}
}

void CDataVolume::CopyBorderTexelsFromInside(float *data){
	int i, j, k;
	int slicesize = _xDim * _yDim;
	int xMaxInd = _xDim - 1;
	int yMaxInd = _yDim - 1;
	int zMaxInd = _zDim - 1;

	for (i=0; i<_xDim; i++){
		for (j=0; j<_yDim; j++){
			for (k=0; k<_zDim; k++){
				int ind = k * slicesize + j * _xDim + i;
				int fromind = ind;

				if (i==0) fromind = fromind + 1;
				if (i==xMaxInd) fromind = fromind - 1;
				if (j==0) fromind = fromind + _xDim;
				if (j==yMaxInd) fromind = fromind - _xDim;
				if (k==0) fromind = fromind + slicesize;
				if (k==zMaxInd) fromind = fromind - slicesize;

				if (fromind != ind) {
					data[ind] = data[fromind];
				}
			}
		}
	}
}


void CDataVolume::SetTextureCheckerBoard(int xdim, int ydim, int zdim){
	int i, j, k, index, sliceSize;

	_xDim = xdim;
	_yDim = ydim;
	_zDim = zdim;

	_slicesize = sliceSize = _xDim * _yDim;
//#define GREYSCALE
#ifndef GREYSCALE
	_components = 4;
	_texels = new GLubyte[(_xDim * _yDim * _zDim * 4)];
	for (i=0; i<_zDim; i++){
		for (j=0; j<_yDim; j++){
			for (k=0; k<_xDim; k++){
				index = 4 * (k + j * _xDim + i * sliceSize);

				if ((i % 3) && (j % 3) && (k % 3)){
					_texels[index + 0] = (GLubyte) 255;
					_texels[index + 1] = (GLubyte) 255;
					_texels[index + 2] = (GLubyte) ((int) ((255.0f / _yDim) * j));
					_texels[index + 3] = (GLubyte) 255;
				} else {
					_texels[index + 0] = (GLubyte) 0;
					_texels[index + 1] = (GLubyte) 0;
					_texels[index + 2] = (GLubyte) 0;
					_texels[index + 3] = (GLubyte) 0;
				}
			}
		}
	}
#else
	assert(false);
	_components = 1;
	_texels = new GLubyte[(_xDim * _yDim * _zDim)];
	for (i=0; i<_zDim; i++){
		for (j=0; j<_yDim; j++){
			for (k=0; k<_xDim; k++){
				index = k + j * _xDim + i * sliceSize;
				if ((i % 2) && (j % 2) && (k % 2)){
					_texels[index] = (GLubyte) ((int) (100.0f + (155.0f  / _yDim) * j));
				} else {
					_texels[index] = (GLubyte) 30;
				}
			}
		}
	}
#endif
}

void CDataVolume::SetTextureCube(int xdim, int ydim, int zdim){
	int i, j, k, index, sliceSize;
	//int xp1 = xdim / 3;
	//int yp1 = ydim / 3;
	//int zp1 = zdim / 3;
	//int xp2 = xp1 * 2;
	//int yp2 = yp1 * 2;
	//int zp2 = zp1 * 2;

	int xp1 = xdim / 8;
	int yp1 = ydim / 8;
	int zp1 = zdim / 8;
	int xp2 = xp1 * 7;
	int yp2 = yp1 * 7;
	int zp2 = zp1 * 7;

	_xDim = xdim;
	_yDim = ydim;
	_zDim = zdim;

	_slicesize = sliceSize = _xDim * _yDim;
	_components = 4;
	_texels = new GLubyte[(_xDim * _yDim * _zDim * 4)];
	for (i=0; i<_zDim; i++){
		for (j=0; j<_yDim; j++){
			for (k=0; k<_xDim; k++){
				index = 4 * (k + j * _xDim + i * sliceSize);

				if ((i > zp1) && (i<zp2) && (j>yp1) && (j<yp2) && (k>xp1) && (k<xp2)){
					_texels[index + 0] = (GLubyte) 255;
					_texels[index + 1] = (GLubyte) 255;
					_texels[index + 2] = (GLubyte) 255;
					_texels[index + 3] = (GLubyte) 255;
				} else {
					_texels[index + 0] = (GLubyte) 0;
					_texels[index + 1] = (GLubyte) 0;
					_texels[index + 2] = (GLubyte) 0;
					_texels[index + 3] = (GLubyte) 0;
				}
			}
		}
	}
}

void CDataVolume::SetTextureSphere(int xdim, int ydim, int zdim){
	int i, j, k, index, sliceSize;

	int midX = xdim / 2;
	int midY = ydim / 2;
	int midZ = zdim / 2;

	int rx = xdim / 6;
	int ry = ydim / 6;
	int rz = zdim / 6;

	int rquad = rx * rx + ry * ry + rz * rz;


	_xDim = xdim;
	_yDim = ydim;
	_zDim = zdim;

	_slicesize = sliceSize = _xDim * _yDim;
	_components = 4;
	_texels = new GLubyte[(_xDim * _yDim * _zDim * 4)];
	for (i=0; i<_zDim; i++){
		for (j=0; j<_yDim; j++){
			for (k=0; k<_xDim; k++){
				index = 4 * (k + j * _xDim + i * sliceSize);

				int distQuad = ((i - midZ) * (i - midZ) + (j - midY) * (j - midY) + (k - midX) * (k - midX));

				if (distQuad < rquad){
					_texels[index + 0] = (GLubyte) (int) (255.0f - 255.0f * ((float) k) / ((float) _xDim));
					_texels[index + 1] = (GLubyte) (int) (255.0f * ((float) k) / ((float) _xDim));
					_texels[index + 2] = (GLubyte) (int) (255.0f * ((float) k) / ((float) _xDim));
					_texels[index + 3] = (GLubyte) (int) (255.0f * (1.0f - ((float) distQuad) / ((float) rquad)));
				} else {
					_texels[index + 0] = (GLubyte) 0;
					_texels[index + 1] = (GLubyte) 0;
					_texels[index + 2] = (GLubyte) 0;
					_texels[index + 3] = (GLubyte) 0;
				}
			}
		}
	}
}


