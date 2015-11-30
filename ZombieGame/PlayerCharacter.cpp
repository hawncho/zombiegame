/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#include <ctype.h>

#include "PlayerCharacter.h"
#include "CollisionCircle.h"

#define MOUSE_DOWN						0
#define MOUSE_UP						1

#define MOUSE_LEFT_BUTTON				0
#define MOUSE_RIGHT_BUTTON				2

#define KEY_MOVE_UP						'w'
#define KEY_MOVE_DOWN					's'
#define KEY_MOVE_LEFT					'a'
#define KEY_MOVE_RIGHT					'd'
#define KEY_GRENADE0					'g'
#define KEY_GRENADE1					' '

#define PLAYER_WIDTH					128.0
#define PLAYER_HEIGHT					128.0

#define PLAYER_HITBOX_RADIUS			20.0

#define MAX_VELOCITY					4.0
#define FRICTION						0.4
#define ACCELERATION					2.0

#define PLAYER_TEXTURE					"./textures/guy.png"
#define PLAYER_TEXTURE_COLUMNS			4
#define PLAYER_TEXTURE_ROWS				4

#define ANIMATION_SHOOT					0
#define ANIMATION_DEATH					1
#define ANIMATION_THROW					2

#define ACTION_NONE						-1
#define ACTION_SHOOT					0
#define ACTION_GRENADE					1
#define ACTION_SHOOT_SPEED				48
#define ACTION_GRENADE_SPEED			24

#define GRENADE_START_COUNT				0
#define GRENADE_MAX_COUNT				3

PlayerCharacter::PlayerCharacter() : MovableObject()
{
	m_pCollisionObject->SetRadius(PLAYER_HITBOX_RADIUS);
	SetSize(PLAYER_WIDTH, PLAYER_HEIGHT);

	SetMaxVelocity(MAX_VELOCITY);
	SetFriction(FRICTION);
	SetSpriteTexture(PLAYER_TEXTURE, PLAYER_TEXTURE_ROWS, PLAYER_TEXTURE_COLUMNS);

	m_MouseX = 0;
	m_MouseY = 0;

	m_Actions[ACTION_SHOOT].SetActionSpeed(ACTION_SHOOT_SPEED);
	m_Actions[ACTION_GRENADE].SetActionSpeed(ACTION_GRENADE_SPEED);

	m_GrenadeCount = GRENADE_START_COUNT;

	m_HitPoints = 1;

	GenerateAnimationData();
}

void PlayerCharacter::GenerateAnimationData()
{
	std::vector<AnimationFrame> shootingAnimation;
	shootingAnimation.push_back(AnimationFrame(0, 10));
	shootingAnimation.push_back(AnimationFrame(1, 8));
	shootingAnimation.push_back(AnimationFrame(2, 8));
	shootingAnimation.push_back(AnimationFrame(3, 14));
	shootingAnimation.push_back(AnimationFrame(0, 0));
	m_pSprite->AddAnimation(ANIMATION_SHOOT, shootingAnimation);

	std::vector<AnimationFrame> deathAnimation;
	deathAnimation.push_back(AnimationFrame(4, 16));
	deathAnimation.push_back(AnimationFrame(5, 32));
	deathAnimation.push_back(AnimationFrame(6, 0));
	m_pSprite->AddAnimation(ANIMATION_DEATH, deathAnimation);

	std::vector<AnimationFrame> throwAnimation;
	throwAnimation.push_back(AnimationFrame(7, 4));
	throwAnimation.push_back(AnimationFrame(8, 4));
	throwAnimation.push_back(AnimationFrame(9, 16));
	throwAnimation.push_back(AnimationFrame(0, 0));
	m_pSprite->AddAnimation(ANIMATION_THROW, throwAnimation);
}

bool PlayerCharacter::HandleKeyboardDown(unsigned char keyPressed, int x, int y)
{
	keyPressed = tolower(keyPressed);

	switch (keyPressed)
	{
		case KEY_MOVE_UP:
			Keys.MoveUp = PRESSED;
			return true;
		case KEY_MOVE_DOWN:
			Keys.MoveDown = PRESSED;
			return true;
		case KEY_MOVE_LEFT:
			Keys.MoveLeft = PRESSED;
			return true;
		case KEY_MOVE_RIGHT:
			Keys.MoveRight = PRESSED;
			return true;
		case KEY_GRENADE0:
		case KEY_GRENADE1:
			Keys.Grenade = PRESSED;
			return true;
	}

	return false;
}

bool PlayerCharacter::HandleKeyboardUp(unsigned char keyPressed, int x, int y)
{
	keyPressed = tolower(keyPressed);

	switch (keyPressed)
	{
		case KEY_MOVE_UP:
			Keys.MoveUp = UNPRESSED;
			return true;
		case KEY_MOVE_DOWN:
			Keys.MoveDown = UNPRESSED;
			return true;
		case KEY_MOVE_LEFT:
			Keys.MoveLeft = UNPRESSED;
			return true;
		case KEY_MOVE_RIGHT:
			Keys.MoveRight = UNPRESSED;
			return true;
		case KEY_GRENADE0:
		case KEY_GRENADE1:
			Keys.Grenade = UNPRESSED;
			return true;
	}

	return false;
}

