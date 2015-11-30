/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#include "Camera2D.h"

// Creates a camera at the origin pointing toward the -Z axis ("forward" in a right-handed coordinate system)
// Stores the window dimensions for use in rotating the camera
Camera2D::Camera2D(float windowWidth, float windowHeight)
{
	m_HalfWindowWidth = windowWidth / 2.0f;
	m_HalfWindowHeight = windowHeight / 2.0f;

	m_Position = glm::vec3(0.0f, 0.0f, 0.0f);

	m_ViewMatrix = glm::lookAt(m_Position, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

// Returns the view matrix for the camera
glm::mat4 Camera2D::GetViewMatrix()
{
	return m_ViewMatrix;
}

void Camera2D::CenterOn(glm::vec2 pos)
{
	CenterOn(pos.x, pos.y);
}

void Camera2D::CenterOn(float xPos, float yPos)
{
	SetPos(xPos - m_HalfWindowWidth, yPos - m_HalfWindowHeight);
}

void Camera2D::SetPos(glm::vec2 pos)
{
	SetPos(pos.x, pos.y);
}

void Camera2D::SetPos(float xPos, float yPos)
{
	Move(xPos - m_Position.x, yPos - m_Position.y);
}

// Moves the camera in world space along the X and Y axis
void Camera2D::Move(float xDistance, float yDistance)
{
	m_Position.x += xDistance;
	m_Position.y += yDistance;

	m_ViewMatrix = glm::translate(m_ViewMatrix, glm::vec3(-xDistance, -yDistance, 0.0f));
}

// Rotates the camera about the center of the screen
void Camera2D::Rotate(float degrees)
{
	float translateToOriginX = m_HalfWindowWidth + m_Position.x;
	float translateToOriginY = m_HalfWindowHeight + m_Position.y;

	m_ViewMatrix = glm::translate(m_ViewMatrix, glm::vec3(translateToOriginX, translateToOriginY, 0.0f));
	m_ViewMatrix = glm::rotate(m_ViewMatrix, glm::radians(degrees), glm::vec3(0.0f, 0.0f, -1.0f));
	m_ViewMatrix = glm::translate(m_ViewMatrix, glm::vec3(-translateToOriginX, -translateToOriginY, 0.0f));
}

glm::vec2 Camera2D::GetPosition()
{
	return glm::vec2(m_Position);
}