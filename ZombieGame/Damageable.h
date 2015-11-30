/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#ifndef DAMAGEABLE_H_
#define DAMAGEABLE_H_

#include "Projectile.h"

class IDamageable
{
	public:
		virtual void GetHit(Projectile* projectile) = 0;

		void TakeDamage(int damage)
		{
			m_HitPoints -= damage;
		}

		bool IsDead()
		{
			return m_HitPoints <= 0;
		}

	protected:
		int m_HitPoints;
};

#endif /* DAMAGEABLE_H_ */