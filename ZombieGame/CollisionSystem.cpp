/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#include <math.h>
#include <algorithm>

#include "CollisionSystem.h"

inline bool equal(double num1, double num2)
{
	// looked into using epsilon, but don't need that much precision
	if (num1 == num2 ||
		std::abs(num1 - num2) < 0.0001)
	{
		return true;
	}
	return false;
}

double CollisionSystem::CheckCollision(CollisionCircle* circle1, CollisionCircle* circle2)
{
	double totalDistance = CalculateDistanceBetweenPoints(circle1->m_CenterPosX, circle1->m_CenterPosY, 
														circle2->m_CenterPosX, circle2->m_CenterPosY);
	
	double combinedRadius = circle1->m_Radius + circle2->m_Radius;
	if (totalDistance < combinedRadius)
	{
		return combinedRadius - totalDistance;
	}

	return 0.0;
}

double CollisionSystem::CheckCollision(CollisionAABB* box1, CollisionAABB* box2)
{
	double box1Left = box1->m_PosX;
	double box1Top = box1->m_PosY;
	double box1Right = box1Left + box1->m_Width;
	double box1Bottom = box1Top + box1->m_Height;
	
	double box2Left = box2->m_PosX;
	double box2Top = box2->m_PosY;
	double box2Right = box2Left + box2->m_Width;
	double box2Bottom = box2Top + box2->m_Height;

	// does not consider the case where one box is fully enclosed in the other
	if (box1Left		<		box2Right		&&
		box1Right		>		box2Left		&&
		box1Top			<		box2Bottom		&&
		box1Bottom		>		box2Top)
	{
		double xPenetration;
		double yPenetration;

		double rightSidePenetration = box2Right - box1Left;
		double leftSidePenetration = box1Right - box2Left;
		double topSidePenetration = box1Bottom - box2Top;
		double bottomSidePenetration = box2Bottom - box1Top;

		xPenetration = (rightSidePenetration < leftSidePenetration) ? rightSidePenetration : leftSidePenetration;
		yPenetration = (topSidePenetration < bottomSidePenetration) ? topSidePenetration : bottomSidePenetration;

		if (equal(xPenetration, 0.0)) return xPenetration;
		if (equal(xPenetration, 0.0)) return yPenetration;

		return hypot(xPenetration, yPenetration);
	}

	return 0.0;
}

double CollisionSystem::CheckCollision(CollisionCircle* circle, CollisionAABB* box)
{
	double boxWidth = box->m_Width;
	double boxHeight = box->m_Height;

	double boxLeft = box->m_PosX;
	double boxTop = box->m_PosY;
	double boxRight = boxLeft + boxWidth;
	double boxBottom = boxTop + boxHeight;

	// determine the point on the AABB 
	// that is closest to the circle
	double closestPointToCircleX = circle->m_CenterPosX;
	double closestPointToCircleY = circle->m_CenterPosY;

	if (closestPointToCircleX > boxRight)
	{
		closestPointToCircleX = boxRight;
	}
	else if (closestPointToCircleX < boxLeft)
	{
		closestPointToCircleX = boxLeft;
	}

	if (closestPointToCircleY > boxBottom)
	{
		closestPointToCircleY = boxBottom;
	}
	else if (closestPointToCircleY < boxTop)
	{
		closestPointToCircleY = boxTop;
	}

	// calculate the distance from the center of the circle
	// and the closest point on the AABB to the circle
	double totalDistance = CalculateDistanceBetweenPoints(circle->m_CenterPosX, circle->m_CenterPosY, 
														closestPointToCircleX, closestPointToCircleY);

	double circleRadius = circle->m_Radius;
	double difference = circle->m_Radius - totalDistance;
	if (!equal(difference, 0.0) && difference > 0.0)
	{

		return circleRadius - totalDistance;
	}

	return 0.0;
}

double CollisionSystem::CheckCollision(CollisionAABB* box, CollisionCircle* circle)
{
	return CheckCollision(circle, box);
}

double CollisionSystem::CalculateDistanceBetweenPoints(double point1X, double point1Y, double point2X, double point2Y)
{
	double xDistance = point1X - point2X;
	double yDistance = point1Y - point2Y;
	return hypot(xDistance, yDistance);
}
