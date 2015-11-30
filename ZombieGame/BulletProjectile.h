/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#ifndef BULLETPROJECTILE_H_
#define BULLETPROJECTILE_H_

#include "Projectile.h"
#include "Collider.h"
#include "CollisionCircle.h"

class BulletProjectile : public Projectile, public Collider<CollisionCircle>
{
	public:
		BulletProjectile();
		~BulletProjectile();

		virtual void Update(int currentTick) override;
		virtual void SetPosition(double xPos, double yPos) override;

		void SetDirection(int xDirection, int yDirection);
		void SetDirection(glm::dvec2 direction);

	private:
		
};

#endif /* BULLETPROJECTILE_H_ */