// only called on press & release of mouse button
bool PlayerCharacter::HandleMouse(int button, int state, int x, int y)
{
	if (state == MOUSE_DOWN)
	{
		switch (button)
		{
			case MOUSE_LEFT_BUTTON:
				Keys.Mouse1 = PRESSED;
				return true;
		}
	}
	else if (state == MOUSE_UP)
	{
		switch (button)
		{
			case MOUSE_LEFT_BUTTON:
				Keys.Mouse1 = UNPRESSED;
				return true;
		}
	}

	return false;
}

bool PlayerCharacter::HandleMovingMouse(int x, int y)
{
	return false;
}

bool PlayerCharacter::HandlePassiveMouse(int x, int y)
{
	return false;
}

void PlayerCharacter::UpdateMousePos(int mouseX, int mouseY)
{
	m_MouseX = mouseX;
	m_MouseY = mouseY;

	RotateTowardPoint(m_MouseX, m_MouseY);
}

void PlayerCharacter::UpdateWithMouse(int currentTick, int mouseX, int mouseY)
{
	if (IsActive())
	{
		UpdateMousePos(mouseX, mouseY);

		SetAcceleration(CalculateAcceleration(Keys.MoveLeft, Keys.MoveRight), CalculateAcceleration(Keys.MoveUp, Keys.MoveDown));

		if (m_CurrentAction == ACTION_NONE)
		{
			if (Keys.Grenade == PRESSED)
			{
				m_CurrentAction = ACTION_GRENADE;
			}
			else if (Keys.Mouse1 == PRESSED)
			{
				m_CurrentAction = ACTION_SHOOT;
			}

			m_Actions[m_CurrentAction].PerformAction(currentTick);
		}
		else if (m_Actions[m_CurrentAction].IsActionCompleted(currentTick))
		{
			m_CurrentAction = ACTION_NONE;
		}
		else
		{
			m_Actions[m_CurrentAction].UpdateAction(currentTick);
		}

		MovableObject::Update(currentTick);
	}
	else
	{
		GameObject::Update(currentTick);
	}
}

void PlayerCharacter::Update(int currentTick)
{
	UpdateWithMouse(currentTick, 0, 0);
}

void PlayerCharacter::SetPosition(double xPos, double yPos)
{
	GameObject::SetPosition(xPos, yPos);

	glm::dvec2 center = GetCenter();
	m_pCollisionObject->CenterOnPoint(center.x, center.y);
}

double PlayerCharacter::CalculateAcceleration(KeyState negativeKey, KeyState positiveKey)
{
	double accel = 0.0;

	if (negativeKey == PRESSED)
	{
		accel -= ACCELERATION;
	}

	if (positiveKey == PRESSED)
	{
		accel += ACCELERATION;
	}

	return accel;
}

glm::dvec2 PlayerCharacter::GetFacingDirection()
{
	return GetRelativeDirection(m_MouseX, m_MouseY);
}

glm::dvec2 PlayerCharacter::GetRelativeDirection(int x, int y)
{
	glm::dvec2 relativeDirection = glm::dvec2(x, y) - GetCenter();
	return glm::normalize(relativeDirection);
}

void PlayerCharacter::RotateTowardPoint(int x, int y)
{
	glm::dvec2 center = GetCenter();
	glm::dvec2 playerToPoint = GetRelativeDirection(x, y);

	double angleRadians = glm::acos(glm::dot(DOWN_VECTOR, playerToPoint));
	
	if (x < center.x)
	{
		angleRadians *= -1.0;
	}

	SetRotation(glm::degrees(angleRadians));
}

bool PlayerCharacter::IsShooting()
{
	return m_Actions[ACTION_SHOOT].IsPerformingAction();
}

bool PlayerCharacter::IsThrowingGrenade()
{
	return m_Actions[ACTION_GRENADE].IsPerformingAction();
}

void PlayerCharacter::PlayShootingAnimation()
{
	m_pSprite->PlayAnimation(ANIMATION_SHOOT);
}

void PlayerCharacter::PlayDeathAnimation()
{
	m_pSprite->PlayAnimation(ANIMATION_DEATH);
}

void PlayerCharacter::PlayThrowAnimation()
{
	m_pSprite->PlayAnimation(ANIMATION_THROW);
}

void PlayerCharacter::AddGrenade()
{
	if (m_GrenadeCount < GRENADE_MAX_COUNT)
	{
		m_GrenadeCount++;
	}
}

void PlayerCharacter::Reset()
{
	m_pSprite->StopAnimation();
	SetActive(true);
	m_HitPoints = 1; 
	m_GrenadeCount = GRENADE_START_COUNT;

	for (int actionIndex = 0; actionIndex < NUM_ACTIONS; ++actionIndex)
	{
		m_Actions[actionIndex].Reset();
	}
}

void PlayerCharacter::SetActive(bool isActive)
{
	GameObject::SetActive(isActive);

	if (!isActive)
	{
		for (int actionIndex = 0; actionIndex < NUM_ACTIONS; ++actionIndex)
		{
			m_Actions[actionIndex].Reset();
		}
	}
}

void PlayerCharacter::GetHit(Projectile* projectile)
{

}