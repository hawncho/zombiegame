/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#ifndef MOVABLEOBJECT_H_
#define MOVABLEOBJECT_H_

#include "GameObject.h"

class MovableObject : public GameObject
{
	public:
		MovableObject();

		virtual void Update(int currentTick) override;

		glm::dvec2 GetVelocity();

		void SetVelocity(double xVelocity, double yVelocity);
		void SetVelocity(glm::dvec2 velocity);

		void AddVelocity(double xVelocity, double yVelocity);
		void AddVelocity(glm::dvec2 velocity);

		void SetAcceleration(double xAcceleration, double yAcceleration);
		void SetAcceleration(glm::dvec2 acceleration);
		void SetAccelerationX(double xAcceleration);
		void SetAccelerationY(double yAcceleration);

		void SetFriction(double frictionValue);
		void SetMaxVelocity(double maxVelocity);

		void CollideWithWall(glm::dvec2 wallNormal, double penetrationDepth);

		bool IsMoving();

	protected:
		double m_MaxVelocity;

	private:
		glm::dvec2 m_Velocity;
		glm::dvec2 m_Acceleration;
		double m_FrictionValue;

		void CalculateAcceleration();
		void CalculateFriction(double velocityMagnitude, glm::dvec2 velocityUnitVector);
};

#endif /* MOVABLEOBJECT_H_ */