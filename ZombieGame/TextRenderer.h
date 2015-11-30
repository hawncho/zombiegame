/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#ifndef TEXTRENDERER_H_
#define TEXTRENDERER_H_

#include <GL\glew.h>
#include <unordered_map>
#include <vector>

#include "HUDElement.h"
#include "Texture.h"
#include "Vertex.h"

class TextRenderer : public IHUDElement
{
	public:
		TextRenderer();
		~TextRenderer();

		bool Init();

		void AddText(const char* text, float xPos = 0.0f, float yPos = 0.0f, float size = 32.0f);
		void UpdateText(const char* text, float xPos = 0.0f, float yPos = 0.0f, float size = 32.0f);
		void ClearAllText();

		virtual void Draw() override;
		virtual glm::mat4 GetWorldMatrix() override;

	private:
		struct TextInfo
		{
			std::string m_Text;
			glm::vec2 m_Position;
			float m_Size;

			TextInfo(std::string text, glm::vec2 position, float size)
			{
				m_Text = text;
				m_Position = position;
				m_Size = size;
			}
		};

		std::unordered_map<char, int> m_CharacterToIndex;

		std::vector<TextInfo> m_TextList;

		std::vector<Vertex> m_Vertices;

		GLuint m_VBO;

		Texture* m_pFont;

		void UpdateVertices();
		glm::vec2 CalculateTopLeftTexCoord(int textureIndex);

		void InitializeBufferObjects();
		void PackBufferObjects();
};

#endif /* TEXTRENDERER_H_ */