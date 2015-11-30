/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#include "CollisionAABB.h"

#define LEFT		0
#define RIGHT		1
#define TOP			2
#define BOTTOM		3

CollisionAABB::CollisionAABB(double xPos, double yPos, double width, double height)
{
	SetPosition(xPos, yPos);
	SetDimensions(width, height);
}

void CollisionAABB::CenterOnPoint(double xPos, double yPos)
{
	SetPosition(xPos - (m_Width / 2.0), yPos - (m_Height / 2.0));
}

void CollisionAABB::SetPosition(double xPos, double yPos)
{
	m_PosX = xPos;
	m_PosY = yPos;
}

void CollisionAABB::SetDimensions(double width, double height)
{
	SetWidth(width);
	SetHeight(height);
}

void CollisionAABB::SetWidth(double width)
{
	m_Width = width;
}

void CollisionAABB::SetHeight(double height)
{
	m_Height = height;
}

glm::dvec2 CollisionAABB::CalculateNormal(double relativePositionX, double relativePositionY)
{
	bool onLeftSide = relativePositionX <= m_PosX;
	bool onRightSide = relativePositionX >= m_PosX + m_Width;
	bool onTopSide = relativePositionY <= m_PosY;
	bool onBottomSide = relativePositionY >= m_PosY + m_Height;

	// corners
	if (onLeftSide && onTopSide)
	{
		return glm::normalize(glm::dvec2(-1.0, -1.0));
	}
	else if (onLeftSide && onBottomSide)
	{
		return glm::normalize(glm::dvec2(-1.0, 1.0));
	}
	else if (onRightSide && onTopSide)
	{
		return glm::normalize(glm::dvec2(1.0, -1.0));
	}
	else if (onRightSide && onBottomSide)
	{
		return glm::normalize(glm::dvec2(1.0, 1.0));
	}

	// sides
	if (onLeftSide)
	{
		return glm::dvec2(-1.0, 0.0);
	}
	else if (onRightSide)
	{
		return glm::dvec2(1.0, 0.0);
	}
	else if (onTopSide)
	{
		return glm::dvec2(0.0, -1.0);
	}
	else if (onBottomSide)
	{
		return glm::dvec2(0.0, 1.0);
	}

	// if none of the above conditions are true,
	// the point is located inside the AABB
	// return the normal that is closest to the point
	const int numSides = 4;
	double distanceFrom[numSides];
	distanceFrom[LEFT] = relativePositionX - m_PosX;
	distanceFrom[RIGHT] = m_PosX + m_Width - relativePositionX;
	distanceFrom[TOP] = relativePositionY - m_PosY;
	distanceFrom[BOTTOM] = m_PosY + m_Height - relativePositionY;
	
	double shortestDistance = distanceFrom[LEFT];
	for (int side = RIGHT; side < numSides; ++side)
	{
		if (distanceFrom[side] < shortestDistance)
		{
			shortestDistance = distanceFrom[side];
		}
	}

	if (shortestDistance == distanceFrom[LEFT])
	{
		return glm::dvec2(-1.0, 0.0);
	}
	else if (shortestDistance == distanceFrom[RIGHT])
	{
		return glm::dvec2(1.0, 0.0);
	}
	else if (shortestDistance == distanceFrom[TOP])
	{
		return glm::dvec2(0.0, -1.0);
	}
	else
	{
		return glm::dvec2(0.0, 1.0);
	}
}

glm::dvec2 CollisionAABB::CalculateNormal(glm::dvec2 relativePosition)
{
	return CalculateNormal(relativePosition.x, relativePosition.y);
}

glm::dvec2 CollisionAABB::GetCenter()
{
	return glm::dvec2(m_PosX + (m_Width / 2.0), m_PosY + (m_Height / 2.0));
}