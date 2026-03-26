#define GLFW_INCLUDE_NONE
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <matrix.h>
#include <math.h>

#define WINDOW_WIDTH 720
#define WINDOW_HEIGHT 720

#define POINTS 100
#define DIMENSIONS 3
#define TOLERANCE 0.001

int main()
{
	double trajectory[POINTS*DIMENSIONS] = {0};
	const double time_increment = 0.001;

	matrix<3, 3> omega = {
		0, 0.1, 0,
		0.1, 0, 0,
		0, 0, 0
	};

	matrix<3, 3> timeScaledOmega = Scale(omega, time_increment);
	matrix<3, 3> omegaExp = Exponential(timeScaledOmega);

	double v = 0.05;
	double gamma = 1/sqrt(1 - pow(v, 2));

	vector<3> fourVelocity = {gamma, gamma * v, 0};
	const double velocityInvariant = sqrt(ApplyMinkowskiMetric(fourVelocity, fourVelocity));
	std::cout << velocityInvariant << std::endl;
	double velocityMetric;

	vector<3> fourAcceleration = Multiply(omega, fourVelocity);
	const double accelerationInvariant = sqrt(-ApplyMinkowskiMetric(fourAcceleration, fourAcceleration));
	std::cout << accelerationInvariant << std::endl;
	double accelerationMetric;
	double auMetric;

	vector<3> fourPosition = {0, 1/accelerationInvariant, 0};
	const double positionInvariant = sqrt(-ApplyMinkowskiMetric(fourPosition, fourPosition));
	std::cout << positionInvariant << std::endl;
	double positionMetric;
	
	for (auto i = 0; i < sizeof(trajectory)/sizeof(double); )
	{
		fourVelocity = Multiply(omegaExp, fourVelocity);
		velocityMetric = ApplyMinkowskiMetric(fourVelocity, fourVelocity);
		
		fourAcceleration = Multiply(omega, fourVelocity);
		accelerationMetric = ApplyMinkowskiMetric(fourAcceleration, fourAcceleration);
		auMetric = ApplyMinkowskiMetric(fourVelocity, fourAcceleration);

		if ( TOLERANCE < velocityMetric - velocityInvariant || TOLERANCE < velocityInvariant - velocityMetric)
		{
			std::cout << "velocidade: " << velocityMetric - velocityInvariant << std::endl;
			//std::cout << "a quadri-velocidade passou da tolerancia em i = " << i << std::endl;
		}

		if ( TOLERANCE < accelerationMetric - accelerationInvariant || TOLERANCE > accelerationInvariant - accelerationMetric)
		{
			std::cout << "aceleracao: " << accelerationMetric << std::endl;
			//std::cout << "a quadri-posicao passou da tolerancia em i = " << i << std::endl;
		}

		if ( TOLERANCE < auMetric || -TOLERANCE > auMetric)
		{
			std::cout << "velocidade-aceleracao: " << auMetric << std::endl;
			//std::cout << "o produto quadri-velocidade quadri-aceleracao passou da tolerancia em i = " << i << std::endl;
		}

		i += DIMENSIONS;
	}

/*
	glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
	glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
		
	if (!glfwInit())
	{
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "window", NULL, NULL);

	if (!window)
	{
		std::cout << "erro ao criar janela." << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "erro ao carregar glad." << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trajectory), trajectory GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, DIMENSIONS, GL_DOUBLE, GL_TRUE, DIMENSIONS*sizeof(double), 0);


	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_POINTS, 0, 4);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
*/
}
