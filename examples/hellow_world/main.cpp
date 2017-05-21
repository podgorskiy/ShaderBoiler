#include <shaderboiler.h>
#include <iostream>

void main()
{
	using namespace sb;
	using namespace sb::gl440;
	using namespace sb::fs;

	{
		context ctx;
		vec4 AmbientColor           = ctx.uniform<vec4>("AmbientColor");
		vec3 normal                 = ctx.in<vec3>("normal");
		vec3 vertex_to_light_vector = ctx.in<vec3>("vertex_to_light_vector");

		array<vec3> lights = ctx.uniform<array<vec3> >("lights[5]");

		array<vec3> lights2(3);

		lights2[0] = vec3(0.0);

		vec3 b = lights2[0];

		// Defining The Material Colors
		const vec4 DiffuseColor = vec4(1.0, 0.0, 0.0, 1.0).SetName("DiffuseColor");

		ivec1 a = gl_MaxProgramTexelOffset;

		// Scaling The Input Vector To Length 1
		//vec3 normalized_normal = normalize(normal);
		vec3 normalized_normal = normal;
		//vec3 normalized_vertex_to_light_vector = normalize(vertex_to_light_vector);
		vec3 normalized_vertex_to_light_vector = vertex_to_light_vector * 2;

		// Calculating The Diffuse Term And Clamping It To [0;1]
		Float DiffuseTerm = max(dot(normal, vertex_to_light_vector), 0.0).SetName("DiffuseTerm");

		// Calculating The Final Color
		ctx[fs::gl_FragColor] = AmbientColor + DiffuseColor * DiffuseTerm;

		std::cout << ctx.genShader();
	}
	std::cout << "Test3" << "\n";

	context ctx;
	vec2 a = ctx.in<vec2>("a");
	vec2 b = ctx.in<vec2>("b");
	vec2 d = ctx.out<vec2>("d");

	vec2 g(1.0f, 2.0f);

	a = 3.0f * (a + b * 1.0f) * g;

	d = a * (a * 1.0f);

	std::cout << ctx.genShader();
}
