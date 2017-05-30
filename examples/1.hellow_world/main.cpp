#include <shaderboiler.h>
#include <iostream>

int main()
{
	using namespace sb;

	context ctx;
	vec3 AlbedoColor           = ctx.uniform<vec3>("AlbedoColor");
	vec3 AmbientLightColor     = ctx.uniform<vec3>("AmbientLightColor");
	vec3 DirectLightColor      = ctx.uniform<vec3>("DirectLightColor");
	vec3 LightPosition         = ctx.uniform<vec3>("LightPosition");

	vec3 normal   = ctx.in<vec3>("normal");
	vec3 position = ctx.in<vec3>("position");
	vec4& color   = ctx.out<vec4>("color");

	vec3 normalized_normal = normalize(normal);

	vec3 fragmentToLight = LightPosition - position;

	Float squaredDistance = dot(fragmentToLight, fragmentToLight);

	vec3 normalized_fragmentToLight = fragmentToLight / sqrt(squaredDistance);

	Float NdotL = dot(normal, normalized_fragmentToLight);

	vec3 DiffuseTerm = max(NdotL, 0.0) * DirectLightColor / squaredDistance;

	color = vec4(AlbedoColor * (AmbientLightColor + DiffuseTerm), 1.0);

	std::cout << ctx.genShader();

	return 0;
}
