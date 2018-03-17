#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include "complex.h"

class FastFourierTransform
{
private:
	unsigned int N, which;

	unsigned int log2n;
	float pi2;
	unsigned int *reversed;

	complex **T;
	complex *c[2];

public:
	FastFourierTransform(unsigned int N);
	~FastFourierTransform();

	unsigned int reverse(unsigned int i);

	complex t(unsigned int x, unsigned int N);

	void fft(complex* input, complex* output, int stride, int offset);
};