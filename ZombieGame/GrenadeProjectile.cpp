/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#include "Global.h"
#include "GrenadeProjectile.h"

#define GRENADE_TEXTURE							"./textures/grenade.png"
#define GRENADE_TEXTURE_ROWS					2
#define GRENADE_TEXTURE_COLUMNS					4

#define GRENADE_MAX_VELOCITY					10.0

#define GRENADE_WIDTH							48.0
#define GRENADE_HEIGHT							48.0

#define GRENADE_LIFESPAN_SECONDS				60

#define GRENADE_DAMAGE							10

GrenadeProjectile::GrenadeProjectile(double targetPointX, double targetPointY)
{
	SetMaxVelocity(GRENADE_MAX_VELOCITY);
	SetSize(GRENADE_WIDTH, GRENADE_HEIGHT);

	SetSpriteTexture(GRENADE_TEXTURE, GRENADE_TEXTURE_ROWS, GRENADE_TEXTURE_COLUMNS);

	m_LifespanInTicks = (int)TICK_RATE * GRENADE_LIFESPAN_SECONDS;

	m_DamageValue = GRENADE_DAMAGE;
	
	m_TargetPoint = glm::dvec2(targetPointX, targetPointY);
}

GrenadeProjectile::~GrenadeProjectile()
{

}

void GrenadeProjectile::Update(int currentTick)
{
	Projectile::Update(currentTick);

	SetVelocity(m_MaxVelocity * glm::normalize(m_TargetPoint - m_Position));

	AddRotation(10.0);

	if (HasReachedTarget())
	{
		SetActive(false);
	}
}

bool GrenadeProjectile::HasReachedTarget()
{
	return glm::length(m_TargetPoint - m_Position) <= m_MaxVelocity / 2.0;
}

void GrenadeProjectile::SetTargetPoint(int targetPointX, int targetPointY)
{
	SetTargetPoint(glm::dvec2(targetPointX, targetPointY));
}

void GrenadeProjectile::SetTargetPoint(glm::dvec2 targetPoint)
{
	m_TargetPoint = targetPoint;
}