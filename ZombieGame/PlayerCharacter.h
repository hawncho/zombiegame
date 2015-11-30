/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#ifndef PLAYERCHARACTER_H_
#define PLAYERCHARACTER_H_

#include "MovableObject.h"
#include "Collider.h"
#include "CollisionCircle.h"
#include "Damageable.h"
#include "ActionData.h"

#define NUM_ACTIONS						2

class PlayerCharacter : public MovableObject, public IDamageable, public Collider<CollisionCircle>
{
	public:
		int m_GrenadeCount;

		PlayerCharacter();

		void UpdateWithMouse(int currentTick, int mouseX, int mouseY);
		virtual void Update(int currentTick) override;
		virtual void SetPosition(double xPos, double yPos) override;
		virtual void SetActive(bool isActive) override;

		virtual void GetHit(Projectile* projectile) override;

		bool HandleKeyboardDown(unsigned char key, int x, int y);
		bool HandleKeyboardUp(unsigned char key, int x, int y);
		bool HandleMouse(int button, int state, int x, int y);
		bool HandleMovingMouse(int x, int y);
		bool HandlePassiveMouse(int x, int y);

		glm::dvec2 GetFacingDirection();

		bool IsShooting();
		bool IsThrowingGrenade();

		void PlayShootingAnimation();
		void PlayDeathAnimation();
		void PlayThrowAnimation();

		void AddGrenade();

		void Reset();

	private:
		int m_MouseX;
		int m_MouseY;

		ActionData m_Actions[NUM_ACTIONS];

		int m_CurrentAction;

		enum KeyState
		{
			UNPRESSED,
			PRESSED
		};

		struct
		{
			KeyState MoveUp = UNPRESSED;
			KeyState MoveDown = UNPRESSED;
			KeyState MoveLeft = UNPRESSED;
			KeyState MoveRight = UNPRESSED;

			KeyState Grenade = UNPRESSED;

			KeyState Mouse1 = UNPRESSED;
		}Keys;

		double CalculateAcceleration(KeyState negativeKey, KeyState positiveKey);
		void RotateTowardPoint(int x, int y);
		glm::dvec2 GetRelativeDirection(int x, int y);
		
		void UpdateMousePos(int mouseX, int mouseY);

		void GenerateAnimationData();
};

#endif /* PLAYERCHARACTER_H_ */