/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#ifndef PICKUP_H_
#define PICKUP_H_

#include "GameObject.h"
#include "Expirable.h"
#include "PlayerCharacter.h"

class Pickup : public GameObject, public Expirable
{
	public:
		virtual void ActivateEffect(PlayerCharacter& player) = 0;

		virtual void Update(int currentTick) override
		{
			GameObject::Update(currentTick);

			UpdateExpirationTimer(currentTick);

			if (IsExpired(currentTick))
			{
				SetActive(false);
			}
		}

		virtual void SetActive(bool isActive) override
		{
			GameObject::SetActive(isActive);

			if (!isActive)
			{
				ResetExpirationTimer();
			}
		}

		glm::vec2 GetGlowColor()
		{
			return m_GlowColor;
		}

	protected:
		glm::vec2 m_GlowColor; // red & blue color components
};

#endif /* PICKUP_H_ */