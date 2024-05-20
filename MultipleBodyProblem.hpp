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
		for (uint8_t i = 0; i < size; i++)
		{
			for (uint8_t j = 0; j < size; j++)
			{
				if (j == i)
				{
					bodiesBefore.at(i).radiuses.at(j) = 0;
					bodiesBefore.at(i).sinAlpha.at(j) = 0;
					bodiesBefore.at(i).cosAlpha.at(j) = 0;
				}
				else
				{
					double deltaX = bodiesBefore.at(j).pos.x - bodiesBefore.at(i).pos.x;
					double deltaY = bodiesBefore.at(j).pos.y - bodiesBefore.at(i).pos.y;

					bodiesBefore.at(i).radiuses.at(j) = sqrt((deltaX) * (deltaX) + (deltaY) * (deltaY));
					bodiesBefore.at(i).sinAlpha.at(j) = deltaY / bodiesBefore.at(i).radiuses.at(j);
					bodiesBefore.at(i).cosAlpha.at(j) = deltaX / bodiesBefore.at(i).radiuses.at(j);
				}
			}
		}
	}

	void calculateForce()
	{
		for (uint8_t i = 0; i < size; i++)
		{
			for (uint8_t j = 0; j < size; j++)
			{
				if (i == j)
				{
					bodiesBefore.at(i).forces.at(j).setZeros();
				}
				else
				{
					bodiesBefore.at(i).forces.at(j).x =
						bodiesBefore.at(i).cosAlpha.at(j) * bodiesBefore.at(j).mass * bodiesBefore.at(i).mass *
						gravConst / (bodiesBefore.at(i).radiuses.at(j) * bodiesBefore.at(i).radiuses.at(j));
					bodiesBefore.at(i).forces.at(j).y =
						bodiesBefore.at(i).sinAlpha.at(j) * bodiesBefore.at(j).mass * bodiesBefore.at(i).mass *
						gravConst / (bodiesBefore.at(i).radiuses.at(j) * bodiesBefore.at(i).radiuses.at(j));
				}
			}
		}
	}

	void calculateAfterPosition()
	{

		for (uint8_t i = 0; i < size; i++)
		{
			Force sumForces{};
			for (auto force : bodiesBefore.at(i).forces)
			{
				sumForces.x += force.x;
				sumForces.y += force.y;
			}
			Velocity afterVelocity{};
			afterVelocity.x = bodiesBefore.at(i).v.x + sumForces.x * dt / bodiesBefore.at(i).mass;
			afterVelocity.y = bodiesBefore.at(i).v.y + sumForces.y * dt / bodiesBefore.at(i).mass;

			Position afterPosition{};
			afterPosition.x = bodiesBefore.at(i).pos.x + bodiesBefore.at(i).v.x * dt;
			afterPosition.y = bodiesBefore.at(i).pos.y + bodiesBefore.at(i).v.y * dt;

			bodiesAfter.at(i).v = afterVelocity;
			bodiesAfter.at(i).pos = afterPosition;
		}
		bodiesBefore = bodiesAfter;
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