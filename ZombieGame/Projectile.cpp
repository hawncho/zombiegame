/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#include "Global.h"
#include "Projectile.h"
#include "Damageable.h"

Projectile::Projectile() : MovableObject(), Expirable()
{
	m_DamageValue = 0;
}

void Projectile::Update(int currentTick)
{
	MovableObject::Update(currentTick);

	UpdateExpirationTimer(currentTick);

	if (IsExpired(currentTick))
	{
		SetActive(false);
	}
}

void Projectile::HitObject(IDamageable* objectHit)
{
	objectHit->GetHit(this);

	SetActive(false);
}

int Projectile::GetDamageValue()
{
	return m_DamageValue;
}

void Projectile::SetActive(bool isActive)
{
	GameObject::SetActive(isActive);

	if (!isActive)
	{
		ResetExpirationTimer();
	}
}
