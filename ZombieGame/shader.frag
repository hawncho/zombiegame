#version 450

const int MAX_BULLET_COUNT = 25;
const int MAX_EXPLOSION_COUNT = 3;
const int MAX_PICKUP_COUNT = 3;
const float PLAYER_LIGHT_RADIUS = 400.0f;
const float BULLET_LIGHT_RADIUS = 50.0f;
const float EXPLOSION_LIGHT_RADIUS = 1000.0f;
const float PICKUP_LIGHT_RADIUS = 300.0f;
const float LIGHT_INTENSITY_LIMIT = 2.0f;

in vec2 TexCoord0;
in vec2 WorldPos;

out vec4 FragColor;

uniform sampler2D Sampler;
uniform vec2 PlayerPos;
uniform int NumBullets;
uniform vec2 BulletPos[MAX_BULLET_COUNT];
uniform int NumExplosions;
uniform vec3 ExplosionData[MAX_EXPLOSION_COUNT];
uniform int NumPickups;
uniform vec4 PickupData[MAX_PICKUP_COUNT];
uniform bool EnableLighting;

vec4 CalculatePointLight(vec4 LightBase, vec2 position, float radius, float falloff)
{
	float Distance = length(WorldPos - position);

	radius = radius * falloff;

	float Attenuation = clamp(1.0f - (Distance * Distance) / (radius * radius), 0.0f, 1.0f);
	Attenuation *= Attenuation;

	return LightBase * Attenuation;
}

void main()
{
	vec4 LightBase = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec4 LightTotal = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	if (EnableLighting)
	{
		LightTotal = CalculatePointLight(LightBase, PlayerPos, PLAYER_LIGHT_RADIUS, 1.0f);

		for (int i = 0; i < NumBullets; ++i)
		{
			LightTotal += CalculatePointLight(LightBase, BulletPos[i], BULLET_LIGHT_RADIUS, 1.0f);
		}

		for (int i = 0; i < NumExplosions; ++i)
		{
			LightTotal += CalculatePointLight(LightBase, ExplosionData[i].xy, EXPLOSION_LIGHT_RADIUS, ExplosionData[i].z);
		}

		for (int i = 0; i < NumPickups; ++i)
		{
			LightTotal += CalculatePointLight(vec4(PickupData[i].z, 0.0f, PickupData[i].w, 1.0f), PickupData[i].xy, PICKUP_LIGHT_RADIUS, 1.0f);
		}
	}

	LightTotal = clamp(LightTotal, 0.0f, LIGHT_INTENSITY_LIMIT);

	FragColor = LightTotal * texture2D(Sampler, TexCoord0.xy);
}