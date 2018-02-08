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
	float fontSize = 40.0f;
	float halfFontSize = fontSize / 2.0f;
	float startingPointX = halfWindowWidth * 0.2f;
	float startingPointY = halfWindowHeight + halfWindowHeight * 0.3f;

	for (int i = 0; i < TEXTARRAYLIMIT; ++i)
	{
		//Draw Downwards - u can write control string with this if you want or hard write using the "bottom 1"
		textObj[i] = Create::Text2DObject("text", Vector3(startingPointX, startingPointY - fontSize * i + halfFontSize, 1.0f), "control string here", Vector3(fontSize, fontSize, fontSize), Color(1.0f, 0.0f, 0.0f));
	}

	cout << "COptionsMenuState loaded\n" << endl;

	EditGraphics = false;
}
void COptionsMenuState::Update(double dt)
{
	if (KeyboardController::GetInstance()->IsKeyReleased(VK_BACK))
	{
		SceneManager::GetInstance()->SetActiveScene("MainMenuState");
	}
	static bool bPad1 = false;
	if (!bPad1 && KeyboardController::GetInstance()->IsKeyReleased(VK_NUMPAD1))
	{
		bPad1 = true;
		EditGraphics = true;
	}
	else if (bPad1 && !KeyboardController::GetInstance()->IsKeyReleased(VK_NUMPAD1))
		bPad1 = false;

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
	ss << "Press 1 to Change Graphic size";
	textObj[0]->SetText(ss.str());
	
	textObj[1]->SetText("test"); // 2nd
	textObj[2]->SetText("test"); // 3rd
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