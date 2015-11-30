/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#ifndef COLLISIONSYSTEM_H_
#define COLLISIONSYSTEM_H_

#include "CollisionCircle.h"
#include "CollisionAABB.h"

class CollisionSystem
{
	public:
		double CheckCollision(CollisionCircle* circle1, CollisionCircle* circle2);
		double CheckCollision(CollisionAABB* box1, CollisionAABB* box2);

		double CheckCollision(CollisionCircle* circle, CollisionAABB* box);
		double CheckCollision(CollisionAABB* box, CollisionCircle* circle);

	private:
		double CalculateDistanceBetweenPoints(double point1X, double point1Y, double point2X, double point2Y);
};

#endif /* COLLISIONSYSTEM_H_ */