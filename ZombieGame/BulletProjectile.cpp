/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#include "Global.h"
#include "BulletProjectile.h"
#include "CollisionCircle.h"

#include "Explosion.h"

#define BULLET_TEXTURE					"./textures/bullet.png"

#define BULLET_MAX_VELOCITY				40.0

#define BULLET_WIDTH					32.0
#define BULLET_HEIGHT					32.0

#define BULLET_HITBOX_RADIUS			2.0

#define BULLET_LIFESPAN_SECONDS			60

#define BULLET_DAMAGE					1

BulletProjectile::BulletProjectile() : Projectile()
{
	m_pCollisionObject->SetRadius(BULLET_HITBOX_RADIUS);

	SetMaxVelocity(BULLET_MAX_VELOCITY);
	SetSize(BULLET_WIDTH, BULLET_HEIGHT);

	SetSpriteTexture(BULLET_TEXTURE);

	m_LifespanInTicks = (int)TICK_RATE * BULLET_LIFESPAN_SECONDS;

	m_DamageValue = BULLET_DAMAGE;
}

BulletProjectile::~BulletProjectile()
{
	
}

void BulletProjectile::Update(int currentTick)
{
	Projectile::Update(currentTick);


}

void BulletProjectile::SetPosition(double xPos, double yPos)
{
	GameObject::SetPosition(xPos, yPos);

	glm::dvec2 center = GetCenter();
	m_pCollisionObject->CenterOnPoint(center.x, center.y);
}

void BulletProjectile::SetDirection(int xDirection, int yDirection)
{
	SetDirection(glm::dvec2(xDirection, yDirection));
}

void BulletProjectile::SetDirection(glm::dvec2 direction)
{
	direction = glm::normalize(direction);

	SetVelocity(m_MaxVelocity * direction);

	RotateTowardDirection(direction);
}
