//#include <math.h>
#include <iostream>
#include <memory>

class Matrix
{

private:
	int m_rows, m_columns;
	std::unique_ptr<double[]> m_data;

public:
	Matrix(std::initializer_list<double> data, int rows, int columns) : m_rows(rows), m_columns(columns)
	{
		unsigned int size = data.size();

		m_data = std::make_unique<double[]>(size);
		auto a = 0;
		for (auto i : data)
		{
			m_data[a++] = i;
		}

		if (m_rows * m_columns != size)
		{
			std::cout << "especificacao de linhas e colunas invalida. a matriz decaiu para um vetor." << std::endl;
			m_rows = size;
			m_columns = 1;
			return;
		}
	
		std::cout << "construtor ok" << std::endl;
	}

	double operator[](int index)
	{
		if (index < 0 || index >= (m_rows * m_columns))
		{
			return 0;
		}

		return m_data[index];
	}

	int GetRows()
	{
		return m_rows;
	}

	int GetColumns()
	{
		return m_columns;
	}

	~Matrix()
	{
		std::cout << "destrutor ok" << std::endl;
	}
};

int main()
{
	{
	Matrix m({0, 1, 2}, 3, 1);
	std::cout << m.GetRows() << ", " << m.GetColumns() << std::endl;
	std::cout << m[2] << std::endl;
	}
	return 0;
}
