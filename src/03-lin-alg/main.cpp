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

	double operator[] (int index) const
	{
		if (index < 0 || index >= (m_rows * m_columns))
		{
			return 0;
		}

		return m_data[index];
	}

	void operator+=(Matrix &other)
	{
		if (other.GetRows() != m_rows || other.GetColumns() != m_columns)
		{
			std::cout << "matriz invalida para a operacao." << std::endl;
			return;
		}

		for (int i = 0; i < m_rows * m_columns; i++)
		{
			m_data[i] += other[i];
		}
	}

	void Scale(double factor)
	{
		for (auto i = 0; i < m_rows*m_columns; i++)
		{
			m_data[i] *= factor;
		}
	}

	int GetRows() const
	{
		return m_rows;
	}

	int GetColumns() const
	{
		return m_columns;
	}

	void Print() const
	{
		for (auto i = 0; i < m_rows; i++)
		{
			for (auto j = 0; j < m_columns; j++)
			{
				std::cout << m_data[i*m_columns + j] << " ";
			}
			std::cout << std::endl;
		}
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
	m.Scale(0);
	m.Print();
	}
	return 0;
}
