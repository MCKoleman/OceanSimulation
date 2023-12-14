#include "fft.h"

FFT::FFT(unsigned int dim)
	: mDim(dim)
{
	mLog2Dim = log(mDim) / log(2);

	// For bit reversals
	mReversed = new unsigned int[mDim];
	for (int i = 0; i < mDim; i++)
		mReversed[i] = Reverse(i);

	int pow2 = 1;
	mT = new Complex*[mLog2Dim];
	for (int i = 0; i < mLog2Dim; i++)
	{
		mT[i] = new Complex[pow2];
		for (int j = 0; j < pow2; j++)
			mT[i][j] = T(j, pow2 * 2);
		pow2 *= 2;
	}

	mC[0] = new Complex[mDim];
	mC[1] = new Complex[mDim];
	mWhich = 0;
}

FFT::~FFT()
{
	if (mC[0])
		delete[] mC[0];
	if (mC[1])
		delete[] mC[1];
	if (mT)
	{
		for (int i = 0; i < mLog2Dim; i++)
			if (mT[i])
				delete[] mT[i];
		delete mT;
	}
	if (mReversed)
		delete mReversed;
}

unsigned int FFT::Reverse(unsigned int i)
{
	unsigned int res = 0;
	for (int j = 0; j < mLog2Dim; j++)
	{
		res = (res << 1) + (i & 1);
		i >>= 1;
	}
	return res;
}

Complex FFT::T(unsigned int x, unsigned int dim)
{
	return Complex(cos(PI2 * x / dim), sin(PI2 * x / dim));
}

void FFT::GetFFT(Complex* input, Complex* output, int stride, int offset)
{
	for (int i = 0; i < mDim; i++)
	{
		mC[mWhich][i] = input[mReversed[i] * stride + offset];
	}

	int loops = mDim >> 1;
	int size = 1 << 1;
	int sizeOver2 = 1;
	int w = 0;

	for (int i = 1; i <= mLog2Dim; i++)
	{
		mWhich ^= 1;
		for (int j = 0; j < loops; j++)
		{
			for (int k = 0; k < sizeOver2; k++)
			{
				mC[mWhich][size * j + k] = mC[mWhich ^ 1][size * j + k] + mC[mWhich ^ 1][size * j + sizeOver2 + k] * mT[w][k];
			}

			for (int k = sizeOver2; k < size; k++)
			{
				mC[mWhich][size * j + k] = mC[mWhich ^ 1][size * j - sizeOver2 + k] - mC[mWhich ^ 1][size * j + k] * mT[w][k - sizeOver2];
			}
		}
		loops >>= 1;
		size <<= 1;
		sizeOver2 <<= 1;
		w++;
	}

	for (int i = 0; i < mDim; i++)
		output[i * stride + offset] = mC[mWhich][i];
}