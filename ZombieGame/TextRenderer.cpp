/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#include <ctype.h>

#include "TextRenderer.h"

#define FONT_TEXTURE_NUM_ROWS			8
#define FONT_TEXTURE_NUM_COLUMNS		8

TextRenderer::TextRenderer()
{
	
}

TextRenderer::~TextRenderer()
{
	delete m_pFont;
	m_pFont = nullptr;
}

bool TextRenderer::Init()
{
	m_pFont = new Texture("textures/font.png");
	if (!m_pFont->Init())
	{
		return false;
	}

	m_CharacterToIndex.insert({ 
		{ ' ', 0 }, { '0', 1 }, { '1', 2 }, { '2', 3}, { '3', 4 }, { '4', 5 }, { '5', 6 }, { '6', 7 }, 
		{ '7', 8 }, { '8', 9 }, { '9', 10 }, { 'a', 11 }, { 'b', 12 }, { 'c', 13 }, { 'd', 14 }, { 'e', 15 }, 
		{ 'f', 16 }, { 'g', 17 }, { 'h', 18 }, { 'i', 19 }, { 'j', 20 }, { 'k', 21 }, { 'l', 22 }, { 'm', 23 },
		{ 'n', 24 }, { 'o', 25 }, { 'p', 26 }, { 'q', 27 }, { 'r', 28 }, { 's', 29 }, { 't', 30 }, { 'u', 31 },
		{ 'v', 32 }, { 'w', 33 }, { 'x', 34 }, { 'y', 35 }, { 'z', 36 }, { ':', 37 }, { '.', 38 }, { '!', 39 }
	});

	InitializeBufferObjects();

	return true;
}

void TextRenderer::AddText(const char* text, float xPos, float yPos, float size)
{
	m_TextList.push_back(TextInfo(std::string(text), glm::vec2(xPos, yPos), size));

	UpdateVertices();
}

void TextRenderer::UpdateText(const char* text, float xPos, float yPos, float size)
{
	m_TextList.clear();
	m_TextList.push_back(TextInfo(std::string(text), glm::vec2(xPos, yPos), size));

	UpdateVertices();
}

void TextRenderer::ClearAllText()
{
	m_TextList.clear();

	UpdateVertices();
}

void TextRenderer::UpdateVertices()
{
	m_Vertices.clear();

	for (std::vector<TextInfo>::iterator textInfoIter = m_TextList.begin(); textInfoIter != m_TextList.end(); ++textInfoIter)
	{
		TextInfo textInfo = *textInfoIter;

		std::string text = textInfo.m_Text;

		for (unsigned int j = 0; j < text.size(); ++j)
		{
			char character = text.at(j);
			character = tolower(character);

			if (m_CharacterToIndex.find(character) != m_CharacterToIndex.end())
			{
				int textureIndex = m_CharacterToIndex.at(character);

				glm::vec2 size(textInfo.m_Size);
				glm::vec2 topLeft = textInfo.m_Position;
				topLeft.x += (size.x * j);

				glm::vec2 topLeftTexCoord = CalculateTopLeftTexCoord(textureIndex);
				glm::vec2 sizeTexCoord(1.0f / (float)FONT_TEXTURE_NUM_COLUMNS, 1.0f / (float)FONT_TEXTURE_NUM_ROWS);

				m_Vertices.push_back(Vertex(topLeft,										topLeftTexCoord));
				m_Vertices.push_back(Vertex(topLeft.x + size.x,		topLeft.y,				topLeftTexCoord.x + sizeTexCoord.x,		topLeftTexCoord.y));
				m_Vertices.push_back(Vertex(topLeft.x,				topLeft.y + size.y,		topLeftTexCoord.x,						topLeftTexCoord.y + sizeTexCoord.y));

				m_Vertices.push_back(Vertex(topLeft.x,				topLeft.y + size.y,		topLeftTexCoord.x,						topLeftTexCoord.y + sizeTexCoord.y));
				m_Vertices.push_back(Vertex(topLeft.x + size.x,		topLeft.y,				topLeftTexCoord.x + sizeTexCoord.x,		topLeftTexCoord.y));
				m_Vertices.push_back(Vertex(topLeft + size,									topLeftTexCoord + sizeTexCoord));
			}
		}
	}

	PackBufferObjects();
}

void TextRenderer::InitializeBufferObjects()
{
	glGenBuffers(1, &m_VBO);
}

void TextRenderer::PackBufferObjects()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), m_Vertices.data(), GL_STATIC_DRAW);
}

void TextRenderer::Draw()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(sizeof(float) * 2));

	m_pFont->Bind(GL_TEXTURE0);

	glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

glm::vec2 TextRenderer::CalculateTopLeftTexCoord(int textureIndex)
{
	int textureColumn = textureIndex % FONT_TEXTURE_NUM_COLUMNS;
	int textureRow = textureIndex / FONT_TEXTURE_NUM_COLUMNS;

	return glm::vec2((float)textureColumn / (float)FONT_TEXTURE_NUM_COLUMNS, (float)textureRow / (float)FONT_TEXTURE_NUM_ROWS);
}

glm::mat4 TextRenderer::GetWorldMatrix()
{
	return glm::mat4();
}