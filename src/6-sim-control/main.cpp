#include <iostream>
#include <exception>

class teste
{
public:
	teste()
	{
		std::cout << "objeto criado" << std::endl;
	}

	void funcao()
	{
		throw std::runtime_error("erro ao criar objeto");
	}

	~teste()
	{
		std::cout << "objeto destruido" << std::endl;
	}
};


int main ()
{
	try
	{
		teste a;
		a.funcao();
	}
	catch (std::exception& e)
	{
		std::cout << "exceção encontrada: " << e.what() << std::endl;
	}

	return 0;
}
