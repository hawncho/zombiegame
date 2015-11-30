/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#ifndef SHADERBACKEND_H_
#define SHADERBACKEND_H_

#include <GL\glew.h>

#include "Global.h"
#include "Sprite.h"
#include "GameLevel.h"
#include "TextRenderer.h"
#include "HUDElement.h"

class ShaderBackend
{
	public:
		ShaderBackend();
		~ShaderBackend();

		bool Init();

		void Enable();

		void RenderHUDElement(IHUDElement* textRenderer);
		void RenderGameLevel(GameLevel* gameLevel);
		void RenderSprite(Sprite* sprite);

		void SetCameraPos(glm::vec2 cameraPos);
		void SetViewProjectionMatrix(glm::mat4 viewProjectionMatrix);
		void SetPlayerPosition(glm::vec2 playerPosition);
		void SetBulletPositions(std::vector<glm::vec2>& bulletPositions);
		void SetExplosionData(std::vector<glm::vec3>& explosionData);
		void SetPickupData(std::vector<glm::vec4>& pickupData);
		void EnableLighting(bool enableLighting);

	private:
		GLuint m_ShaderProgram;

		GLuint m_WVPLocation;
		GLuint m_WorldMatrixLocation;
		GLuint m_SamplerLocation;
		GLuint m_PlayerPositionLocation;
		GLuint m_NumBulletsLocation;
		GLuint m_BulletPosLocation;
		GLuint m_NumExplosionsLocation;
		GLuint m_ExplosionDataLocation;
		GLuint m_NumPickupsLocation;
		GLuint m_PickupDataLocation;
		GLuint m_EnableLightingLocation;
		GLuint m_FrameSizeLocation;
		GLuint m_FramePosLocation;

		glm::mat4 m_ViewProjectionMatrix;
		glm::vec3 m_CameraPos;

		bool AddShader(const char* shaderFilename, GLenum shaderType);
		bool InitUniformVariables();
};

#endif /* SHADERBACKEND_H_ */