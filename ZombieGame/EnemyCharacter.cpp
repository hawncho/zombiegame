/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#include "EnemyCharacter.h"
#include "Global.h"

#define ENEMY_WIDTH					128.0
#define ENEMY_HEIGHT				128.0

#define ENEMY_HITBOX_RADIUS			27.0

#define ENEMY_MAX_VELOCITY			3.0
#define ENEMY_FRICTION				0.0

#define ENEMY_TEXTURE				"./textures/zombie.png"

#define ENEMY_MAX_HITPOINTS			3

#define ENEMY_ANIMATION_DEATH		0

EnemyCharacter::EnemyCharacter()
{
	m_pCollisionObject->SetRadius(ENEMY_HITBOX_RADIUS);

	SetSize(ENEMY_WIDTH, ENEMY_HEIGHT);

	m_HasTarget = false;

	SetMaxVelocity(ENEMY_MAX_VELOCITY);
	SetFriction(ENEMY_FRICTION);
	SetSpriteTexture(ENEMY_TEXTURE, 4, 4);

	m_HitPoints = ENEMY_MAX_HITPOINTS;

	GenerateAnimationData();
}

EnemyCharacter::~EnemyCharacter()
{
	
}

void EnemyCharacter::GenerateAnimationData()
{
	std::vector<AnimationFrame> deathAnimation;
	deathAnimation.push_back(AnimationFrame(1));
	deathAnimation.push_back(AnimationFrame(2));
	deathAnimation.push_back(AnimationFrame(3));
	deathAnimation.push_back(AnimationFrame(4));
	deathAnimation.push_back(AnimationFrame(5, (int)TICK_RATE * 3));
	m_pSprite->AddAnimation(ENEMY_ANIMATION_DEATH, deathAnimation);
}

void EnemyCharacter::Update(int currentTick)
{
	if (m_HasTarget && !IsDead())
	{
		glm::dvec2 direction = m_TargetPoint - m_Position;
		direction = glm::normalize(direction);
		RotateTowardDirection(direction);

		SetVelocity(m_MaxVelocity * direction);

		if (glm::length(m_Position - m_TargetPoint) <= m_MaxVelocity / 2.0)
		{
			DisableTargetPoint();
		}
	}
	else
	{
		SetVelocity(0.0, 0.0);
	}

	if (IsDead() && !m_pSprite->IsAnimationPlaying())
	{
		SetActive(false);
	}

	MovableObject::Update(currentTick);
}

void EnemyCharacter::SetTargetPoint(double targetPointX, double targetPointY)
{
	m_TargetPoint.x = targetPointX;
	m_TargetPoint.y = targetPointY;

	m_HasTarget = true;
}

void EnemyCharacter::SetTargetPoint(glm::dvec2 targetPoint)
{
	SetTargetPoint(targetPoint.x, targetPoint.y);
}

void EnemyCharacter::DisableTargetPoint()
{
	m_HasTarget = false;
}

void EnemyCharacter::SetPosition(double xPos, double yPos)
{
	GameObject::SetPosition(xPos, yPos);

	glm::dvec2 center = GetCenter();
	m_pCollisionObject->CenterOnPoint(center.x, center.y);
}

void EnemyCharacter::SetActive(bool isActive)
{
	GameObject::SetActive(isActive);

	m_HitPoints = ENEMY_MAX_HITPOINTS;
	m_pSprite->SetFrameIndex(0);
}

void EnemyCharacter::Reset()
{
	TakeDamage(ENEMY_MAX_HITPOINTS);
	SetActive(false);
	m_pSprite->SetFrameIndex(0);
}

void EnemyCharacter::GetHit(Projectile* projectile)
{
	m_HitPoints -= projectile->GetDamageValue();
}

void EnemyCharacter::PlayDeathAnimation()
{
	m_pSprite->PlayAnimation(ENEMY_ANIMATION_DEATH);
}