/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#include <stdio.h>
#include <string>
#include <fstream>
#include <glm\gtc\type_ptr.hpp>

#include "ShaderBackend.h"

#define INVALID_UNIFORM_LOCATION 0xFFFFFFFF

ShaderBackend::ShaderBackend()
{
	m_ShaderProgram = 0;
}

ShaderBackend::~ShaderBackend()
{
	if (m_ShaderProgram != 0)
	{
		glDeleteProgram(m_ShaderProgram);
		m_ShaderProgram = 0;
	}
}

bool ShaderBackend::Init()
{
	// create the shader program object and check for errors
	m_ShaderProgram = glCreateProgram();

	if (m_ShaderProgram == 0)
	{
		fprintf(stderr, "Error creating shader program.\n");
		return false;
	}
	
	// attach the vertex shader to the shader program object
	if (!AddShader("shader.vert", GL_VERTEX_SHADER))
	{
		fprintf(stderr, "Failed to initialize vertex shader.\n");
		return false;
	}

	// attach the fragment shader to the shader program object
	if (!AddShader("shader.frag", GL_FRAGMENT_SHADER))
	{
		fprintf(stderr, "Failed to initialize fragment shader.\n");
		return false;
	}

	// link the shader program object
	glLinkProgram(m_ShaderProgram);

	// check for linking errors
	GLint success = 0;
	GLchar infoLog[1024] = { 0 };
	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
	if (success == 0)
	{
		glGetProgramInfoLog(m_ShaderProgram, sizeof(infoLog), 0, infoLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", infoLog);
		return false;
	}

	if (!InitUniformVariables())
	{
		return false;
	}

	return glGetError() == GL_NO_ERROR;
}

void ShaderBackend::Enable()
{
	glUseProgram(m_ShaderProgram);
}

bool ShaderBackend::AddShader(const char* shaderFilename, GLenum shaderType)
{
	// read in the shader file using an input filestream
	std::ifstream shaderFileStream(shaderFilename);
	std::string shaderSource((std::istreambuf_iterator<char>(shaderFileStream)), std::istreambuf_iterator<char>());

	// check if the shader file was successfully read
	if (shaderSource.length() == 0)
	{
		fprintf(stderr, "Failed to read shader file '%s' (empty string)\n", shaderFilename);
		return false;
	}

	// create the shader object
	GLuint shaderObject = 0;
	shaderObject = glCreateShader(shaderType);

	// check for errors creating the shader object
	if (shaderObject == 0)
	{
		fprintf(stderr, "Failed to create shader object for shader type: '%d'\n", shaderType);
		return false;
	}

	// attach the shader file source to the shader object
	const GLchar* source[1];
	source[0] = shaderSource.c_str();
	GLint length[1];
	length[0] = shaderSource.length();
	glShaderSource(shaderObject, 1, source, length);

	// compile the shader object and check for compilation errors
	glCompileShader(shaderObject);

	GLint success = 0;
	GLchar infoLog[1024] = { 0 };
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderObject, sizeof(infoLog), 0, infoLog);
		fprintf(stderr, "Error compiling shader '%d': '%s'\n", shaderType, infoLog);
		return false;
	}

	// attach the shader object to the shader program object
	glAttachShader(m_ShaderProgram, shaderObject);

	return true;
}

bool ShaderBackend::InitUniformVariables()
{
	m_WVPLocation = glGetUniformLocation(m_ShaderProgram, "WVP");
	m_WorldMatrixLocation = glGetUniformLocation(m_ShaderProgram, "World");
	m_SamplerLocation = glGetUniformLocation(m_ShaderProgram, "Sampler");
	m_PlayerPositionLocation = glGetUniformLocation(m_ShaderProgram, "PlayerPos");
	m_NumBulletsLocation = glGetUniformLocation(m_ShaderProgram, "NumBullets");
	m_BulletPosLocation = glGetUniformLocation(m_ShaderProgram, "BulletPos");
	m_NumExplosionsLocation = glGetUniformLocation(m_ShaderProgram, "NumExplosions");
	m_ExplosionDataLocation = glGetUniformLocation(m_ShaderProgram, "ExplosionData");
	m_NumPickupsLocation = glGetUniformLocation(m_ShaderProgram, "NumPickups");
	m_PickupDataLocation = glGetUniformLocation(m_ShaderProgram, "PickupData");
	m_EnableLightingLocation = glGetUniformLocation(m_ShaderProgram, "EnableLighting");
	m_FrameSizeLocation = glGetUniformLocation(m_ShaderProgram, "FrameSize");
	m_FramePosLocation = glGetUniformLocation(m_ShaderProgram, "FramePos");

	if (m_WVPLocation == INVALID_UNIFORM_LOCATION ||
		m_WorldMatrixLocation == INVALID_UNIFORM_LOCATION ||
		m_SamplerLocation == INVALID_UNIFORM_LOCATION ||
		m_PlayerPositionLocation == INVALID_UNIFORM_LOCATION ||
		m_NumBulletsLocation == INVALID_UNIFORM_LOCATION ||
		m_BulletPosLocation == INVALID_UNIFORM_LOCATION ||
		m_NumExplosionsLocation == INVALID_UNIFORM_LOCATION ||
		m_ExplosionDataLocation == INVALID_UNIFORM_LOCATION ||
		m_NumPickupsLocation == INVALID_UNIFORM_LOCATION ||
		m_PickupDataLocation == INVALID_UNIFORM_LOCATION ||
		m_EnableLightingLocation == INVALID_UNIFORM_LOCATION ||
		m_FrameSizeLocation == INVALID_UNIFORM_LOCATION || 
		m_FramePosLocation == INVALID_UNIFORM_LOCATION)
	{
		fprintf(stderr, "Failed to retrieve uniform variable location.\n");
		return false;
	}

	return true;
}

