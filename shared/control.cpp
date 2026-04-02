#include <iostream>
#include <control.h>
#include <matrix.h>

namespace simulation
{
	struct state
	{	
		vector<3> linAcceleration = {0, 0, 0};
		vector<3> angAcceleration = {0, 0, 0};
		vector<3> spacialVelocity = {0, 0, 0};
		vector<3> spacialPosition = {0, 0, 0};
		char visibleAxis1;
		char visibleAxis2;
	};

	void SetInitialValues(state& s)
	{
		std::cout << "aceleracao linear: ";
		std::cin >> s.linAcceleration.data[0] >> s.linAcceleration.data[1] >> s.linAcceleration.data[2];
		std::cout << "aceleracao angular: ";
		std::cin >> s.angAcceleration.data[0] >> s.angAcceleration.data[1] >> s.angAcceleration.data[2];
		std::cout << "velocidade: ";
		std::cin >> s.spacialVelocity.data[0] >> s.spacialVelocity.data[1] >> s.spacialVelocity.data[2];
		std::cout << "posicao: ";
		std::cin >> s.spacialPosition.data[0] >> s.spacialPosition.data[1] >> s.spacialPosition.data[2];
		std::cout << "eixos visiveis: ";
		std::cin >> s.visibleAxis1 >> s.visibleAxis2;
	}
}
