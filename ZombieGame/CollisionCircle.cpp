/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#include "CollisionCircle.h"

CollisionCircle::CollisionCircle(double radius, double xCenter, double yCenter)
{
	SetRadius(radius);
	SetCenter(xCenter, yCenter);
}

void CollisionCircle::SetCenter(double xCenter, double yCenter)
{
	m_CenterPosX = xCenter;
	m_CenterPosY = yCenter;
}

void CollisionCircle::SetRadius(double radius)
{
	m_Radius = radius;
}

void CollisionCircle::CenterOnPoint(double xPos, double yPos)
{
	m_CenterPosX = xPos;
	m_CenterPosY = yPos;
}

glm::dvec2 CollisionCircle::CalculateNormal(double relativePositionX, double relativePositionY)
{
	return CalculateNormal(glm::dvec2(relativePositionX, relativePositionY));
}

glm::dvec2 CollisionCircle::CalculateNormal(glm::dvec2 relativePosition)
{
	glm::dvec2 center(m_CenterPosX, m_CenterPosY);

	return glm::normalize(relativePosition - center);
}