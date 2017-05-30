#include <shaderboiler.h>
#include <iostream>

int main()
{
	using namespace sb;

	constexpr int lightCount = 4;

	context ctx;
	vec3 AlbedoColor = ctx.uniform<vec3>("AlbedoColor");
	vec3 AmbientLightColor = ctx.uniform<vec3>("AmbientLightColor");
	array<vec3, lightCount> LightPositionArray = ctx.uniform<array<vec3, lightCount> >("LightPositions");
	array<vec3, lightCount> DirectLightColorArray = ctx.uniform<array<vec3, lightCount> >("DirectLightColors");

	vec3 normal = ctx.in<vec3>("normal");
	vec3 position = ctx.in<vec3>("position");

	vec3 accumulatedDiffuse = vec3(0.0);

	vec3 normalized_normal = normalize(normal);

	for (int i = 0; i < lightCount; ++i)
	{
		vec3 fragmentToLight = LightPositionArray[i] - position;

		Float squaredDistance = dot(fragmentToLight, fragmentToLight);

		vec3 normalized_fragmentToLight = fragmentToLight / sqrt(squaredDistance);

		Float NdotL = dot(normalized_normal, normalized_fragmentToLight);

		vec3 DiffuseTerm = max(NdotL, 0.0) * DirectLightColorArray[i] / squaredDistance;

		accumulatedDiffuse +=AlbedoColor * DiffuseTerm;
	}

	ctx.out<vec4>("color") = vec4(AlbedoColor * (AmbientLightColor + accumulatedDiffuse), 1.0);

	std::cout << ctx.genShader();

	return 0;
}
