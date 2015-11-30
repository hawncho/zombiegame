/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <GL\glew.h>

class Texture
{
	public:
		Texture(const char* textureFilename);
		~Texture();

		bool Init();
		void Bind(GLenum textureUnit);

		int GetTextureWidth();
		int GetTextureHeight();

	private:
		GLuint m_TextureObject;
		
		const char* m_TextureFilename;

		int m_TextureWidth;
		int m_TextureHeight;
};

#endif /* TEXTURE_H_ */