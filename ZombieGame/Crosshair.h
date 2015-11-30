/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#ifndef CROSSHAIR_H_
#define CROSSHAIR_H_

#include <GL\glew.h>

#include "HUDElement.h"
#include "Texture.h"

class Crosshair : public IHUDElement
{
	public:
		Crosshair();

		~Crosshair();

		void Init();

		virtual void Draw() override;
		virtual glm::mat4 GetWorldMatrix() override;
		
		void CenterOnScreenPosition(glm::vec2 screenPosition);
		void CenterOnScreenPosition(float screenPositionX, float screenPositionY);

	private:
		Texture* m_pTexture;
		glm::vec2 m_Size;

		GLuint m_VBO;
};

#endif /* CROSSHAIR_H_ */