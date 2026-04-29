#define GLFW_INCLUDE_NONE
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <matrix.h>
#include <math.h>
#include <cmath>
#include <control.h>
#include <iomanip>
#include <utils.h>

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 900

#define ITERATIONS 1000
#define DIMENSIONS 2

vector<3> lorentzT(double gamma, double vx, vector<3>& s1)
{
	matrix<3, 3> LT = {gamma, -gamma*vx, 0, -gamma*vx, gamma, 0, 0, 0, 1};
	vector<3> result = Multiply(LT, s1);
	return result;
}

unsigned int pointsToBeRendered(vector<3> p1, vector<3> p2, unsigned int windowsize)
{
	float deltaX = p1.data[1] - p2.data[1];
	float deltaY = p1.data[0] - p2.data[0];
	
	if (deltaX < 0)
	{
		deltaX = -deltaX;
	}

	if (deltaY < 0)
	{
		deltaY = -deltaY;
	}

	unsigned int pixelPos1, pixelPos2;

	if (deltaX > deltaY)
	{
		std::cout << "x: " << static_cast<int>(((p1.data[1]+1.0f) * windowsize / 2.0f) + 0.5f) << std::endl;
		std::cout << "x: " << static_cast<int>(((p2.data[1]+1.0f) * windowsize / 2.0f) + 0.5f) << std::endl;
		pixelPos1 = static_cast<int>(((p1.data[1]+1.0f) * windowsize / 2.0f) + 0.5f);
		pixelPos2 = static_cast<int>(((p2.data[1]+1.0f) * windowsize / 2.0f) + 0.5f);
		std::cout << "pixelx1 = " << pixelPos1 << ", pixelx2 =" << pixelPos2 << std::endl;
		
		return (pixelPos1 > pixelPos2) ? pixelPos1 - pixelPos2 : pixelPos2 - pixelPos1;
	}

	else
	{
		pixelPos1 = std::ceil((p1.data[0]+1) * windowsize / 2);
		pixelPos2 = std::ceil((p2.data[0]+1) * windowsize / 2);
		
		return (pixelPos1 > pixelPos2) ? pixelPos1 - pixelPos2 : pixelPos2 - pixelPos1;
	}
}


void generatePoints(vector<3> pos1, vector<3> pos2, unsigned int numOfPoints, double points[])
{
	double xIncrement, yIncrement;
	xIncrement = (pos2.data[1] - pos1.data[1]) / (numOfPoints - 1);
	yIncrement = (pos2.data[0] - pos1.data[0]) / (numOfPoints - 1);
	std::cout << xIncrement << ", " << yIncrement << std::endl;

	int i;
	for (i = 0; i < 2*numOfPoints; i+=2)
	{
		if (i == 0)
		{
			points[i] = pos1.data[1];
			points[i+1] = pos1.data[0];
		}

		
		else
		{
			points[i] = points[i-2] + xIncrement;
			points[i+1] = points[i-1] + yIncrement;
		}
	}

	

}

int main()
{
	int transform;
	std::cout << "0 para referencial do lab, 1 para referencial da nave: ";
	std::cin >> transform;

	vector<3> fpos1 = {0.2, 0.1, 1}, fpos2 = {-0.1, 0.5, 1};
	unsigned int numOfPoints = pointsToBeRendered(fpos1, fpos2, WINDOW_WIDTH);
	std::cout << "pontos a serem renderizados: " << numOfPoints << std::endl;
	double objectPoints[2*numOfPoints] = {0};
	generatePoints(fpos1, fpos2, numOfPoints, objectPoints);
	for (int i = 0; i < 2*numOfPoints; i+=2)
	{
		std::cout << objectPoints[i] << ", " << objectPoints[i+1] << std::endl;
		std::cout << i << std::endl;
	}
/*	
	matrix<3, 3> omega = {
		0, 0, 1,
		0, 0, 0.5,
		0, 0, 0
	};

	vector<3> fvel1 = Multiply(omega, fpos1);
	vector<3> fvel2 = Multiply(omega, fpos2);

	double v1 = fvel1.data[1] / fvel1.data[0];
	double v2 = fvel2.data[1] / fvel2.data[0];

	double gamma1 = 1/sqrt(1 - pow(v1, 2));
	double gamma2 = 1/sqrt(1 - pow(v2, 2));

	const double time_increment = 0.001;

	matrix<3, 3> timeScaledOmega = Scale(omega, time_increment);
	matrix<3, 3> omegaExp = Exponential(timeScaledOmega);

	double trajectory[2*ITERATIONS*DIMENSIONS] = {0};

	vector<3> f1 = fpos1, f2 = fpos2;

	for (auto i = 0; i < 2*ITERATIONS*DIMENSIONS; i+=2*DIMENSIONS)
	{
		if (transform == 1)
		{
			f1 = lorentzT(gamma1, v1, fpos1);
			f2 = lorentzT(gamma2, v2, fpos2);
		}

		else
		{
			f1 = fpos1;
			f2 = fpos2;
		}

		trajectory[i] = f1.data[1];
		trajectory[i+1] = f1.data[0];
		trajectory[i+2] = f2.data[1];
		trajectory[i+3] = f2.data[0];

		fpos1 = Multiply(omegaExp, fpos1);
		fpos2 = Multiply(omegaExp, fpos2);
	}
*/
	GLFWwindow* window;
	int initialized = init(window, WINDOW_WIDTH, WINDOW_HEIGHT);

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(objectPoints), objectPoints, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 2*sizeof(double), 0);

	while(!glfwWindowShouldClose(window))
	{

		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_POINTS, 0, numOfPoints);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

}

