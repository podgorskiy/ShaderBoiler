#include <shaderboiler.h>
#include <iostream>

void main()
{
	std::cout << "Test1" << "\n";

	sb::vec1 l(5.0);
	sb::vec2 l2 = sb::vec2(l);

	std::cout << sb::genShader(l2);

	std::cout << "Test1" << "\n";


	sb::vec2 a(sb::in);
	sb::vec2 b(sb::in);
	sb::vec2 d(sb::out);


	sb::vec2 g(1.0f, 2.0f);

	a = 3.0f * (a + b * 1.0f) * g;

	d = a * (a * 1.0f);

	std::cout << sb::genShader(d);
}
