#define GLFW_INCLUDE_NONE
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <matrix.h>
#include <math.h>
#include <control.h>
#include <iomanip>

#define WINDOW_WIDTH 720
#define WINDOW_HEIGHT 720

#define POINTS 5000
#define DIMENSIONS 2
#define TOLERANCE 0.001


int main()
{
	std::cout << "OBS: a escolha de velocidade inicial será automaticamente setada para 0." << std::endl;
	simulation::state s1;
	simulation::SetInitialValues(s1);
	
	double x2;
	std::cout << "x2: ";
	std::cin >> x2;

	std::cout << "eixos visiveis: " << s1.visibleAxis1 << " " << s1.visibleAxis2 << std::endl;
	
	static double trajectory1[POINTS*DIMENSIONS] = {0};
	static double trajectory2[POINTS*DIMENSIONS] = {0};

	const double time_increment = 0.001;

	matrix<3, 3> omega = {
		0                         , s1.linAcceleration.data[0], s1.linAcceleration.data[1],
		s1.linAcceleration.data[0] , 0                        , s1.angAcceleration.data[0],
		s1.linAcceleration.data[1] , -s1.angAcceleration.data[0], 0
	};

	matrix<3, 3> timeScaledOmega = Scale(omega, time_increment);
	matrix<3, 3> omegaExp = Exponential(timeScaledOmega);

	vector<3> fourPosition1 = {0, s1.spacialPosition.data[0], 0};
	vector<3> fourPosition2 = {0, x2, 0};

	vector<3> fourVelocity1 = Multiply(omega, fourPosition1);
	vector<3> fourVelocity2 = Multiply(omega, fourPosition2);

	if (ApplyMinkowskiMetric(fourVelocity1, fourVelocity1) <= 0 || ApplyMinkowskiMetric(fourVelocity2, fourVelocity2) <= 0)
	{
		std::cout << "condicoes iniciais invalidas." << std::endl;
		return -1;
	}
	
	std::cout.precision(15);
	std::cout << std::fixed;

	const double positionInvariant = sqrt(-ApplyMinkowskiMetric(fourPosition1, fourPosition1));
	std::cout << positionInvariant << std::endl;
	double positionMetric;

	
	for (auto i = 0; i < sizeof(trajectory1)/sizeof(double); )
	{
		if (s1.visibleAxis1 != 'y' && s1.visibleAxis2 != 'y')
		{
			trajectory1[i] = fourPosition1.data[1];
			trajectory1[i+1] = fourPosition1.data[0];
			trajectory2[i] = fourPosition2.data[1];
			trajectory2[i+1] = fourPosition2.data[0];
		}

		else if (s1.visibleAxis1 == 't' || s1.visibleAxis2 == 't')
		{
			trajectory1[i] = fourPosition1.data[2];
			trajectory1[i+1] = fourPosition1.data[0];
			trajectory2[i] = fourPosition2.data[2];
			trajectory2[i+1] = fourPosition2.data[0];
		}
		
		else
		{
			trajectory1[i] = fourPosition1.data[1];
			trajectory1[i+1] = fourPosition1.data[2];
			trajectory2[i] = fourPosition2.data[1];
			trajectory2[i+1] = fourPosition2.data[2];
		}

		fourPosition1 = Multiply(omegaExp, fourPosition1);
		fourPosition2 = Multiply(omegaExp, fourPosition2);
/*
		positionMetric = ApplyMinkowskiMetric(fourPosition1, fourPosition1);

		if(positionInvariant - positionMetric > TOLERANCE || positionMetric - positionInvariant > TOLERANCE)
		{
			std::cout << "a metrica passou da tolerancia em " << ((positionInvariant > positionMetric) ? (positionInvariant - positionMetric) : (positionMetric - positionInvariant)) << " em i = " << i << std::endl;
		}
*/
		i += DIMENSIONS;
	}


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

	unsigned int buffer1;
	glGenBuffers(1, &buffer1);
	glBindBuffer(GL_ARRAY_BUFFER, buffer1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trajectory1), trajectory1, GL_STATIC_DRAW);

	unsigned int buffer2;
	glGenBuffers(1, &buffer2);
	glBindBuffer(GL_ARRAY_BUFFER, buffer2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trajectory2), trajectory2, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);

	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glBindBuffer(GL_ARRAY_BUFFER, buffer1);
		glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, DIMENSIONS*sizeof(double), 0);
		glDrawArrays(GL_POINTS, 0, POINTS);

		glBindBuffer(GL_ARRAY_BUFFER, buffer2);
		glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, DIMENSIONS*sizeof(double), 0);
		glDrawArrays(GL_POINTS, 0, POINTS);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}
