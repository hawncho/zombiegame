/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#include "GrenadePickup.h"
#include "Global.h"

#define GRENADEPICKUP_WIDTH					64.0
#define GRENADEPICKUP_HEIGHT				64.0

#define GRENADEPICKUP_LIFESPAN_SECONDS		40

#define GRENADEPICKUP_TEXTURE				"./textures/grenade.png"
#define GRENADEPICKUP_TEXTURE_ROWS			2
#define GRENADEPICKUP_TEXTURE_COLUMNS		4

#define ANIMATION_SPIN						0

GrenadePickup::GrenadePickup()
{
	m_pCollisionObject->SetRadius(GRENADEPICKUP_WIDTH / 2.0);

	SetSize(GRENADEPICKUP_WIDTH, GRENADEPICKUP_HEIGHT);

	SetSpriteTexture(GRENADEPICKUP_TEXTURE, GRENADEPICKUP_TEXTURE_ROWS, GRENADEPICKUP_TEXTURE_COLUMNS);

	m_LifespanInTicks = (int)TICK_RATE * GRENADEPICKUP_LIFESPAN_SECONDS;

	m_GlowColor = glm::vec2(1.0f, 0.0f);

	GenerateAnimationData();
}

void GrenadePickup::GenerateAnimationData()
{
	std::vector<AnimationFrame> spinAnimation;
	spinAnimation.push_back(AnimationFrame(0, 8));
	spinAnimation.push_back(AnimationFrame(1, 8));
	spinAnimation.push_back(AnimationFrame(2, 8));
	spinAnimation.push_back(AnimationFrame(3, 8));
	spinAnimation.push_back(AnimationFrame(4, 8));
	spinAnimation.push_back(AnimationFrame(5, 8));
	spinAnimation.push_back(AnimationFrame(6, 8));
	spinAnimation.push_back(AnimationFrame(7, 8));
	m_pSprite->AddAnimation(ANIMATION_SPIN, spinAnimation);
}

void GrenadePickup::ActivateEffect(PlayerCharacter& player)
{
	player.AddGrenade();
}

void GrenadePickup::SetPosition(double xPos, double yPos)
{
	GameObject::SetPosition(xPos, yPos);

	glm::dvec2 center = GetCenter();
	m_pCollisionObject->CenterOnPoint(center.x, center.y);
}

void GrenadePickup::SetActive(bool isActive)
{
	Pickup::SetActive(isActive);

	m_pSprite->LoopAnimation(ANIMATION_SPIN);
}