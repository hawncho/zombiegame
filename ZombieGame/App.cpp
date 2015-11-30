/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#include <stdlib.h>
#include <stdio.h>
#include <vector>

#include "GLUTBackend.h"
#include "GLUTCallbacks.h"
#include "ShaderBackend.h"
#include "Sprite.h"
#include "Camera2D.h"
#include "MovableObject.h"
#include "Game.h"
#include "Global.h"

#define WINDOW_TITLE		"Zombie Game"

// Main application class
// Extends ICallbacks to be used for GLUT callback functions
class App : public ICallbacks
{
	public:
		~App();
		bool Init();

		virtual void DisplayCB() override;
		virtual void IdleCB() override;
		virtual void KeyboardCB(unsigned char key, int x, int y) override;
		virtual void KeyboardUpCB(unsigned char key, int x, int y) override;
		virtual void MouseCB(int button, int state, int x, int y) override;
		virtual void MovingMouseCB(int x, int y) override;
		virtual void PassiveMouseCB(int x, int y) override;

	private:
		Game* m_pGame;

		glm::vec2 m_MousePos;

		ShaderBackend* m_pShaderBackend;

		glm::mat4 m_ProjectionMatrix;

		Camera2D* m_pCamera;	

		int m_FPS;
		int m_FramesThisSecond;
		int m_TotalTime;
		int m_CurrentTime;

		void SetOGLOptions();
		void CalculateFPS();

		void UpdateMousePos(int x, int y);
		glm::vec2 ConvertWindowPosToWorldPos(glm::vec2 windowPos);
		glm::vec2 ConvertWindowPosToWorldPos(float windowPosX, float windowPosY);
};


App::~App()
{
	delete m_pCamera;
	m_pCamera = nullptr;

	delete m_pShaderBackend;
	m_pShaderBackend = nullptr;

	delete m_pGame;
	m_pGame = nullptr;
}

// Initializes that app
bool App::Init()
{
	m_FPS = 0;
	m_FramesThisSecond = 0;
	m_TotalTime = 0;
	m_CurrentTime = 0;

	m_pShaderBackend = new ShaderBackend();
	if (!m_pShaderBackend->Init())
	{
		fprintf(stderr, "Failed to initialize shaders.\n");
		return false;
	}

	m_pShaderBackend->Enable();

	m_ProjectionMatrix = glm::ortho(0.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, 0.0f);

	m_pCamera = new Camera2D((float)WINDOW_WIDTH, (float)WINDOW_HEIGHT);

	SetOGLOptions();

	m_pGame = new Game();
	if (!m_pGame->Init())
	{
		fprintf(stderr, "Failed to initialize Game.\n");
		return false;
	}

	StartGlutLoop();

	return true;
}

void App::CalculateFPS()
{
	m_TotalTime = glutGet(GLUT_ELAPSED_TIME);
	double timeSinceLastCalculation = (double)(m_TotalTime - m_CurrentTime);
	if (timeSinceLastCalculation >= 1000.0)
	{
		m_FPS = m_FramesThisSecond / (int)(timeSinceLastCalculation / 1000.0);
		m_CurrentTime = m_TotalTime;
		m_FramesThisSecond = 0;
		char titleWithFPS[64];
		sprintf_s(titleWithFPS, sizeof(titleWithFPS), "%s -- FPS: %d", WINDOW_TITLE, m_FPS);
		glutSetWindowTitle(titleWithFPS);
	}
}

glm::vec2 App::ConvertWindowPosToWorldPos(glm::vec2 windowPos)
{
	return ConvertWindowPosToWorldPos(windowPos.x, windowPos.y);
}

glm::vec2 App::ConvertWindowPosToWorldPos(float windowPosX, float windowPosY)
{
	glm::vec2 cameraPos = m_pCamera->GetPosition();

	return glm::vec2(windowPosX + cameraPos.x, windowPosY + cameraPos.y);
}

// Sets OpenGL options
void App::SetOGLOptions()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void App::DisplayCB()
{
	glClear(GL_COLOR_BUFFER_BIT);

	m_pShaderBackend->SetCameraPos(m_pCamera->GetPosition());
	m_pShaderBackend->SetViewProjectionMatrix(m_ProjectionMatrix * m_pCamera->GetViewMatrix());
	m_pShaderBackend->SetPlayerPosition(m_pGame->GetPlayerCenter());
	m_pShaderBackend->SetBulletPositions(m_pGame->GetBulletPositions());
	m_pShaderBackend->SetExplosionData(m_pGame->GetExplosionData());
	m_pShaderBackend->SetPickupData(m_pGame->GetPickupData());

	m_pShaderBackend->EnableLighting(true);
	m_pShaderBackend->RenderGameLevel(m_pGame->GetCurrentLevel());

	std::vector<Sprite*> sprites = m_pGame->GetSprites();
	for (std::vector<Sprite*>::iterator it = sprites.begin(); it != sprites.end(); ++it)
	{
		m_pShaderBackend->RenderSprite(*it);
	}

	m_pShaderBackend->EnableLighting(false);
	std::vector<IHUDElement*> elements = m_pGame->GetHUDElements();
	for (std::vector<IHUDElement*>::iterator it = elements.begin(); it != elements.end(); ++it)
	{
		m_pShaderBackend->RenderHUDElement(*it);
	}

	glutSwapBuffers();

	m_FramesThisSecond++;
}

void App::IdleCB()
{
	glm::vec2 worldMousePos = ConvertWindowPosToWorldPos(m_MousePos);

	m_pGame->MainGameLoop((int)worldMousePos.x, (int)worldMousePos.y);

	m_pCamera->CenterOn(m_pGame->GetPlayerCenter());

	CalculateFPS();

	glutPostRedisplay();
}

void App::KeyboardCB(unsigned char key, int x, int y)
{
	if (!m_pGame->HandleKeyboardDown(key, x, y))
	{
		switch (key)
		{
			case 27: // escape key
				exit(0);
				break;
			case 13: // enter key
				if (glutGetModifiers() == GLUT_ACTIVE_ALT)
				{
					ToggleFullScreen();
				}
				break;
		}
	}
}

void App::KeyboardUpCB(unsigned char key, int x, int y)
{
	if (!m_pGame->HandleKeyboardUp(key, x, y))
	{

	}
}

void App::MouseCB(int button, int state, int x, int y)
{
	UpdateMousePos(x, y);

	if (!m_pGame->HandleMouse(button, state, x, y))
	{

	}
}

void App::MovingMouseCB(int x, int y)
{
	UpdateMousePos(x, y);

	if (!m_pGame->HandleMovingMouse(x,y))
	{

	}
}

void App::PassiveMouseCB(int x, int y)
{
	UpdateMousePos(x, y);

	if (!m_pGame->HandlePassiveMouse(x, y))
	{

	}
}

void App::UpdateMousePos(int x, int y)
{
	m_MousePos.x = (float)x;
	m_MousePos.y = (float)y;
}

int main(int argc, char** argv)
{
	App app;

	// Create a GLUT window
	InitializeWindow(argc, argv, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, &app);

	// Initialize that app
	if (!app.Init())
	{
		fprintf(stderr, "Failed to initialize app.\n");
		system("pause");
		return 17;
	}

	return 0;
}
