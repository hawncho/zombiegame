/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include "MovableObject.h"
#include "Expirable.h"
#include "CollisionCircle.h"

// In Damageable.h
class IDamageable;

class Projectile : public MovableObject, public Expirable
{
	public:
		Projectile();

		int GetDamageValue();

		virtual void Update(int currentTick) override;
		virtual void SetActive(bool isActive) override;

		void HitObject(IDamageable* objectHit);

	protected:
		int m_DamageValue;
};

#endif /* PROJECTILE_H_ */