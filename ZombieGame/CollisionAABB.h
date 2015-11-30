/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#ifndef COLLISIONAABB_H_
#define COLLISIONAABB_H_

#include <glm\gtx\transform.hpp>

class CollisionAABB
{
	public:
		double m_PosX;
		double m_PosY;
		double m_Width;
		double m_Height;

		CollisionAABB(double xPos = 0.0, double yPos = 0.0, double width = 0.0, double height = 0.0);

		void SetPosition(double xPos, double yPos);
		void SetDimensions(double width, double height);
		void SetWidth(double width);
		void SetHeight(double height);

		void CenterOnPoint(double xPos, double yPos);

		glm::dvec2 CalculateNormal(double relativePositionX, double relativePositionY);
		glm::dvec2 CalculateNormal(glm::dvec2 relativePosition);

		glm::dvec2 GetCenter();
};

#endif /* COLLISIONAABB_H_ */