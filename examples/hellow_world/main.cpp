#include <shaderboiler.h>
#include <iostream>

void main()
{
	using namespace sb;

	{
		std::cout << "Test1" << "\n";

		vec3 a(1.0f);                               // initializes each component of the vec3 with the float
		vec4 b(ivec4(1, 2, 3, 4));                  // makes a vec4 with component-wise conversion
		vec2 c(1.0f, 2.0f);                         // initializes a vec2 with 2 floats
		ivec3 d(1, 2, 3);                           // initializes an ivec3 with 3 ints
		bvec4 e(1, 2, 2.0f, 3.0f);                  // uses 4 Boolean conversions
		vec2 f(vec4(1.0f, 2.0, 3.0f, 1.0f));        // drops the third component of a vec3
		vec3 g(vec4(1.0f, 2.0f, 3.0f, 4.0f));       // drops the fourth component of a vec4
		vec3 h(c, 2.0f);                            // vec3.x = vec2.x, vec3.y = vec2.y, vec3.z = float
		vec3 i(1.0f, c);                            // vec3.x = float, vec3.y = vec2.x, vec3.z = vec2.y
		vec4 j(h, 4.0f);
		vec4 k(1.0, i);
		vec4 l(c, f);

		std::cout << genShader(j + k + l + b + vec4(e));
	}

	{
		std::cout << "\n" << "Test2" << "\n";

		vec3 normal(in, "normal");
		vec4 color(out, "color");

		vec3 vertex_to_light_vector(in, "vertex_to_light_vector");

		// Defining The Material Colors
		vec4 AmbientColor = vec4(0.1, 0.0, 0.0, 1.0).SetName("AmbientColor");
		const vec4 DiffuseColor = vec4(1.0, 0.0, 0.0, 1.0).SetName("DiffuseColor");

		AmbientColor += DiffuseColor;

		// Scaling The Input Vector To Length 1
		//vec3 normalized_normal = normalize(normal);
		vec3 normalized_normal = normal;
		//vec3 normalized_vertex_to_light_vector = normalize(vertex_to_light_vector);
		vec3 normalized_vertex_to_light_vector = vertex_to_light_vector * 2;

		// Calculating The Diffuse Term And Clamping It To [0;1]
		//float DiffuseTerm = clamp(dot(normal, vertex_to_light_vector), 0.0, 1.0);
		Float DiffuseTerm = Float(normal * normalized_vertex_to_light_vector).SetName("DiffuseTerm");

		// Calculating The Final Color
		color = (AmbientColor + DiffuseColor * DiffuseTerm);

		std::cout << genShader(color);
	}
	std::cout << "Test3" << "\n";

	vec2 a(sb::in);
	vec2 b(sb::in);
	vec2 d(sb::out);

	vec2 g(1.0f, 2.0f);

	a = 3.0f * (a + b * 1.0f) * g;

	d = a * (a * 1.0f);

	std::cout << genShader(d);
}
