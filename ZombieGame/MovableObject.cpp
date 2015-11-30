/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#include "MovableObject.h"

MovableObject::MovableObject() : GameObject()
{
	m_Velocity.x = 0.0;
	m_Velocity.y = 0.0;

	m_Acceleration.x = 0.0;
	m_Acceleration.y = 0.0;

	m_FrictionValue = 0.0;
	m_MaxVelocity = 0.1;
}

void MovableObject::Update(int currentTick)
{
	GameObject::Update(currentTick);

	double velocityMagnitude = glm::length(m_Velocity);
	glm::dvec2 velocityUnitVector = glm::normalize(m_Velocity);

	TranslatePosition(m_Velocity);

	CalculateAcceleration();

	CalculateFriction(velocityMagnitude, velocityUnitVector);

	if (velocityMagnitude > m_MaxVelocity)
	{
		m_Velocity = m_MaxVelocity * velocityUnitVector;
	}
}

void MovableObject::CollideWithWall(glm::dvec2 wallNormal, double penetrationDepth)
{
	TranslatePosition(penetrationDepth * wallNormal);

	if (wallNormal.x == 0.0)
	{
		m_Velocity.y = 0.0;
	}
	else if (wallNormal.y == 0)
	{
		m_Velocity.x = 0.0;
	}
}

void MovableObject::CalculateAcceleration()
{
	AddVelocity(m_Acceleration);
}

void MovableObject::CalculateFriction(double velocityMagnitude, glm::dvec2 velocityUnitVector)
{
	if (m_FrictionValue != 0.0)
	{
		if (velocityMagnitude > 0.0)
		{
			if (velocityMagnitude < m_FrictionValue)
			{
				m_Velocity.x = 0.0;
				m_Velocity.y = 0.0;
			}
			else
			{
				glm::dvec2 velocityLost = m_FrictionValue * velocityUnitVector;
				m_Velocity -= velocityLost;
			}
		}
	}
}

glm::dvec2 MovableObject::GetVelocity()
{
	return m_Velocity;
}

void MovableObject::SetVelocity(double xVelocity, double yVelocity)
{
	m_Velocity.x = xVelocity;
	m_Velocity.y = yVelocity;
}

void MovableObject::SetVelocity(glm::dvec2 velocity)
{
	SetVelocity(velocity.x,  velocity.y);
}

void MovableObject::AddVelocity(double xVelocity, double yVelocity)
{
	m_Velocity.x += xVelocity;
	m_Velocity.y += yVelocity;
}

void MovableObject::AddVelocity(glm::dvec2 velocity)
{
	AddVelocity(velocity.x, velocity.y);
}

void MovableObject::SetAcceleration(double xAcceleration, double yAcceleration)
{
	SetAccelerationX(xAcceleration);
	SetAccelerationY(yAcceleration);
}

void MovableObject::SetAcceleration(glm::dvec2 acceleration)
{
	SetAcceleration(acceleration.x, acceleration.y);
}

void MovableObject::SetAccelerationX(double xAcceleration)
{
	m_Acceleration.x = xAcceleration;
}

void MovableObject::SetAccelerationY(double yAcceleration)
{
	m_Acceleration.y = yAcceleration;
}

void MovableObject::SetFriction(double frictionValue)
{
	m_FrictionValue = frictionValue;
}

void MovableObject::SetMaxVelocity(double maxVelocity)
{
	m_MaxVelocity = maxVelocity;
}

bool MovableObject::IsMoving()
{
	return glm::length(m_Velocity) > 0.0;
}