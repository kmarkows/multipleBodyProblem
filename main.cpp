#include "./MultipleBodyProblem.hpp"

int main()
{
	constexpr uint8_t nBodies{3};

	Position pos1{-0.602885898116520, 1.059162128863347e-1};
	Position pos2{0.252709795391000, 1.058254872224370e-1};
	Position pos3{-0.355389016941814, 1.038323764315145e-1};
	std::array<Position, nBodies> startingPositions{pos1, pos2, pos3};

	Velocity v1{0.122913546623784, 0.747443868604908};
	Velocity v2{-0.019325586404545, 1.369241993562101};
	Velocity v3{-0.103587960218793, -2.116685862168820};
	std::array<Velocity, nBodies> startingVelocities{v1, v2, v3};

	std::array<double, nBodies> masses{1, 1, 1};

	MultipleBodyProblem<nBodies> testCase(0.0001, 1, startingPositions, startingVelocities, masses);
	testCase.doTimeIterations();
	// testCase.calculateAfterPosition();
}