void ShaderBackend::SetCameraPos(glm::vec2 cameraPos)
{
	m_CameraPos = glm::vec3(cameraPos, 0.0f);
}

void ShaderBackend::SetViewProjectionMatrix(glm::mat4 viewProjectionMatrix)
{
	m_ViewProjectionMatrix = viewProjectionMatrix;
}

void ShaderBackend::SetPlayerPosition(glm::vec2 playerPosition)
{
	glUniform2f(m_PlayerPositionLocation, playerPosition.x, playerPosition.y);
}

void ShaderBackend::SetBulletPositions(std::vector<glm::vec2>& bulletPositions)
{
	int numBullets = bulletPositions.size();
	glUniform1i(m_NumBulletsLocation, numBullets);

	glUniform2fv(m_BulletPosLocation, numBullets, reinterpret_cast<GLfloat *>(bulletPositions.data()));
}

void ShaderBackend::SetExplosionData(std::vector<glm::vec3>& explosionData)
{
	int numExplosions = explosionData.size();
	glUniform1i(m_NumExplosionsLocation, numExplosions);

	glUniform3fv(m_ExplosionDataLocation, numExplosions, reinterpret_cast<GLfloat *>(explosionData.data()));
}

void ShaderBackend::SetPickupData(std::vector<glm::vec4>& pickupData)
{
	int numPickups = pickupData.size();
	glUniform1i(m_NumPickupsLocation, numPickups);

	glUniform4fv(m_PickupDataLocation, numPickups, reinterpret_cast<GLfloat *>(pickupData.data()));
}

void ShaderBackend::EnableLighting(bool enableLighting)
{
	glUniform1i(m_EnableLightingLocation, enableLighting);
}

void ShaderBackend::RenderHUDElement(IHUDElement* element)
{
	// not multitexturing, so only use one texture unit
	glUniform1i(m_SamplerLocation, 0);

	glUniformMatrix4fv(m_WVPLocation, 1, GL_FALSE, glm::value_ptr(m_ViewProjectionMatrix * glm::translate(m_CameraPos) * element->GetWorldMatrix()));
	glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4()));

	glUniform2f(m_FrameSizeLocation, 1.0, 1.0);
	glUniform2i(m_FramePosLocation, 0, 0);

	element->Draw();
}

void ShaderBackend::RenderGameLevel(GameLevel* gameLevel)
{
	// not multitexturing, so only use one texture unit
	glUniform1i(m_SamplerLocation, 0);

	glUniformMatrix4fv(m_WVPLocation, 1, GL_FALSE, glm::value_ptr(m_ViewProjectionMatrix));
	glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4()));

	glUniform2f(m_FrameSizeLocation, 1.0, 1.0);
	glUniform2i(m_FramePosLocation, 0, 0);

	gameLevel->Draw();
}

void ShaderBackend::RenderSprite(Sprite* sprite)
{
	glm::mat4 worldMatrix = sprite->GetModelMatrix();
	glm::mat4 WVP = m_ViewProjectionMatrix * worldMatrix;

	// not multitexturing, so only use one texture unit
	glUniform1i(m_SamplerLocation, 0);

	glUniformMatrix4fv(m_WVPLocation, 1, GL_FALSE, glm::value_ptr(WVP));
	glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_FALSE, glm::value_ptr(worldMatrix));

	glm::vec2 frameSize = sprite->GetFrameSize();
	glm::i32vec2 framePos = sprite->GetFramePos();
	glUniform2f(m_FrameSizeLocation, frameSize.x, frameSize.y);
	glUniform2i(m_FramePosLocation, framePos.x, framePos.y);

	sprite->Draw();
}