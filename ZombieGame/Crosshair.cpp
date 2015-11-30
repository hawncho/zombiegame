/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#include <glm\gtx\transform.hpp>
#include <vector>

#include "Crosshair.h"
#include "Vertex.h"

#define CROSSHAIR_WIDTH			32.0f
#define CROSSHAIR_HEIGHT		32.0f
#define CROSSHAIR_TEXTURE		"./textures/crosshair.png"

Crosshair::Crosshair()
{
	Init();
}

Crosshair::~Crosshair()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void Crosshair::Init()
{
	m_pTexture = new Texture(CROSSHAIR_TEXTURE);
	m_pTexture->Init();

	std::vector<Vertex> vertices;
	vertices.push_back(Vertex(0.0f, 0.0f, 0.0f, 0.0f));
	vertices.push_back(Vertex(CROSSHAIR_WIDTH, 0.0f, 1.0f, 0.0f));
	vertices.push_back(Vertex(0.0f, CROSSHAIR_HEIGHT, 0.0f, 1.0f));

	vertices.push_back(Vertex(0.0f, CROSSHAIR_HEIGHT, 0.0f, 1.0f));
	vertices.push_back(Vertex(CROSSHAIR_WIDTH, 0.0f, 1.0f, 0.0f));
	vertices.push_back(Vertex(CROSSHAIR_WIDTH, CROSSHAIR_HEIGHT, 1.0f, 1.0f));

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

void Crosshair::Draw()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(sizeof(float) * 2));

	m_pTexture->Bind(GL_TEXTURE0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

glm::mat4 Crosshair::GetWorldMatrix()
{
	return glm::translate(glm::vec3(m_ScreenPosition.x, m_ScreenPosition.y, 0.0f));
}

void Crosshair::CenterOnScreenPosition(glm::vec2 screenPosition)
{
	CenterOnScreenPosition(screenPosition.x, screenPosition.y);
}

void Crosshair::CenterOnScreenPosition(float screenPositionX, float screenPositionY)
{
	screenPositionX -= (0.5f * CROSSHAIR_WIDTH);
	screenPositionY -= (0.5f * CROSSHAIR_HEIGHT);

	SetScreenPosition(screenPositionX, screenPositionY);
}