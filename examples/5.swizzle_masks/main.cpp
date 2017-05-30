#include <shaderboiler.h>
#include <iostream>

int main()
{
	using namespace sb;

	context ctx;
	vec4 input = ctx.uniform<vec4>("input");
	vec4& color = ctx.out<vec4>("color");

	vec3 xyz = input.xyz();

	xyz *= input.w();

	xyz.x() = input.r();

	vec4 other = xyz.zyzx();

	other = other.abgr();

	color = other.spqt().tqpt();

	color.x() = 1.0;

	std::cout << ctx.genShader();

	return 0;
}
