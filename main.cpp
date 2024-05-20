#include "./MultipleBodyProblem.hpp"

int main()
{
	constexpr uint8_t nBodies{3};

	Position pos1{0.536387073390469, 0.054088605007709};
	Position pos2{-0.252099126491433, 0.694527327749042};
	Position pos3{-0.275706601688421, -0.335933589317989};
	std::array<Position, nBodies> startingPositions{pos1, pos2, pos3};

	Velocity v1{-0.569379585580752, 1.255291102530929};
	Velocity v2{0.079644615251500, -0.458625997341406};
	Velocity v3{0.489734970329286, -0.796665105189482};
	std::array<Velocity, nBodies> startingVelocities{v1, v2, v3};

	std::array<double, nBodies> masses{1, 1, 1};

	MultipleBodyProblem<nBodies> testCase(0.0001, 1, startingPositions, startingVelocities, masses);
	testCase.doTimeIterations();
	// testCase.calculateAfterPosition();
}