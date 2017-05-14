#include <shaderboiler.h>
#include <iostream>

void main()
{
	sb::vec2 a(sb::in);
	sb::vec2 b(sb::in);
	sb::vec2 d(sb::out);

	sb::vec2 g(1.0f, 2.0f);

	a = 3.0f * (a + b * 1.0f) * g;

	d = a * (a * 1.0f);

	std::cout << sb::genShader(d);
}
