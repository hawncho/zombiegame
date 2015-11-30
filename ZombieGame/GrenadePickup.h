/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#ifndef GRENADEPICKUP_H_
#define GRENADEPICKUP_H_

#include "Pickup.h"
#include "Collider.h"
#include "CollisionCircle.h"

class GrenadePickup : public Pickup, public Collider<CollisionCircle>
{
	public:
		GrenadePickup();

		virtual void ActivateEffect(PlayerCharacter& player) override;

		virtual void SetPosition(double xPos, double yPos) override;

		virtual void SetActive(bool isActive) override;

	private:
		void GenerateAnimationData();
};

#endif /* GRENADEPICKUP_H_ */