#include "./Body.hpp"
#include "./MathHelper.hpp"
#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

constexpr double gravConst{1};

template <uint8_t nBodies> class MultipleBodyProblem
{
	double dt{};
	uint8_t nt{};
	const uint8_t size = nBodies;

  public:
	std::array<Body<nBodies>, nBodies> bodiesBefore{};
	std::array<Body<nBodies>, nBodies> bodiesAfter{};
	std::vector<std::array<Position, nBodies>> positionData{};

	MultipleBodyProblem(double dt, uint8_t nt, std::array<Position, nBodies> startingPositions,
						std::array<Velocity, nBodies> startingVelocities, std::array<double, nBodies> masses)
	{
		this->dt = dt;
		this->nt = nt;
		for (uint8_t i = 0; i < size; i++)
		{
            bodiesBefore.at(i).pos = startingPositions.at(i);
            bodiesBefore.at(i).v = startingVelocities.at(i);
			bodiesBefore.at(i).id = i;
			bodiesAfter.at(i).id = i;
			bodiesBefore.at(i).mass = masses.at(i);
			bodiesAfter.at(i).mass = masses.at(i);
		}
	}

	void calculateRadiousesAndSinCosAlpha()
	{
		// std::cout << " --------- calculateRadiousesAndSinCosAlpha --------- " << std::endl;
		for (uint8_t i = 0; i < size; i++)
		{
			for (uint8_t j = 0; j < size; j++)
			{
				if (j == i)
				{
					bodiesBefore.at(i).radiuses.at(j) = 0;
					bodiesBefore.at(i).sinAlpha.at(j) = 0;
                    bodiesBefore.at(i).cosAlpha.at(j) = 0;
					// std::cout << "if " << (int)i << " " << (int)j << "    " << bodiesBefore.at(i).radiuses.at(j) << "
					// " <<
					//                                                              bodiesBefore.at(i).sinAlpha.at(j) <<
					//                                                              " " <<
					//                                                              bodiesBefore.at(i).cosAlpha.at(j) <<
					//                                                              std::endl;
				}
				else
				{
					double deltaX = bodiesBefore.at(j).pos.x - bodiesBefore.at(i).pos.x;
					double deltaY = bodiesBefore.at(j).pos.y - bodiesBefore.at(i).pos.y;
					// std::cout << "deltaX " << deltaX << " " << "DeltaY " << deltaY << std::endl;
					bodiesBefore.at(i).radiuses.at(j) = sqrt((deltaX) * (deltaX) + (deltaY) * (deltaY));
					bodiesBefore.at(i).sinAlpha.at(j) = deltaY / bodiesBefore.at(i).radiuses.at(j);
					bodiesBefore.at(i).cosAlpha.at(j) = deltaX / bodiesBefore.at(i).radiuses.at(j);
					// std::cout << "else " << (int)i << " " << (int)j << "    " << bodiesBefore.at(i).radiuses.at(j) <<
					// " " <<
					//                                                              bodiesBefore.at(i).sinAlpha.at(j) <<
					//                                                              " " <<
					//                                                              bodiesBefore.at(i).cosAlpha.at(j) <<
					//                                                              std::endl;
				}
			}
		}
	}

	void calculateForce()
	{
		// std::cout << " --------- calculateForce --------- " << std::endl;
		for (uint8_t i = 0; i < size; i++)
		{
			for (uint8_t j = 0; j < size; j++)
			{
				if (i == j)
				{
					bodiesBefore.at(i).forces.at(j).setZeros();
					// std::cout << "if " <<  (int)i << " " << (int)j << " " << bodiesBefore.at(i).forces.at(j).x << " "
					// << bodiesBefore.at(i).forces.at(j).y << " " << std::endl;
				}
				else
				{
					bodiesBefore.at(i).forces.at(j).x =
						bodiesBefore.at(i).cosAlpha.at(j) * bodiesBefore.at(j).mass * bodiesBefore.at(i).mass *
						gravConst / (bodiesBefore.at(i).radiuses.at(j) * bodiesBefore.at(i).radiuses.at(j));
					bodiesBefore.at(i).forces.at(j).y =
						bodiesBefore.at(i).sinAlpha.at(j) * bodiesBefore.at(j).mass * bodiesBefore.at(i).mass *
						gravConst / (bodiesBefore.at(i).radiuses.at(j) * bodiesBefore.at(i).radiuses.at(j));
					// std::cout << "else " << (int)i << " " << (int)j << " " << bodiesBefore.at(i).forces.at(j).x << "
					// " << bodiesBefore.at(i).forces.at(j).y << " " << std::endl;
				}
			}
		}
	}

	void calculateAfterPosition()
	{

		for (uint8_t i = 0; i < size; i++)
		{
			// std::cout << "bodyId " << (int)bodiesBefore.at(i).id << std::endl;
			Force sumForces{};
			for (auto force : bodiesBefore.at(i).forces)
			{
				sumForces.x += force.x;
				sumForces.y += force.y;
			}
			// std::cout << "sumForces " << sumForces.x << " " << sumForces.y << std::endl;

			Velocity afterVelocity{};
			afterVelocity.x = bodiesBefore.at(i).v.x + sumForces.x * dt / bodiesBefore.at(i).mass;
			afterVelocity.y = bodiesBefore.at(i).v.y + sumForces.y * dt / bodiesBefore.at(i).mass;
			// std::cout << "afterVelocity " << afterVelocity.x << " " << afterVelocity.y << std::endl;

			Position afterPosition{};
			afterPosition.x = bodiesBefore.at(i).pos.x + bodiesBefore.at(i).v.x * dt;
			afterPosition.y = bodiesBefore.at(i).pos.y + bodiesBefore.at(i).v.y * dt;
			// std::cout << "afterPosition " << afterPosition.x << " " << afterPosition.y << std::endl;

			bodiesAfter.at(i).v = afterVelocity;
			// std::cout << "bodiesAfter.at(i).v " << bodiesAfter.at(i).v.x  << " " << bodiesAfter.at(i).v.y  <<
			// std::endl;
			bodiesAfter.at(i).pos = afterPosition;
			// std::cout << "bodiesAfter.at(i).pos " << bodiesAfter.at(i).pos.x  << " " << bodiesAfter.at(i).pos.y  <<
			// std::endl;

			// saveToFile();
			// rewrite BodiesBefore and BodiesAfter
		}
		bodiesBefore = bodiesAfter;
		// std::cout << "bodiesBefore.at(i).pos after rewrite " << bodiesBefore.at(0).pos.x  << " " <<
		// bodiesBefore.at(0).pos.y  << std::endl;
		for (auto body : bodiesAfter)
		{
			body.pos.setZeros();
			body.v.setZeros();
			for (uint8_t i = 0; i < size; i++)
			{
				body.forces.at(i).setZeros();
				body.sinAlpha.at(i) = 0;
				body.cosAlpha.at(i) = 0;
				body.radiuses.at(i) = 0;
			}
		}
		// std::cout << "bodiesAfter.at(i).pos after rewrite " << bodiesAfter.at(0).pos.x  << " " <<
		// bodiesAfter.at(0).pos.y  << std::endl;
	}

	void doTimeIterations()
	{
		for (uint32_t t = 0; t < 100001; t++)
		{
			if (t % 250 == 0)
			{
				pushDataVector(t);
			}
			calculateRadiousesAndSinCosAlpha();
			calculateForce();
			calculateAfterPosition();
		}
		saveDataToFile();
	}

	void pushDataVector(uint32_t t)
	{
		// std::cout << " --------- pushDataVector --------- " << std::endl;
		// std::cout << "timeIteration: " << t << std::endl;
		std::array<Position, nBodies> positionsDataInOneTimeIteration{};
		for (uint8_t i = 0; i < size; i++)
		{
			positionsDataInOneTimeIteration.at(i).x = bodiesBefore.at(i).pos.x;
			positionsDataInOneTimeIteration.at(i).y = bodiesBefore.at(i).pos.y;
		}
		positionData.push_back(positionsDataInOneTimeIteration);
	}

	void saveDataToFile()
	{
		std::ofstream file;
		file.open("./data/positionData.txt");
		for (uint32_t i = 0; i < positionData.size(); i++)
		{
			for (auto position : positionData.at(i))
			{
				file << position.x << " " << position.y << " ";
			}
			file << std::endl;
		}
		file.close();
	}
};