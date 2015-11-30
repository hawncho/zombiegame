/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#include "Sprite.h"

#define ANIMATION_NONE				-1

// a quad with the full sprite texture mapped
Vertex Sprite::m_Vertices[m_NumVertices] =
{
			// pos			// tex
	Vertex(0.0f, 0.0f,		0.0f, 0.0f),
	Vertex(1.0f, 0.0f,		1.0f, 0.0f),
	Vertex(0.0f, 1.0f,		0.0f, 1.0f),

	Vertex(0.0f, 1.0f,		0.0f, 1.0f),
	Vertex(1.0f, 0.0f,		1.0f, 0.0f),
	Vertex(1.0f, 1.0f,		1.0f, 1.0f)
};

Sprite::Sprite(const char* textureSheetFilename, int textureSheetNumRows, int textureSheetNumColumns, float xPosition, float yPosition, float width, float height, float rotation)
{
	SetPosition(xPosition, yPosition);

	SetSize(width, height);

	SetScale(1.0f, 1.0f);

	SetRotation(rotation);

	SetTexture(textureSheetFilename, textureSheetNumRows, textureSheetNumColumns);

	m_FrameIndex = 0;
	m_CurrentAnimIndex = ANIMATION_NONE;
	m_CurrentAnimFrameIndex = 0;
	m_LastAnimationFrameTick = 0;
	m_IsPlayingNewAnimation = false;

	InitializeVBO();
}

Sprite::~Sprite()
{
	DeleteTexture();
}

void Sprite::DeleteTexture()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

// Generate vertex buffer objects and store the quad vertex information in the buffer
void Sprite::InitializeVBO()
{
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_Vertices), m_Vertices, GL_STATIC_DRAW);
}

// Draw the sprite
void Sprite::Draw()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(sizeof(float) * 2));

	m_pTexture->Bind(GL_TEXTURE0);

	glDrawArrays(GL_TRIANGLES, 0, m_NumVertices);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Sprite::SetPosition(float xPos, float yPos)
{
	m_Position.x = xPos;
	m_Position.y = yPos;
}

void Sprite::SetPosition(glm::vec2 position)
{
	SetPosition(position.x, position.y);
}

void Sprite::SetSize(float width, float height)
{
	m_Size.x = width;
	m_Size.y = height;
}

void Sprite::SetSize(glm::vec2 size)
{
	SetSize(size.x, size.y);
}

void Sprite::SetScale(float xScale, float yScale)
{
	m_Scale.x = xScale;
	m_Scale.y = yScale;
}

void Sprite::SetScale(glm::vec2 scale)
{
	SetScale(scale.x, scale.y);
}

void Sprite::SetRotation(float rotationInDegrees)
{
	m_Rotation = glm::radians(rotationInDegrees);
}

void Sprite::SetTexture(const char* textureFilename, int textureSheetNumRows, int textureSheetNumColumns)
{
	DeleteTexture();

	m_pTexture = new Texture(textureFilename);
	m_pTexture->Init();

	UpdateSpriteFrameDimensions(textureSheetNumRows, textureSheetNumColumns);

	m_TextureSheetNumColumns = textureSheetNumColumns;
}

void Sprite::SetFrameIndex(int frameIndex)
{
	m_FrameIndex = frameIndex;
}

void Sprite::AddAnimation(unsigned int animationIndex, std::vector<AnimationFrame> animation)
{
	if (animationIndex >= m_AnimationList.capacity())
	{
		m_AnimationList.resize(animationIndex + 1);
	}

	m_AnimationList[animationIndex] = animation;
}

void Sprite::PlayAnimation(int animationIndex)
{
	m_CurrentAnimIndex = animationIndex;
	m_IsPlayingNewAnimation = true;

	m_IsLoopingAnimation = false;
}

void Sprite::LoopAnimation(int animationIndex)
{
	PlayAnimation(animationIndex);

	m_IsLoopingAnimation = true;
}

void Sprite::StopAnimation()
{
	m_CurrentAnimIndex = ANIMATION_NONE;
	m_IsPlayingNewAnimation = false;
	m_IsLoopingAnimation = false;
	m_FrameIndex = 0;
}

bool Sprite::IsAnimationPlaying()
{
	return m_CurrentAnimIndex != ANIMATION_NONE;
}

void Sprite::Update(int currentTick)
{
	if (m_CurrentAnimIndex != ANIMATION_NONE)
	{
		std::vector<AnimationFrame> animation = m_AnimationList[m_CurrentAnimIndex];
		if (m_IsPlayingNewAnimation)
		{
			m_IsPlayingNewAnimation = false;

			m_LastAnimationFrameTick = currentTick;

			m_CurrentAnimFrameIndex = 0;
			m_FrameIndex = animation[m_CurrentAnimFrameIndex].m_FrameIndex;
		}

		if (currentTick - m_LastAnimationFrameTick >=
			animation[m_CurrentAnimFrameIndex].m_FrameDurationInTicks)
		{
			m_CurrentAnimFrameIndex++;
			
			// if the animation has already gone past the last frame
			if ((unsigned int)m_CurrentAnimFrameIndex >= animation.size())
			{
				if (m_IsLoopingAnimation)
				{
					m_IsPlayingNewAnimation = true;
				}
				else
				{
					m_CurrentAnimIndex = ANIMATION_NONE;
				}
			}
			else
			{
				m_FrameIndex = animation[m_CurrentAnimFrameIndex].m_FrameIndex;
				m_LastAnimationFrameTick = currentTick;
			}
		}
	}
}

void Sprite::UpdateSpriteFrameDimensions(int textureSheetNumRows, int textureSheetNumColumns)
{
	float textureWidth = (float)m_pTexture->GetTextureWidth();
	float textureHeight = (float)m_pTexture->GetTextureHeight();

	m_FrameSize.x = textureWidth / (float)textureSheetNumColumns;
	m_FrameSize.y = textureHeight / (float)textureSheetNumRows;

	// normalize texture dimensionss
	m_FrameSize.x /= textureWidth;
	m_FrameSize.y /= textureHeight;
}

// Handles the model matrix containing rotation, translation, and scaling information
void Sprite::UpdateModelMatrix()
{
	m_ModelMatrix = glm::translate(glm::vec3(m_Position.x, m_Position.y, 0.0f));

	float scaledWidth = m_Size.x * m_Scale.x;
	float scaledHeight = m_Size.y * m_Scale.y;
	m_ModelMatrix = glm::translate(m_ModelMatrix, glm::vec3(scaledWidth/2.0f, scaledHeight/2.0f, 0.0f));
	m_ModelMatrix = glm::rotate(m_ModelMatrix, m_Rotation, glm::vec3(0.0f, 0.0f, -1.0f));
	m_ModelMatrix = glm::translate(m_ModelMatrix, glm::vec3(-scaledWidth/2.0f, -scaledHeight/2.0f, 0.0f));

	m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(scaledWidth, scaledHeight, 1.0f));
}

glm::mat4 Sprite::GetModelMatrix()
{
	UpdateModelMatrix();

	return m_ModelMatrix;
}

glm::vec2 Sprite::GetFrameSize()
{
	return m_FrameSize;
}

glm::i32vec2 Sprite::GetFramePos()
{
	return glm::i32vec2(m_FrameIndex % m_TextureSheetNumColumns, m_FrameIndex / m_TextureSheetNumColumns);
}