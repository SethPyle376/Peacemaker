#include "FastFourierTransform.h"

FastFourierTransform::FastFourierTransform(unsigned int N)
{
	this->N = N;
	reversed = 0;
	T = 0;
	pi2 = (2 * M_PI);

	c[0] = 0;
	c[1] = 0;

	log2n = log(N) / log(2);

	reversed = new unsigned int[N];

	for (int i = 0; i < N; i++)
		reversed[i] = reverse(i);

	int pow2 = 1;

	T = new complex*[log2n];

	for (int i = 0; i < log2n; i++)
	{
		T[i] = new complex[pow2];

		for (int j = 0; j < pow2; j++)
			T[i][j] = t(j, pow2 * 2);
		pow2 *= 2;
	}

	c[0] = new complex[N];
	c[1] = new complex[N];
	which = 0;
}

FastFourierTransform::~FastFourierTransform()
{
	if (c[0]) delete[] c[0];
	if (c[1]) delete[] c[1];
	if (T) {
		for (int i = 0; i < log2n; i++) if (T[i]) delete[] T[i];
		delete[] T;
	}
	if (reversed) delete[] reversed;
}

unsigned int FastFourierTransform::reverse(unsigned int i) {
	unsigned int res = 0;
	for (int j = 0; j < log2n; j++) {
		res = (res << 1) + (i & 1);
		i >>= 1;
	}
	return res;
}


complex FastFourierTransform::t(unsigned int x, unsigned int N) 
{
	return complex(cos(pi2 * x / N), sin(pi2 * x / N));
}

void FastFourierTransform::fft(complex* input, complex* output, int stride, int offset) {
	for (int i = 0; i < N; i++) c[which][i] = input[reversed[i] * stride + offset];

	int loops = N >> 1;
	int size = 1 << 1;
	int size_over_2 = 1;
	int w_ = 0;
	for (int i = 1; i <= log2n; i++) {
		which ^= 1;
		for (int j = 0; j < loops; j++) {
			for (int k = 0; k < size_over_2; k++) {
				c[which][size * j + k] = c[which ^ 1][size * j + k] +
					c[which ^ 1][size * j + size_over_2 + k] * T[w_][k];
			}

			for (int k = size_over_2; k < size; k++) {
				c[which][size * j + k] = c[which ^ 1][size * j - size_over_2 + k] -
					c[which ^ 1][size * j + k] * T[w_][k - size_over_2];
			}
		}
		loops >>= 1;
		size <<= 1;
		size_over_2 <<= 1;
		w_++;
	}

	for (int i = 0; i < N; i++) 
		output[i * stride + offset] = c[which][i];
}