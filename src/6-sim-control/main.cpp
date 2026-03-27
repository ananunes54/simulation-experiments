#include <iostream>
#include <matrix.h>

namespace simulation
{
	struct state
	{
		enum axis {time, X, Y, Z};
		vector<3> linAcceleration = {0, 0, 0};
		vector<3> angAcceleration = {0, 0, 0};
		vector<3> spacialVelocity = {0, 0, 0};
		vector<3> spacialPosition = {0, 0, 0};
		axis visibleAxis1;
		axis visibleAxis2;
	};

	void SetInitialValues(simulation::state& state)
	{
		std::cout << "aceleracao linear: ";
		std::cin >> state.linAcceleration.data[0] >> state.linAcceleration.data[1] >> state.linAcceleration.data[2];
		std::cout << "aceleracao angular: ";
		std::cin >> state.angAcceleration.data[0] >> state.angAcceleration.data[1] >> state.angAcceleration.data[2];
		std::cout << "velocidade: ";
		std::cin >> state.spacialVelocity.data[0] >> state.spacialVelocity.data[1] >> state.spacialVelocity.data[2];
		std::cout << "posicao: ";
		std::cin >> state.spacialPosition.data[0] >> state.spacialPosition.data[1] >> state.spacialPosition.data[2];
	}
}

int main()
{
	simulation::state state;

	simulation::SetInitialValues(state);

	Print(state.linAcceleration);
	Print(state.angAcceleration);
	Print(state.spacialVelocity);
	Print(state.spacialPosition);

}
