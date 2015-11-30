/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#ifndef EXPLOSION_H_
#define EXPLOSION_H_

#include "GameObject.h"
#include "Collider.h"
#include "CollisionCircle.h"

class Explosion : public GameObject, public Collider<CollisionCircle> 
{
	public:
		Explosion();

		int GetDamageValue();
		void SetDamageValue(int damageValue);

		float GetLightFalloff();

		virtual void Update(int currentTick) override;
		virtual void SetActive(bool isActive) override;
		virtual void SetPosition(double xPos, double yPos) override;

	private:
		int m_DamageValue;

		int m_SpawnTick;

		float m_LightFalloff;

		void GenerateAnimationData();
};

#endif /* EXPLOSION_H_ */