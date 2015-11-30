/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#include "GameObject.h"

GameObject::GameObject()
{
	m_pSprite = new Sprite();

	m_IsActive = false;
}

GameObject::~GameObject()
{
	delete m_pSprite;
	m_pSprite = nullptr;
}

void GameObject::SetPosition(glm::dvec2 pos)
{
	SetPosition(pos.x, pos.y);
}

void GameObject::SetPosition(double xPos, double yPos)
{
	m_Position.x = xPos;
	m_Position.y = yPos;

	m_pSprite->SetPosition(m_Position);
}

void GameObject::TranslatePosition(double xDistance, double yDistance)
{
	SetPosition(m_Position.x + xDistance, m_Position.y + yDistance);
}

void GameObject::TranslatePosition(glm::dvec2 distance)
{
	TranslatePosition(distance.x, distance.y);
}

void GameObject::SetSize(double width, double height)
{
	m_Size.x = width;
	m_Size.y = height;
	m_pSprite->SetSize(m_Size);
}

void GameObject::SetRotation(double rotationInDegrees)
{
	m_Rotation = rotationInDegrees;
	m_pSprite->SetRotation((float)m_Rotation);
}

void GameObject::AddRotation(double rotationInDegrees)
{
	m_Rotation += rotationInDegrees;
	m_pSprite->SetRotation((float)m_Rotation);
}

void GameObject::SetSpriteTexture(const char* spriteTextureFilename, int textureSheetNumRows, int textureSheetNumColumns)
{
	m_pSprite->SetTexture(spriteTextureFilename, textureSheetNumRows, textureSheetNumColumns);
}

Sprite* GameObject::GetSprite()
{
	return m_pSprite;
}

glm::dvec2 GameObject::GetSize()
{
	return m_Size;
}

double GameObject::GetWidth()
{
	return m_Size.x;
}

double GameObject::GetHeight()
{
	return m_Size.y;
}

glm::dvec2 GameObject::GetCenter()
{
	return m_Position + (m_Size * glm::dvec2(0.5, 0.5));
}

void GameObject::CenterOnPosition(glm::dvec2 pos)
{
	CenterOnPosition(pos.x, pos.y);
}

void GameObject::CenterOnPosition(double xPos, double yPos)
{
	glm::dvec2 center = GetCenter();
	double xDistToTranslate = xPos - center.x;
	double yDistToTranslate = yPos - center.y;

	TranslatePosition(xDistToTranslate, yDistToTranslate);
}

void GameObject::RotateTowardDirection(glm::dvec2 direction)
{
	direction = glm::normalize(direction);

	double degreesInRadians = glm::acos(glm::dot(DOWN_VECTOR, direction));

	if (direction.x < 0.0)
	{
		degreesInRadians *= -1.0;
	}

	SetRotation(glm::degrees(degreesInRadians));
}

glm::dvec2 GameObject::GetPosition()
{
	return m_Position;
}

double GameObject::GetRotation()
{
	return m_Rotation;
}

bool GameObject::IsActive()
{
	return m_IsActive;
}

void GameObject::SetActive(bool isActive)
{
	m_IsActive = isActive;
}

void GameObject::Update(int currentTick)
{
	m_pSprite->Update(currentTick);
}
