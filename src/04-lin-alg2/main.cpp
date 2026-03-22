#include <iostream>
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
matrix<I, J> Add(matrix<I, J>& m1, matrix<I, J>& m2)
{
	matrix<I, J> result;
	for (auto c = 0; c < I*J; c++)
	{
		result.data[c] = m1.data[c] + m2.data[c];
	}
	return result;
}

template<int I, int J>
matrix<I, J> Scale(matrix<I, J>& m, double factor)
{
	matrix<I, J> result;
	for (auto c = 0; c < I*J; c++)
	{
		result.data[c] = factor * m.data[c];	
	}
	return result;
}

template<int I, int J, int K>
matrix<I, J> Multiply(matrix<I, K>& m1, matrix<K, J>& m2)
{
	auto m1Columns = K;
	auto m2Columns = J;
	auto resultColumns = J;
	auto sum = 0;
	matrix<I, J> result;

	for (auto i = 0; i < I; i++)
	{
		for (auto j = 0; j < J; j++)
		{
			for (auto k = 0; k < K; k++)
			{
				sum += m1.data[i*m1Columns + k] * m2.data[k*m2Columns + j];
			}

			result.data[i*resultColumns + j] = sum;
			sum = 0;
		}
	}
	return result;
}

template<int I, int J>
matrix<I, J> Transpose(matrix<J, I>& m)
{
	auto mColumns = I;
	auto resultColumns = J;
	matrix<I, J> result;
	for (auto i = 0; i < I;	i++)
	{
		for (auto j = 0; j < J; j++)
		{
			result.data[i*resultColumns + j] = m.data[j*mColumns + i];
		}
	}
	return result;
}

template<int N>
matrix<N, N> MinkowskiMatrix()
{
	matrix<N, N> result;
	for (auto c = 0; c < N*N; c++)
	{
		if(c == 0)
		{
			result.data[0] = 1;
		}

		else if(c%(N+1) == 0)
		{
			result.data[c] = -1;
		}

		else
		{
			result.data[c] = 0;
		}
	}
	return result;
}

template<int N>
double ApplyMetric(vector<N>& v1, vector<N>& v2)
{
	matrix<N, N> minkowski = MinkowskiMatrix<N>();
	v1 = Multiply(minkowski, v1);
	covector<N> c1 = Transpose(v1);
	matrix<1, 1> result = Multiply(c1, v2);
	return result.data[0];
}

template<int I, int J>
void Print(matrix<I, J>& m)
{
	for (auto i = 0; i < I; i++)
	{
		for (auto j = 0; j < J; j++)
		{
			std::cout << m.data[i*J + j] << " ";
		}

		std::cout << std::endl;
	}
}


int main()
{
	vector<3> v1 = {1, 1, 1};
	vector<3> v2 = v1;
	double r = ApplyMetric(v1, v2);
	std::cout << r << std::endl;
}
