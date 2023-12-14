#pragma once
#include "glIncludes.h"
#include "complex.h"

class FFT
{
private:
	unsigned int mDim;
	unsigned int mWhich; // ???
	unsigned int mLog2Dim;
	unsigned int* mReversed;
	Complex** mT;
	Complex* mC[2];

	const float PI2 = 6.28318530718f;

public:
	FFT(unsigned int dim);
	
	~FFT();

	unsigned int Reverse(unsigned int i);

	Complex T(unsigned int x, unsigned int dim);

	void GetFFT(Complex* input, Complex* output, int stride, int offset);
};