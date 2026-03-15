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

	Matrix(const Matrix& other) : m_rows(other.m_rows), m_columns(other.m_columns)
	{
		m_data = std::make_unique<double[]>(m_rows*m_columns);
		for (auto i = 0; i < m_rows*m_columns; i++)
		{
			m_data[i] = other[i];
		}
	}

	Matrix(int rows, int columns) : m_rows(rows), m_columns(columns)
	{
		m_data = std::make_unique<double[]>(m_rows*m_columns);
		for (auto i = 0; i < m_rows*m_columns; i++)
		{
			m_data[i] = 0;
		}
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
		if (other.m_rows != m_rows || other.m_columns != m_columns)
		{
			std::cout << "matriz invalida para a operacao." << std::endl;
			return;
		}

		for (int i = 0; i < m_rows * m_columns; i++)
		{
			m_data[i] += other[i];
		}
	}

	Matrix& operator=(const Matrix& other)
	{
		if (&other == this) 
		{
			return *this;
		}

		if (other.m_rows != m_rows || other.m_columns != m_columns)
		{
			std::cout << "numero de linhas e colunas incompativel; a copia nao foi concluida." << std::endl;
			return *this;
		}

		for (auto i = 0; i < m_rows*m_columns; i++)
		{
			m_data[i] = other[i];
		}
		
		return *this;
	}

	Matrix& operator=(Matrix&& other)
	{
		if (this == &other)
		{
			return *this;
		}

		if (other.m_rows != m_rows || other.m_columns != m_columns)
		{
			std::cout << "numero de linhas e colunas incompativel; a copia nao foi concluida." << std::endl;
			return *this;
		}

		m_data = std::move(other.m_data);
		return *this;
	}

	Matrix operator*(const Matrix& other) const
	{
		if (&other == this)
		{
			return *this;
		}

		if (m_columns != other.m_rows)
		{
			std::cout << "numero de linhas e colunas incompativel; matriz nula retornada com ";
			std::cout << m_rows << "linhas e ";
			std::cout << m_columns << "colunas." << std::endl;

			Matrix temp(m_rows, m_columns);

			for (auto i = 0; i < m_columns*m_rows; i++)
			{
				temp.m_data[i] = 0;
			}

			return temp;
		}

		Matrix temp(m_rows, other.m_columns);

		{
			std::cout << "matriz da multiplicacao: ";
			std::cout << m_rows << "linhas, ";
			std::cout << other.m_columns << "colunas." << std::endl;

			for (auto i = 0; i < m_rows; i++)
			{
				for (auto j = 0, sum = 0; j < other.m_columns; j++)
				{
					for (auto k = 0; k < m_columns; k++)
					{
						sum += m_data[i*m_columns + k] * other.m_data[other.m_columns*k + j];
					}

					temp.m_data[i*other.m_columns + j] = sum;
				}
			}
		}

		return temp;
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
	Matrix n = m;
	n.Print();
	}
	return 0;
}
