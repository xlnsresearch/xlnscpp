#include <iostream>
#define xlns16_ideal
#include <xlns/xlns16.h>

int main()
{
	float xf;
	xlns16_float x;

	std::cout << "xlns16 function test; enter 0 to quit\n";
	do {
	std::cin >> xf;
	x = xf;
	std::cout << "x=" << x << " (" << std::hex << xlns16_internal(x) << std::dec << ")\n";
	std::cout << " sin(x)=" << sin(x) << " (" << std::hex << xlns16_internal(sin(x)) << std::dec << ")\n";
	std::cout << " cos(x)=" << cos(x) << " (" << std::hex << xlns16_internal(cos(x)) << std::dec << ")\n";
	std::cout << " atan(x)=" << atan(x) << " (" << std::hex << xlns16_internal(atan(x)) << std::dec << ")\n";
	std::cout << " exp(x)=" << exp(x) << " (" << std::hex << xlns16_internal(exp(x)) << std::dec << ")\n";
	std::cout << " abs(x)=" << abs(x) << " (" << std::hex << xlns16_internal(abs(x)) << std::dec << ")\n";
	if (x>0)
		std::cout << " log(x)=" << log(x) << " (" << std::hex << xlns16_internal(log(x)) << std::dec << ")\n";
	if (x>0)
		std::cout << " sqrt(x)=" << sqrt(x) << " (" << std::hex << xlns16_internal(sqrt(x)) << std::dec << ")\n";
        } while (x!=0);
	return 0;
}

