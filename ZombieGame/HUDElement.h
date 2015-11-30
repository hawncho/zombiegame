/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#ifndef HUDELEMENT_H_
#define HUDELEMENT_H_

#include <glm\gtx\transform2.hpp>

class IHUDElement
{
	public:
		virtual void Draw() = 0;

		virtual glm::mat4 GetWorldMatrix() = 0;

		void SetScreenPosition(glm::vec2 screenPosition)
		{
			m_ScreenPosition = screenPosition;
		}

		void SetScreenPosition(float screenPositionX, float screenPositionY)
		{
			SetScreenPosition(glm::vec2(screenPositionX, screenPositionY));
		}

	protected:
		glm::vec2 m_ScreenPosition;
};

#endif /* HUDELEMENT_H_ */