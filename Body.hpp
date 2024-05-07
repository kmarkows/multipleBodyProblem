#include "./Force.hpp"
#include "./Position.hpp"
#include "./Velocity.hpp"
#include <array>

template <uint8_t nBodies> class Body
{

  public:
	Position pos{};
	Velocity v{};
	double mass{};
	uint8_t id{};
	uint8_t size{nBodies};

	std::array<Force, nBodies> forces{};
	std::array<double, nBodies> radiuses{};
	std::array<double, nBodies> sinAlpha{};
	std::array<double, nBodies> cosAlpha{};
};

// operator =