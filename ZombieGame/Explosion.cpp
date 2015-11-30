/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#include "Explosion.h"

#define EXPLOSION_RADIUS				128.0
#define EXPLOSION_DURATION_TICKS		40

#define ANIMATION_EXPLOSION				0

#define EXPLOSION_TEXTURE				"textures/explosion.png"
#define EXPLOSION_TEXTURE_COLUMNS		4
#define EXPLOSION_TEXTURE_ROWS			4

#define SPAWN_TICK_NOT_SET				-1

Explosion::Explosion()
{
	m_pCollisionObject->SetRadius(EXPLOSION_RADIUS);

	SetSize(EXPLOSION_RADIUS * 2.0 + 64.0, EXPLOSION_RADIUS * 2.0 + 64.0);
	SetSpriteTexture(EXPLOSION_TEXTURE, EXPLOSION_TEXTURE_COLUMNS, EXPLOSION_TEXTURE_ROWS);

	m_DamageValue = 1;

	m_SpawnTick = SPAWN_TICK_NOT_SET;

	GenerateAnimationData();
}

void Explosion::GenerateAnimationData()
{
	std::vector<AnimationFrame> explosionAnim;
	explosionAnim.push_back(AnimationFrame(0));
	explosionAnim.push_back(AnimationFrame(1));
	explosionAnim.push_back(AnimationFrame(2));
	explosionAnim.push_back(AnimationFrame(3));
	explosionAnim.push_back(AnimationFrame(4));
	explosionAnim.push_back(AnimationFrame(5));
	explosionAnim.push_back(AnimationFrame(6));
	explosionAnim.push_back(AnimationFrame(7));
	explosionAnim.push_back(AnimationFrame(8));
	explosionAnim.push_back(AnimationFrame(9));
	m_pSprite->AddAnimation(ANIMATION_EXPLOSION, explosionAnim);
}

void Explosion::Update(int currentTick)
{
	GameObject::Update(currentTick);

	if (m_SpawnTick == SPAWN_TICK_NOT_SET)
	{
		m_SpawnTick = currentTick;
	}

	m_LightFalloff = (float)(EXPLOSION_DURATION_TICKS - (currentTick - m_SpawnTick)) / (float)EXPLOSION_DURATION_TICKS;
	m_LightFalloff = sinf(m_LightFalloff) * 2.5f;

	if (!m_pSprite->IsAnimationPlaying())
	{
		SetActive(false);
	}
}

void Explosion::SetPosition(double xPos, double yPos)
{
	GameObject::SetPosition(xPos, yPos);

	glm::dvec2 center = GetCenter();
	m_pCollisionObject->CenterOnPoint(center.x, center.y);
}

void Explosion::SetActive(bool isActive)
{
	GameObject::SetActive(isActive);

	m_SpawnTick = SPAWN_TICK_NOT_SET;

	m_pSprite->PlayAnimation(ANIMATION_EXPLOSION);
}

float Explosion::GetLightFalloff()
{
	return m_LightFalloff;
}

int Explosion::GetDamageValue()
{
	return m_DamageValue;
}

void Explosion::SetDamageValue(int damageValue)
{
	m_DamageValue = damageValue;
}