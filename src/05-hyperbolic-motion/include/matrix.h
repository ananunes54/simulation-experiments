#ifndef MATRIX_INCLUDED_H
#define MATRIX_INCLUDED_H

#include <array>

template<int I, int J>
struct matrix
{
	std::array<double, I*J> data;
};

template<int I>
using vector = matrix<I, 1>;

template<int J>
using covector = matrix<1, J>;

template<int I, int J>
matrix<I, J> Add(matrix<I, J>& m1, matrix<I, J>& m2);

template<int I, int J>
matrix<I, J> Scale(matrix<I, J>& m, double factor);

template<int I, int J, int K>
matrix<I, J> Multiply(matrix<I, K>& m1, matrix<K, J>& m2);

template<int I, int J>
matrix<I, J> Transpose(matrix<J, I>& m);

template<int N>
matrix<N, N> Exponential(matrix<N, N>& m);

template<int N>
matrix<N, N> MinkowskiMatrix();

template<int N>
double ApplyMetric(vector<N>& v1, vector<N>& v2);

template<int I, int J>
void Print(matrix<I, J>& m);
#endif
