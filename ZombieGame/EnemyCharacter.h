/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#ifndef ENEMYCHARACTER_H_
#define ENEMYCHARACTER_H_

#include "MovableObject.h"
#include "Damageable.h"
#include "Collider.h"
#include "CollisionCircle.h"

class EnemyCharacter : public MovableObject, public IDamageable, public Collider<CollisionCircle>
{
	public:
		EnemyCharacter();
		~EnemyCharacter();

		virtual void Update(int currentTick) override;
		virtual void SetPosition(double xPos, double yPos) override;
		virtual void SetActive(bool isActive);

		virtual void GetHit(Projectile* projectile) override;

		void SetTargetPoint(double targetPointX, double targetPointY);
		void SetTargetPoint(glm::dvec2 targetPoint);
		void DisableTargetPoint();

		void PlayDeathAnimation();

		void Reset();

	private:
		bool m_HasTarget;
		glm::dvec2 m_TargetPoint;

		void GenerateAnimationData();
};

#endif /* ENEMYCHARACTER_H_ */