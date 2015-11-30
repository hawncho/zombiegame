/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#ifndef HUDGRENADECOUNT_H_
#define HUDGRENADECOUNT_H_

#include "HUDElement.h"
#include "Texture.h"

class HUDGrenadeCount : public IHUDElement
{
	public:
		HUDGrenadeCount();

		~HUDGrenadeCount();

		void Init();

		void UpdateVertices();

		void SetGrenadeCount(int count);

		virtual void Draw() override;
		virtual glm::mat4 GetWorldMatrix() override;

	private:
		Texture* m_pTexture;
		glm::vec2 m_IconSize;

		GLuint m_VBO;

		int m_NumGrenades;
};

#endif /* HUDGRENADECOUNT_H_ */