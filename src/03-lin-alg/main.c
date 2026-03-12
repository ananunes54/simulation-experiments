//#include <math.h>
#include <iostream>

struct Vec
{
	float t;
	float x;
	float y;
};

struct Matrix
{
	int rows, columns;
	Vec* data;

	Matrix(float t, float x, float y)
	{
		data = new Vec;
		data->t = t;
		data->x = x;
		data->y = y;
	
		std::cout << "ok" << std::endl;
	}
};

int main()
{
	Matrix *m = new Matrix(1, 0, 0);
	std::cout << m->data->t << m->data->x << m->data->y << std::endl;
	return 0;
}
