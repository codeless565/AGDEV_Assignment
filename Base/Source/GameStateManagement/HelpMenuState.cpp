#include <iostream>
using namespace std;

#include "HelpMenuState.h"
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

CHelpMenuState::CHelpMenuState()
{
	// make new bg for this, make do with it and add in the control using fonts and all
}
CHelpMenuState::~CHelpMenuState()
{

}

void CHelpMenuState::Init()
{
	// Create and attach the camera to the scene
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Get WindowSizing
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() * 0.5f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() * 0.5f;

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateQuad("INTROSTATE_BKGROUND", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("INTROSTATE_BKGROUND")->textureID = LoadTGA("Image//HelpMenu.tga");
	
	IntroStateBackground = Create::Sprite2DObject("INTROSTATE_BKGROUND",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(800.0f, 600.0f, 0.0f));

	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//calibri.tga");

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

	// >>>>> "bottom 1" is here
	textObj[0]->SetText("test"); // 1st
	textObj[1]->SetText("test"); // 2nd
	textObj[2]->SetText("test"); // 3rd


	cout << "CHelpMenuState loaded\n" << endl;
}
void CHelpMenuState::Update(double dt)
{
	if (KeyboardController::GetInstance()->IsKeyReleased(VK_BACK))
	{
		cout << "Loading MenuState" << endl;
		SceneManager::GetInstance()->SetActiveScene("MainMenuState");
	}
}
void CHelpMenuState::Render()
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
	textObj[0]->RenderUI();
}
void CHelpMenuState::Exit()
{
	// Remove the entity from EntityManager
	EntityManager::GetInstance()->RemoveEntity(IntroStateBackground);
	EntityManager::GetInstance()->Cleanthis_ForExit();

	// Remove the meshes which are specific to CHelpMenuState
	MeshBuilder::GetInstance()->RemoveMesh("INTROSTATE_BKGROUND");

	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
}