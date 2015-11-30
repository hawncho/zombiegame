/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#ifndef GRENADEPROJECTILE_H_
#define GRENADEPROJECTILE_H_

#include "Projectile.h"

class GrenadeProjectile : public Projectile
{
	public:
		GrenadeProjectile(double targetPointX = 0.0, double targetPointY = 0.0);
		~GrenadeProjectile();

		virtual void Update(int currentTick) override;

		bool HasReachedTarget();

		void SetTargetPoint(int targetPointX, int targetPointY);
		void SetTargetPoint(glm::dvec2 targetPoint);

	private:
		glm::dvec2 m_TargetPoint;
};

#endif /* GRENADEPROJECTILE_H_ */