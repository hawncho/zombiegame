/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#ifndef COLLISIONCIRCLE_H_
#define COLLISIONCIRCLE_H_

#include <glm\gtx\transform.hpp>

class CollisionCircle
{
	public:
		double m_CenterPosX;
		double m_CenterPosY;
		double m_Radius;

		CollisionCircle(double radius = 0.0, double xCenter = 0.0, double yCenter = 0.0);

		void SetCenter(double xCenter, double yCenter);
		void SetRadius(double radius);
		
		glm::dvec2 CalculateNormal(double relativePositionX, double relativePositionY);
		glm::dvec2 CalculateNormal(glm::dvec2 relativePosition);

		void CenterOnPoint(double xPos, double yPos);
};

#endif /* COLLISIONCIRCLE_H_ */