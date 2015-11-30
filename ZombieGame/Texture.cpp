/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Texture.h"

Texture::Texture(const char* textureFilename)
{
	m_TextureFilename = textureFilename;

	m_TextureObject = 0;
}

Texture::~Texture()
{

}

bool Texture::Init()
{
	int components;
	unsigned char* textureData = stbi_load(m_TextureFilename, &m_TextureWidth, &m_TextureHeight, &components, STBI_rgb_alpha);

	// check if the texture was loaded successfully
	if (textureData == 0)
	{
		fprintf(stderr, "Failed to load texture '%s'\n", m_TextureFilename);
		return false;
	}

	glGenTextures(1, &m_TextureObject);
	glBindTexture(GL_TEXTURE_2D, m_TextureObject);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_TextureWidth, m_TextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

void Texture::Bind(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, m_TextureObject);
}

int Texture::GetTextureWidth()
{
	return m_TextureWidth;
}

int Texture::GetTextureHeight()
{
	return m_TextureHeight;
}