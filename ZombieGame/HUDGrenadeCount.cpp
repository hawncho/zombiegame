/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#include <GL\glew.h>
#include <vector>

#include "HUDGrenadeCount.h"
#include "Global.h"
#include "Vertex.h"

#define HUD_GRENADECOUNT_WIDTH					64.0f
#define HUD_GRENADECOUNT_HEIGHT					64.0f

#define HUD_GRENADECOUNT_ICON_COUNT				3

#define HUD_GRENADECOUNT_TEXTURE				"./textures/hud_grenade.png"
#define HUD_GRENADECOUNT_TEXTURE_ROWS			1
#define HUD_GRENADECOUNT_TEXTURE_COLUMNS		2

#define HUD_GRENADECOUNT_POSX					WINDOW_WIDTH - HUD_GRENADECOUNT_WIDTH
#define HUD_GRENADECOUNT_POSY					WINDOW_HEIGHT - HUD_GRENADECOUNT_HEIGHT

HUDGrenadeCount::HUDGrenadeCount()
{
	Init();
}

HUDGrenadeCount::~HUDGrenadeCount()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void HUDGrenadeCount::Init()
{
	m_pTexture = new Texture(HUD_GRENADECOUNT_TEXTURE);
	m_pTexture->Init();

	m_IconSize.x = HUD_GRENADECOUNT_WIDTH;
	m_IconSize.y = HUD_GRENADECOUNT_HEIGHT;

	m_ScreenPosition.x = HUD_GRENADECOUNT_POSX;
	m_ScreenPosition.y = HUD_GRENADECOUNT_POSY;

	glGenBuffers(1, &m_VBO);
	UpdateVertices();
}

void HUDGrenadeCount::UpdateVertices()
{
	std::vector<Vertex> vertices;

	for (int i = 0; i < HUD_GRENADECOUNT_ICON_COUNT; ++i)
	{
		float shiftAmount = -((float)i * m_IconSize.x);

		glm::vec2 topLeft(0.0f + shiftAmount, 0.0f);
		glm::vec2 topRight(m_IconSize.x + shiftAmount, 0.0f);
		glm::vec2 bottomLeft(0.0f + shiftAmount, m_IconSize.y);
		glm::vec2 bottomRight(m_IconSize.x + shiftAmount, m_IconSize.y);

		glm::vec2 texTopLeft(0.0f, 0.0f);
		glm::vec2 texTopRight(0.5f, 0.0f);
		glm::vec2 texBottomLeft(0.0f, 1.0f);
		glm::vec2 texBottomRight(0.5f, 1.0f);

		if (i >= m_NumGrenades)
		{
			texTopLeft.x += 0.5f;
			texTopRight.x += 0.5f;
			texBottomLeft.x += 0.5f;
			texBottomRight.x += 0.5f;
		}

		vertices.push_back(Vertex(topLeft, texTopLeft));
		vertices.push_back(Vertex(topRight, texTopRight));
		vertices.push_back(Vertex(bottomLeft, texBottomLeft));

		vertices.push_back(Vertex(bottomLeft, texBottomLeft));
		vertices.push_back(Vertex(topRight, texTopRight));
		vertices.push_back(Vertex(bottomRight, texBottomRight));
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
}

void HUDGrenadeCount::Draw()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(sizeof(float) * 2));

	m_pTexture->Bind(GL_TEXTURE0);

	glDrawArrays(GL_TRIANGLES, 0, HUD_GRENADECOUNT_ICON_COUNT * 6);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

glm::mat4 HUDGrenadeCount::GetWorldMatrix()
{
	return glm::translate(glm::vec3(m_ScreenPosition.x, m_ScreenPosition.y, 0.0f));
}

void HUDGrenadeCount::SetGrenadeCount(int count)
{
	m_NumGrenades = count;
	UpdateVertices();
}