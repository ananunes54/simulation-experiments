#define GLFW_INCLUDE_NONE
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <matrix.h>
#include <math.h>
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
		pixelPos1 = std::ceil((p1.data[1]+1) * windowsize / 2);
		pixelPos2 = std::ceil((p2.data[1]+1) * windowsize / 2);
		
		return (pixelPos1 > pixelPos2) ? pixelPos1 - pixelPos2 : pixelPos2 - pixelPos1;
	}

	else
	{
		pixelPos1 = std::ceil((p1.data[0]+1) * windowsize / 2);
		pixelPos2 = std::ceil((p2.data[0]+1) * windowsize / 2);
		
		return (pixelPos1 > pixelPos2) ? pixelPos1 - pixelPos2 : pixelPos2 - pixelPos1;
	}
}

int main()
{
	int transform;
	std::cout << "0 para referencial do lab, 1 para referencial da nave: ";
	std::cin >> transform;

	vector<3> fpos1 = {0, 0.1, 1}, fpos2 = {0, 0.2, 1};
	unsigned int numOfPoints = pointsToBeRendered(fpos1, fpos2, WINDOW_WIDTH);
	std::cout << "pontos a serem renderizados: " << numOfPoints << numOfPoints << std::endl;

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

	GLFWwindow* window;
	int initialized = init(window, WINDOW_WIDTH, WINDOW_HEIGHT);

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trajectory), trajectory, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 2*sizeof(double), 0);

	while(!glfwWindowShouldClose(window))
	{

		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_LINES, 0, ITERATIONS);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}

