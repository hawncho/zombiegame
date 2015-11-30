/**
* 
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm\gtx\transform.hpp>

class Camera2D
{
	public:
		Camera2D(float windowWidth, float windowHeight);

		glm::mat4 GetViewMatrix();

		void CenterOn(glm::vec2 pos);
		void CenterOn(float xPos, float yPos);

		void SetPos(glm::vec2 pos);
		void SetPos(float xPos, float yPos);
		void Move(float xDistance, float yDistance);
		void Rotate(float degrees);

		glm::vec2 GetPosition();

	private:
		float m_HalfWindowWidth;
		float m_HalfWindowHeight;
		glm::vec3 m_Position;

		glm::mat4 m_ViewMatrix;
};

#endif /* CAMERA_H_ */