#include <shaderboiler.h>
#include <iostream>

void main()
{
	using namespace sb;

	{
		std::cout << "\n" << "Test2" << "\n";

		context ctx;
		vec4 AmbientColor           = ctx.uniform<vec4>("AmbientColor");
		vec3 normal                 = ctx.in<vec3>("normal");
		vec3 vertex_to_light_vector = ctx.in<vec3>("vertex_to_light_vector");
		vec4 color                  = ctx.out<vec4>("color");

		// Defining The Material Colors
		const vec4 DiffuseColor = vec4(1.0, 0.0, 0.0, 1.0).SetName("DiffuseColor");

		// Scaling The Input Vector To Length 1
		//vec3 normalized_normal = normalize(normal);
		vec3 normalized_normal = normal;
		//vec3 normalized_vertex_to_light_vector = normalize(vertex_to_light_vector);
		vec3 normalized_vertex_to_light_vector = vertex_to_light_vector * 2;

		// Calculating The Diffuse Term And Clamping It To [0;1]
		//float DiffuseTerm = clamp(dot(normal, vertex_to_light_vector), 0.0, 1.0);
		Float DiffuseTerm = Float(normal * normalized_vertex_to_light_vector).SetName("DiffuseTerm");

		// Calculating The Final Color
		color += (AmbientColor + DiffuseColor * DiffuseTerm);

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
