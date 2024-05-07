#include <cinttypes>

uint8_t factorial(uint8_t number)
{
	return (number == 0 or number == 1) ? 1 : number * factorial(number - 1);
}