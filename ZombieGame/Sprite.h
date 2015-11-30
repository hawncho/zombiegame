/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#ifndef SPRITE_H_
#define SPRITE_H_

#include <GL\glew.h>
#include <glm\gtx\transform.hpp>
#include <vector>

#include "Vertex.h"
#include "Texture.h"

struct AnimationFrame
{
	int m_FrameIndex;
	int m_FrameDurationInTicks;

	AnimationFrame(int frameIndex)
	{
		m_FrameIndex = frameIndex;
		m_FrameDurationInTicks = 4;
	}

	AnimationFrame(int frameIndex, int frameDurationInTicks)
	{
		m_FrameIndex = frameIndex;
		m_FrameDurationInTicks = frameDurationInTicks;
	}
};

class Sprite
{
	public:
		Sprite(const char* textureSheetFilename = "./textures/error.png", 
				int textureSheetNumRows = 1, 
				int textureSheetNumColumns = 1,
				float xPosition = 0.0f,
				float yPosition = 0.0f,
				float width = 1.0f,
				float height = 1.0f,
				float rotation = 0.0f);

		~Sprite();

		void SetPosition(float xPos, float yPos);
		void SetPosition(glm::vec2 position);

		void SetSize(float width, float height);
		void SetSize(glm::vec2 size);

		void SetScale(float xScale, float yScale);
		void SetScale(glm::vec2);

		void SetRotation(float rotationInDegrees);

		void SetTexture(const char* textureFilename, int textureSheetNumRows = 1, int textureSheetNumColumns = 1);

		void SetFrameIndex(int frameIndex);

		void AddAnimation(unsigned int animationIndex, std::vector<AnimationFrame> animation);

		void PlayAnimation(int animationIndex);
		void LoopAnimation(int animationIndex);
		void StopAnimation();

		bool IsAnimationPlaying();

		void Update(int currentTick);

		void Draw();

		glm::mat4 GetModelMatrix();
		glm::vec2 GetFrameSize();
		glm::i32vec2 GetFramePos();

	private:
		static const int m_NumVertices = 6;

		// position of the sprite in pixels
		glm::vec2 m_Position;
		// size of the sprite in pixels
		glm::vec2 m_Size;
		// scale factor of the sprite
		glm::vec2 m_Scale;
		// rotation of the sprite in degrees
		float m_Rotation;

		glm::mat4 m_ModelMatrix;

		// a quad with the full sprite texture mapped
		static Vertex m_Vertices[m_NumVertices];

		// vertex buffer object
		GLuint m_VBO;

		// sprite's texture
		Texture* m_pTexture;

		int m_TextureSheetNumColumns;
		glm::dvec2 m_FrameSize;
		int m_FrameIndex;

		std::vector<std::vector<AnimationFrame>> m_AnimationList;
		int m_CurrentAnimIndex;
		int m_CurrentAnimFrameIndex;
		int m_LastAnimationFrameTick;
		bool m_IsPlayingNewAnimation;
		bool m_IsLoopingAnimation;

		void InitializeVBO();
		void UpdateModelMatrix();
		void DeleteTexture();
		void UpdateSpriteFrameDimensions(int textureSheetNumRows, int textureSheetNumColumns);
};

#endif /* SPRITE_H_ */