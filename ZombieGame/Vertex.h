/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#ifndef VERTEX_H_
#define VERTEX_H_

#include <glm\vec2.hpp>

struct Vertex
{
	glm::vec2 Pos;
	glm::vec2 Tex;

	Vertex()
	{
		Pos.x = 0.0f;
		Pos.y = 0.0f;

		Tex.x = 0.0f;
		Tex.y = 0.0f;
	}

	Vertex(float xPos, float yPos)
	{
		Pos.x = xPos;
		Pos.y = yPos;

		Tex.x = 0.0f;
		Tex.y = 0.0f;
	}

	Vertex(float xPos, float yPos, float xTex, float yTex)
	{
		Pos.x = xPos;
		Pos.y = yPos;

		Tex.x = xTex;
		Tex.y = yTex;
	}

	Vertex(glm::vec2 position)
	{
		Pos.x = position.x;
		Pos.y = position.y;

		Tex.x = 0.0f;
		Tex.y = 0.0f;
	}

	Vertex(glm::vec2 position, glm::vec2 texCoord)
	{
		Pos.x = position.x;
		Pos.y = position.y;

		Tex.x = texCoord.x;
		Tex.y = texCoord.y;
	}
};

#endif /* VERTEX_H_ */