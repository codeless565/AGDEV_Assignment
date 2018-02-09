#include <iostream>
#include <sstream>
using namespace std;

#include "OptionsMenuState.h"
#include "GL\glew.h"
#include "../Application.h"
#include "LoadTGA.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"
#include "../TextEntity.h"
#include "RenderHelper.h"
#include "../SpriteEntity.h"
#include "../EntityManager.h"

#include "KeyboardController.h"
#include "SceneManager.h"
#include "../Lua/CLuaInterface.h"
#include "../Base/Source/ControlsManager.h"

COptionsMenuState::COptionsMenuState()
{
	//Will make new BG, u add in the change control using font
}
COptionsMenuState::~COptionsMenuState()
{

}

void COptionsMenuState::Init()
{
	// Create and attach the camera to the scene
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Get WindowSizing
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);

	MeshBuilder::GetInstance()->GenerateQuad("INTROSTATE_BKGROUND", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("INTROSTATE_BKGROUND")->textureID = LoadTGA("Image//OptionsMenu.tga");

	IntroStateBackground = Create::Sprite2DObject("INTROSTATE_BKGROUND",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(800.0f, 600.0f, 0.0f));

	// Write Text here
	float fontSize = 25.0f;
	float halfFontSize = fontSize / 2.0f;
	float startingPointX = halfWindowWidth * 0.2f;
	float startingPointY = halfWindowHeight + halfWindowHeight * 0.3f;

	for (int i = 0; i < TEXTARRAYLIMIT; ++i)
	{
		//Draw Downwards - u can write control string with this if you want or hard write using the "bottom 1"
		textObj[i] = Create::Text2DObject("text", Vector3(startingPointX, startingPointY - fontSize * i + halfFontSize, 1.0f), "", Vector3(fontSize, fontSize, fontSize), Color(1.0f, 0.0f, 0.0f));
	}

	cout << "COptionsMenuState loaded\n" << endl;

	//EditGraphics = false;
	//GraphicsChoices = 0;
	// Initialise the custom keyboard inputs
}
void COptionsMenuState::Update(double dt)
{
	if (KeyboardController::GetInstance()->IsKeyReleased(VK_BACK))
	{
		ControlsManager::GetInstance()->SaveOptionsState();
		ControlsManager::GetInstance()->ResetEditing();
		SceneManager::GetInstance()->SetActiveScene("MainMenuState");
	}
	
	static bool bPad1 = false;
	if (!bPad1 && KeyboardController::GetInstance()->IsKeyReleased(VK_F1))
	{
		bPad1 = true;
		ControlsManager::GetInstance()->setEditingForwardKey(true);
	}
	else if (bPad1 && !KeyboardController::GetInstance()->IsKeyReleased(VK_F1))
		bPad1 = false;

	static bool bPad2 = false;
	if (!bPad2 && KeyboardController::GetInstance()->IsKeyReleased(VK_F2))
	{
		bPad2 = true;
		ControlsManager::GetInstance()->setEditingBackwardKey(true);
	}
	else if (bPad2 && !KeyboardController::GetInstance()->IsKeyReleased(VK_F2))
		bPad2 = false;

	static bool bPad3 = false;
	if (!bPad3 && KeyboardController::GetInstance()->IsKeyReleased(VK_F3))
	{
		bPad3 = true;
		ControlsManager::GetInstance()->setEditingLeftKey(true);
	}
	else if (bPad3 && !KeyboardController::GetInstance()->IsKeyReleased(VK_F3))
		bPad3 = false;

	static bool bPad4 = false;
	if (!bPad4 && KeyboardController::GetInstance()->IsKeyReleased(VK_F4))
	{
		bPad4 = true;
		ControlsManager::GetInstance()->setEditingRightKey(true);
	}
	else if (bPad4 && !KeyboardController::GetInstance()->IsKeyReleased(VK_F4))
		bPad4 = false;

	static bool bPad5 = false;
	if (!bPad5 && KeyboardController::GetInstance()->IsKeyReleased(VK_F5))
	{
		bPad5 = true;
		ControlsManager::GetInstance()->setEditingOptionsKey(true);
	}
	else if (bPad5 && !KeyboardController::GetInstance()->IsKeyReleased(VK_F5))
		bPad5 = false;

	if (ControlsManager::GetInstance()->getEditingForwardKey())
	{
		if (KeyboardController::GetInstance()->IsKeyDown(VK_UP))
		{
			if (ControlsManager::GetInstance()->getCurrentChar() + 1 <= 90)
				ControlsManager::GetInstance()->addCurrentChar(1);
		}
		else if (KeyboardController::GetInstance()->IsKeyDown(VK_DOWN))
		{
			if (ControlsManager::GetInstance()->getCurrentChar() - 1 >= 65)
				ControlsManager::GetInstance()->addCurrentChar(-1);
		}
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
		{
			if (!ControlsManager::GetInstance()->InvalidKey())
			{
				ControlsManager::GetInstance()->setInvalidKeyPressed(false);
				ControlsManager::GetInstance()->setKeyMoveForward(ControlsManager::GetInstance()->getCurrentChar());
				ControlsManager::GetInstance()->SaveOptionsState();
				ControlsManager::GetInstance()->setEditingForwardKey(false);
			}
			else
				ControlsManager::GetInstance()->setInvalidKeyPressed(true);
		}
	}

	if (ControlsManager::GetInstance()->getEditingBackwardKey())
	{
		if (KeyboardController::GetInstance()->IsKeyDown(VK_UP))
		{
			if (ControlsManager::GetInstance()->getCurrentChar() + 1 <= 90)
				ControlsManager::GetInstance()->addCurrentChar(1);
		}
		else if (KeyboardController::GetInstance()->IsKeyDown(VK_DOWN))
		{
			if (ControlsManager::GetInstance()->getCurrentChar() - 1 >= 65)
				ControlsManager::GetInstance()->addCurrentChar(-1);
		}
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
		{
			if (!ControlsManager::GetInstance()->InvalidKey())
			{
				ControlsManager::GetInstance()->setInvalidKeyPressed(false);
				ControlsManager::GetInstance()->setKeyMoveBackward(ControlsManager::GetInstance()->getCurrentChar());
				ControlsManager::GetInstance()->SaveOptionsState();
				ControlsManager::GetInstance()->setEditingBackwardKey(false);
			}
			else
				ControlsManager::GetInstance()->setInvalidKeyPressed(true);
		}
	}
	if (ControlsManager::GetInstance()->getEditingLeftKey())
	{
		if (KeyboardController::GetInstance()->IsKeyDown(VK_UP))
		{
			if (ControlsManager::GetInstance()->getCurrentChar() + 1 <= 90)
				ControlsManager::GetInstance()->addCurrentChar(1);
		}
		else if (KeyboardController::GetInstance()->IsKeyDown(VK_DOWN))
		{
			if (ControlsManager::GetInstance()->getCurrentChar() - 1 >= 65)
				ControlsManager::GetInstance()->addCurrentChar(-1);
		}
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
		{
			if (!ControlsManager::GetInstance()->InvalidKey())
			{
				ControlsManager::GetInstance()->setInvalidKeyPressed(false);
				ControlsManager::GetInstance()->setKeyMoveLeft(ControlsManager::GetInstance()->getCurrentChar());
				ControlsManager::GetInstance()->SaveOptionsState();
				ControlsManager::GetInstance()->setEditingLeftKey(false);
			}
			else
				ControlsManager::GetInstance()->setInvalidKeyPressed(true);
		}
	}

	if (ControlsManager::GetInstance()->getEditingRightKey())
	{
		if (KeyboardController::GetInstance()->IsKeyDown(VK_UP))
		{
			if (ControlsManager::GetInstance()->getCurrentChar() + 1 <= 90)
				ControlsManager::GetInstance()->addCurrentChar(1);
		}
		else if (KeyboardController::GetInstance()->IsKeyDown(VK_DOWN))
		{
			if (ControlsManager::GetInstance()->getCurrentChar() - 1 >= 65)
				ControlsManager::GetInstance()->addCurrentChar(-1);
		}
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
		{
			if (!ControlsManager::GetInstance()->InvalidKey())
			{
				ControlsManager::GetInstance()->setInvalidKeyPressed(false);
				ControlsManager::GetInstance()->setKeyMoveRight(ControlsManager::GetInstance()->getCurrentChar());
				ControlsManager::GetInstance()->SaveOptionsState();
				ControlsManager::GetInstance()->setEditingRightKey(false);
			}
			else
				ControlsManager::GetInstance()->setInvalidKeyPressed(true);
		}
	}

	if (ControlsManager::GetInstance()->getEditingOptionsKey())
	{
		if (KeyboardController::GetInstance()->IsKeyDown(VK_UP))
		{
			if (ControlsManager::GetInstance()->getCurrentChar() + 1 <= 90)
				ControlsManager::GetInstance()->addCurrentChar(1);
		}
		else if (KeyboardController::GetInstance()->IsKeyDown(VK_DOWN))
		{
			if (ControlsManager::GetInstance()->getCurrentChar() - 1 >= 65)
				ControlsManager::GetInstance()->addCurrentChar(-1);
		}
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
		{
			if (!ControlsManager::GetInstance()->InvalidKey())
			{
				ControlsManager::GetInstance()->setInvalidKeyPressed(false);
				ControlsManager::GetInstance()->setKeyOptions(ControlsManager::GetInstance()->getCurrentChar());
				ControlsManager::GetInstance()->SaveOptionsState();
				ControlsManager::GetInstance()->setEditingOptionsKey(false);
			}
			else
				ControlsManager::GetInstance()->setInvalidKeyPressed(true);
		}
	}

	/*std::stringstream ss;
	if (EditGraphics)
	{
		ss.str("");
		ss << "Current Graphics " << windowWidth << " by " << windowHeight;
		textObj[6]->SetText(ss.str()); // 2nd

		if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
			GraphicsChoices++;
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			GraphicsChoices--;
		
		if (GraphicsChoices - 1 < 1)
			GraphicsChoices = 1;
		if (GraphicsChoices + 1 > 3)
			GraphicsChoices = 3;

		ss.str("");
		switch (GraphicsChoices)
		{
		case 1:
			ss << "800 x 600";
			break;
		case 2: 
			ss << "1280 x 600";
			break;
		case 3:
			ss << "1360 x 600";
			break;
		}
		textObj[7]->SetText(ss.str()); // 3rd

		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
		{
			if (GraphicsChoices == 1)
			{
				windowWidth = 800;
				windowHeight = 600;
				SaveOptionsState();
				EditGraphics = false;
			}
			else if (GraphicsChoices == 2)
			{
				windowWidth = 1280;
				windowHeight = 600;
				SaveOptionsState();
				EditGraphics = false;
			}
			else if (GraphicsChoices == 3)
			{
				windowWidth = 1360;
				windowHeight = 600;
				SaveOptionsState();
				EditGraphics = false;
			}
			Application::GetInstance().setWindowHeight(windowHeight);
			Application::GetInstance().setWindowWidth(windowWidth);
		}
	}*/
}
void COptionsMenuState::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//GraphicsManager::GetInstance()->UpdateLightUniforms();

	// Setup 3D pipeline then render 3D
	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Render the required entities
	EntityManager::GetInstance()->Render();

	// Setup 2D pipeline then render 2D
	GraphicsManager::GetInstance()->SetOrthographicProjection(0,
		Application::GetInstance().GetWindowWidth(),
		0,
		Application::GetInstance().GetWindowHeight(),
		-10, 10);
	GraphicsManager::GetInstance()->DetachCamera();

	// Render the required entities
	EntityManager::GetInstance()->RenderUI();

	// >>>>> "bottom 1" is here
	std::stringstream ss;
	
	ss.str("");
	ss << "[F1] Move Forward " << ControlsManager::GetInstance()->getkeyMoveForward();
	textObj[0]->SetText(ss.str());
	

	ss.str("");
	ss << "[F2] Move Backward" << ControlsManager::GetInstance()->getkeyMoveBackward();
	textObj[1]->SetText(ss.str());

	ss.str("");
	ss << "[F3] Move Left    " << ControlsManager::GetInstance()->getkeyMoveLeft();
	textObj[2]->SetText(ss.str());

	ss.str("");
	ss << "[F4] Move Right   " << ControlsManager::GetInstance()->getkeyMoveRight();
	textObj[3]->SetText(ss.str());

	ss.str("");
	ss << "[F5] Options      " << ControlsManager::GetInstance()->getkeyOptions();
	textObj[4]->SetText(ss.str());


	char PrevChar = ControlsManager::GetInstance()->getCurrentChar() - 1;
	char NextChar = ControlsManager::GetInstance()->getCurrentChar() + 1;
	if (PrevChar < 65)
		PrevChar = ' ';
	if (NextChar > 90)
		NextChar = ' ';

	ss.str("");
	if (ControlsManager::GetInstance()->getEditingForwardKey())
		ss << "Edit forward key to:" << PrevChar << " " << ControlsManager::GetInstance()->getCurrentChar() << " " << NextChar;
	else if (ControlsManager::GetInstance()->getEditingBackwardKey())
		ss << "Edit backward key to:" << PrevChar << " " << ControlsManager::GetInstance()->getCurrentChar() << " " << NextChar;
	else if (ControlsManager::GetInstance()->getEditingLeftKey())
		ss << "Edit left key to:" << PrevChar << " " << ControlsManager::GetInstance()->getCurrentChar() << " " << NextChar;
	else if (ControlsManager::GetInstance()->getEditingRightKey())
		ss << "Edit right key to:" << PrevChar << " " << ControlsManager::GetInstance()->getCurrentChar() << " " << NextChar;
	else if (ControlsManager::GetInstance()->getEditingOptionsKey())
		ss << "Edit options key to:" << PrevChar << " " << ControlsManager::GetInstance()->getCurrentChar() << " " << NextChar;
	textObj[9]->SetText(ss.str());

	ss.str("");
	if (ControlsManager::GetInstance()->InvalidKeyPressed)
		ss << "Key bind already exist";
	else
		ss.str("");
	textObj[8]->SetText(ss.str());

}
void COptionsMenuState::Exit()
{
	// Remove the entity from EntityManager
	EntityManager::GetInstance()->RemoveEntity(IntroStateBackground);
	EntityManager::GetInstance()->Cleanthis_ForExit();

	// Remove the meshes which are specific to COptionsMenuState
	MeshBuilder::GetInstance()->RemoveMesh("INTROSTATE_BKGROUND");

	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
}