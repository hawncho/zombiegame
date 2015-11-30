/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include <glm\vec2.hpp>

#include "Sprite.h"

class GameObject
{
	public:
		GameObject();
		~GameObject();

		void SetPosition(glm::dvec2 pos);
		virtual void SetPosition(double xPos, double yPos);
		void SetSize(double width, double height);
		void SetRotation(double rotationInDegrees);
		void AddRotation(double rotationInDegrees);
		void SetSpriteTexture(const char* spriteTextureFilename, int textureSheetNumRows = 1, int textureSheetNumColumns = 1);

		void TranslatePosition(double xDistance, double yDistance);
		void TranslatePosition(glm::dvec2 distance);

		glm::dvec2 GetPosition();
		double GetRotation();

		virtual void Update(int currentTick);

		Sprite* GetSprite();

		glm::dvec2 GetSize();
		double GetWidth();
		double GetHeight();

		glm::dvec2 GetCenter();
		void CenterOnPosition(glm::dvec2 pos);
		void CenterOnPosition(double xPos, double yPos);

		void RotateTowardDirection(glm::dvec2 direction);

		bool IsActive();
		virtual void SetActive(bool isActive);

	protected:
		const glm::dvec2 DOWN_VECTOR = glm::dvec2(0.0, 1.0);

		glm::dvec2 m_Position;
		glm::dvec2 m_Size;
		double m_Rotation;

		Sprite* m_pSprite;

	private:
		bool m_IsActive;
};

#endif /* GAMEOBJECT_H_ */