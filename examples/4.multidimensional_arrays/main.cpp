#include <shaderboiler.h>
#include <iostream>

int main()
{
	using namespace sb;

	constexpr int lightCount = 4;

	context ctx;
	array<vec3, 3> array1d = ctx.uniform<array<vec3, 3> >("array1d");
	array<array<vec3, 3>, 3>& array2d = ctx.out<array<array<vec3, 3>, 3> >("array2d");

	array<array<array<vec3, 3>, 4>, 5> array3d;

	array3d[0][0][0] = array1d[0];
	array3d[0][0][2] = array1d[1];

	array3d[0][0][1] = (array3d[0][0][1] + array3d[0][0][2]) / 2.0;

	array3d[0][0][2] *= array3d[0][0][0];

	array3d[0][1][0] = log(array3d[0][0][2]);
	
	array2d[0][1] = array3d[0][1][0];

	std::cout << ctx.genShader();

	return 0;
}
