//#include <math.h>
#include <iostream>
#include <memory>

class Matrix
{

private:
	int rows, columns;
	std::unique_ptr<double> data;

public:
	Matrix()
	{
		data = std::make_unique<double>();
	
		std::cout << "construtor ok" << std::endl;
	}

	~Matrix()
	{
		std::cout << "destrutor ok" << std::endl;
	}
};

int main()
{
	{
	Matrix m;
	}
	return 0;
}
