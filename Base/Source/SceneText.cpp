#include "SceneText.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "KeyboardController.h"
#include "MouseController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"
#include "EntityManager.h"

#include "GenericEntity.h"
#include "GroundEntity.h"
#include "TextEntity.h"
#include "SpriteEntity.h"
#include "Light.h"
#include "SkyBox/SkyBoxEntity.h"
#include "Waypoint\WaypointManager.h"

#include "SceneGraph.h"
#include "SceneNode.h"
//#include "CUpdateTransformation.h"

#include <iostream>
using namespace std;

//SceneText* SceneText::sInstance = new SceneText(SceneManager::GetInstance());

SceneText::SceneText()
{
}

//SceneText::SceneText(SceneManager* _sceneMgr)
//{
//	_sceneMgr->AddScene("Start", this);
//}

void SceneText::SpawnSandBag()
{
	for (int i = 0; i < 15; ++i)
	{
		float xPos = Math::RandFloatMinMax(0.f, 50.f);
		float zPos = Math::RandFloatMinMax(0.f, 50.f);
		GenericEntity* Sandbag = Create::Entity("cubeSG", Vector3(xPos, groundEntity->GetPosition().y, 30.0f*i), Vector3(10.f, 10.f, 10.f)); //Somehow cannot use Asset, it doesnt render
		Sandbag->InitLOD("SandBag_High", "SandBag_Med", "SandBag_Low");
	}
}

SceneText::~SceneText()
{
	CSceneGraph::GetInstance()->Destroy();
}

void SceneText::Init()
{
	lights[0] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[0]", lights[0]);
	lights[0]->type = Light::LIGHT_DIRECTIONAL;
	lights[0]->position.Set(0, 20, 0);
	lights[0]->color.Set(1, 1, 1);
	lights[0]->power = 1;
	lights[0]->kC = 1.f;
	lights[0]->kL = 0.01f;
	lights[0]->kQ = 0.001f;
	lights[0]->cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0]->cosInner = cos(Math::DegreeToRadian(30));
	lights[0]->exponent = 3.f;
	lights[0]->spotDirection.Set(0.f, 1.f, 0.f);
	lights[0]->name = "lights[0]";

	lights[1] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[1]", lights[1]);
	lights[1]->type = Light::LIGHT_DIRECTIONAL;
	lights[1]->position.Set(1, 1, 0);
	lights[1]->color.Set(1, 1, 0.5f);
	lights[1]->power = 0.4f;
	lights[1]->name = "lights[1]";


	// Create the playerinfo instance, which manages all information about the player
	playerInfo = CPlayerInfo::GetInstance();
	playerInfo->Init();

	// Create and attach the camera to the scene
	//camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	camera.Init(playerInfo->GetPos(), playerInfo->GetTarget(), playerInfo->GetUp());
	playerInfo->AttachCamera(&camera);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	/********************************************************
	********************* MESHES ****************************
	********************************************************/
	{
		MeshBuilder::GetInstance()->GenerateAxes("reference");
		MeshBuilder::GetInstance()->GenerateCrossHair("crosshair");
		MeshBuilder::GetInstance()->GenerateQuad("quad", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("quad")->textureID = LoadTGA("Image//calibri.tga");
		MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
		MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//calibri.tga");
		MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);
		MeshBuilder::GetInstance()->GenerateOBJ("Chair", "OBJ//chair.obj");
		MeshBuilder::GetInstance()->GetMesh("Chair")->textureID = LoadTGA("Image//chair.tga");
		MeshBuilder::GetInstance()->GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
		MeshBuilder::GetInstance()->GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
		MeshBuilder::GetInstance()->GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 0.5f);
		MeshBuilder::GetInstance()->GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
		MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.0f, 1.0f, 0.0f), 1.0f);
		MeshBuilder::GetInstance()->GetMesh("cone")->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
		MeshBuilder::GetInstance()->GetMesh("cone")->material.kSpecular.Set(0.f, 0.f, 0.f);
		MeshBuilder::GetInstance()->GenerateQuad("GRASS_DARKGREEN", Color(1, 1, 1), 1.f);
		//MeshBuilder::GetInstance()->GetMesh("GRASS_DARKGREEN")->textureID = LoadTGA("Image//grass_darkgreen.tga");
		MeshBuilder::GetInstance()->GetMesh("GRASS_DARKGREEN")->textureID = LoadTGA("Image//ground_pebble.tga");
		MeshBuilder::GetInstance()->GenerateQuad("GEO_GRASS_LIGHTGREEN", Color(1, 1, 1), 1.f);
		//MeshBuilder::GetInstance()->GetMesh("GEO_GRASS_LIGHTGREEN")->textureID = LoadTGA("Image//grass_lightgreen.tga");
		MeshBuilder::GetInstance()->GetMesh("GEO_GRASS_LIGHTGREEN")->textureID = LoadTGA("Image//ground_pebble.tga");

		MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_FRONT", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BACK", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_LEFT", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_RIGHT", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_TOP", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BOTTOM", Color(1, 1, 1), 1.f);
		//MeshBuilder::GetInstance()->GetMesh("SKYBOX_FRONT")->textureID = LoadTGA("Image//SkyBox//skybox_front.tga");
		//MeshBuilder::GetInstance()->GetMesh("SKYBOX_BACK")->textureID = LoadTGA("Image//SkyBox//skybox_back.tga");
		//MeshBuilder::GetInstance()->GetMesh("SKYBOX_LEFT")->textureID = LoadTGA("Image//SkyBox//skybox_left.tga");
		//MeshBuilder::GetInstance()->GetMesh("SKYBOX_RIGHT")->textureID = LoadTGA("Image//SkyBox//skybox_right.tga");
		//MeshBuilder::GetInstance()->GetMesh("SKYBOX_TOP")->textureID = LoadTGA("Image//SkyBox//skybox_top.tga");
		//MeshBuilder::GetInstance()->GetMesh("SKYBOX_BOTTOM")->textureID = LoadTGA("Image//SkyBox//skybox_bottom.tga");
		MeshBuilder::GetInstance()->GetMesh("SKYBOX_FRONT")->textureID = LoadTGA("Image//CastleBox//sandcastle_ft.tga");
		MeshBuilder::GetInstance()->GetMesh("SKYBOX_BACK")->textureID = LoadTGA("Image//CastleBox//sandcastle_bk.tga");
		MeshBuilder::GetInstance()->GetMesh("SKYBOX_LEFT")->textureID = LoadTGA("Image//CastleBox//sandcastle_lf.tga");
		MeshBuilder::GetInstance()->GetMesh("SKYBOX_RIGHT")->textureID = LoadTGA("Image//CastleBox//sandcastle_rt.tga");
		MeshBuilder::GetInstance()->GetMesh("SKYBOX_TOP")->textureID = LoadTGA("Image//CastleBox//sandcastle_up.tga");
		MeshBuilder::GetInstance()->GetMesh("SKYBOX_BOTTOM")->textureID = LoadTGA("Image//CastleBox//sandcastle_dn.tga");
		MeshBuilder::GetInstance()->GenerateRay("laser", 10.0f);
		MeshBuilder::GetInstance()->GenerateQuad("GRIDMESH", Color(1, 1, 1), 10.f);
	}

	//MasterSphere
	MeshBuilder::GetInstance()->GenerateOBJ("MS_High", "OBJ//MasterSphere_High.obj");
	MeshBuilder::GetInstance()->GetMesh("MS_High")->textureID = LoadTGA("Image//MasterSphere.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("MS_Med", "OBJ//MasterSphere_Med.obj");
	MeshBuilder::GetInstance()->GetMesh("MS_Med")->textureID = LoadTGA("Image//MasterSphere.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("MS_Low", "OBJ//MasterSphere_Low.obj");
	MeshBuilder::GetInstance()->GetMesh("MS_Low")->textureID = LoadTGA("Image//MasterSphere.tga");

	//================== CHILD RED ================================
	{
		//ChildSphere Red
		MeshBuilder::GetInstance()->GenerateOBJ("CSRed_High", "OBJ//ChildSphere_High.obj");
		MeshBuilder::GetInstance()->GetMesh("CSRed_High")->textureID = LoadTGA("Image//Camo_Red.tga");
		MeshBuilder::GetInstance()->GenerateOBJ("CSRed_Med", "OBJ//ChildSphere_Med.obj");
		MeshBuilder::GetInstance()->GetMesh("CSRed_Med")->textureID = LoadTGA("Image//Camo_Red.tga");
		MeshBuilder::GetInstance()->GenerateOBJ("CSRed_Low", "OBJ//ChildSphere_Low.obj");
		MeshBuilder::GetInstance()->GetMesh("CSRed_Low")->textureID = LoadTGA("Image//Camo_Red.tga");
	}
	//================== CHILD BLUE ===============================
	{
		//ChildSphere Blue
		MeshBuilder::GetInstance()->GenerateOBJ("CSBlue_High", "OBJ//ChildSphere_High.obj");
		MeshBuilder::GetInstance()->GetMesh("CSBlue_High")->textureID = LoadTGA("Image//Camo_Blue.tga");
		MeshBuilder::GetInstance()->GenerateOBJ("CSBlue_Med", "OBJ//ChildSphere_Med.obj");
		MeshBuilder::GetInstance()->GetMesh("CSBlue_Med")->textureID = LoadTGA("Image//Camo_Blue.tga");
		MeshBuilder::GetInstance()->GenerateOBJ("CSBlue_Low", "OBJ//ChildSphere_Low.obj");
		MeshBuilder::GetInstance()->GetMesh("CSBlue_Low")->textureID = LoadTGA("Image//Camo_Blue.tga");
	}
	//================== CHILD GREEN ==============================
	{
		//ChildSphere Green
		MeshBuilder::GetInstance()->GenerateOBJ("CSGreen_High", "OBJ//ChildSphere_High.obj");
		MeshBuilder::GetInstance()->GetMesh("CSGreen_High")->textureID = LoadTGA("Image//Camo_Green.tga");
		MeshBuilder::GetInstance()->GenerateOBJ("CSGreen_Med", "OBJ//ChildSphere_Med.obj");
		MeshBuilder::GetInstance()->GetMesh("CSGreen_Med")->textureID = LoadTGA("Image//Camo_Green.tga");
		MeshBuilder::GetInstance()->GenerateOBJ("CSGreen_Low", "OBJ//ChildSphere_Low.obj");
		MeshBuilder::GetInstance()->GetMesh("CSGreen_Low")->textureID = LoadTGA("Image//Camo_Green.tga");
	}
	//================== CHILD YELLOW =============================
	{
		//ChildSphere Yellow
		MeshBuilder::GetInstance()->GenerateOBJ("CSYellow_High", "OBJ//ChildSphere_High.obj");
		MeshBuilder::GetInstance()->GetMesh("CSYellow_High")->textureID = LoadTGA("Image//Camo_Yellow.tga");
		MeshBuilder::GetInstance()->GenerateOBJ("CSYellow_Med", "OBJ//ChildSphere_Med.obj");
		MeshBuilder::GetInstance()->GetMesh("CSYellow_Med")->textureID = LoadTGA("Image//Camo_Yellow.tga");
		MeshBuilder::GetInstance()->GenerateOBJ("CSYellow_Low", "OBJ//ChildSphere_Low.obj");
		MeshBuilder::GetInstance()->GetMesh("CSYellow_Low")->textureID = LoadTGA("Image//Camo_Yellow.tga");
	}

	//Asset - Pillar
	MeshBuilder::GetInstance()->GenerateOBJ("Pillar_High", "OBJ//Pillar_High.obj");
	MeshBuilder::GetInstance()->GetMesh("Pillar_High")->textureID = LoadTGA("Image//Pillar.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Pillar_Med", "OBJ//Pillar_Med.obj");
	MeshBuilder::GetInstance()->GetMesh("Pillar_Med")->textureID = LoadTGA("Image//Pillar.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Pillar_Low", "OBJ//Pillar_Low.obj");
	MeshBuilder::GetInstance()->GetMesh("Pillar_Low")->textureID = LoadTGA("Image//Pillar.tga");
	//Asset - Tree
	MeshBuilder::GetInstance()->GenerateOBJ("Tree_High", "OBJ//Tree_High.obj");
	MeshBuilder::GetInstance()->GetMesh("Tree_High")->textureID = LoadTGA("Image//Tree.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Tree_Med", "OBJ//Tree_Med.obj");
	MeshBuilder::GetInstance()->GetMesh("Tree_Med")->textureID = LoadTGA("Image//Tree.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Tree_Low", "OBJ//Tree_Low.obj");
	MeshBuilder::GetInstance()->GetMesh("Tree_Low")->textureID = LoadTGA("Image//Tree.tga");
	//Asset - SandBag
	MeshBuilder::GetInstance()->GenerateOBJ("SandBag_High", "OBJ//SandBag_High.obj");
	MeshBuilder::GetInstance()->GetMesh("SandBag_High")->textureID = LoadTGA("Image//SandBag.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("SandBag_Med", "OBJ//SandBag_Med.obj");
	MeshBuilder::GetInstance()->GetMesh("SandBag_Med")->textureID = LoadTGA("Image//SandBag.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("SandBag_Low", "OBJ//SandBag_Low.obj");
	MeshBuilder::GetInstance()->GetMesh("SandBag_Low")->textureID = LoadTGA("Image//SandBag.tga");

	groundEntity = Create::Ground("GRASS_DARKGREEN", "GEO_GRASS_LIGHTGREEN");
	//	Create::Text3DObject("text", Vector3(0.0f, 0.0f, 0.0f), "DM2210", Vector3(10.0f, 10.0f, 10.0f), Color(0, 1, 1));
	Create::Sprite2DObject("crosshair", Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f));

	SkyBoxEntity* theSkyBox = Create::SkyBox("SKYBOX_FRONT", "SKYBOX_BACK",
		"SKYBOX_LEFT", "SKYBOX_RIGHT",
		"SKYBOX_TOP", "SKYBOX_BOTTOM");

	// Customise the ground entity
	groundEntity->SetPosition(Vector3(0, -10, 0));
	groundEntity->SetScale(Vector3(100.0f, 100.0f, 100.0f));
	groundEntity->SetGrids(Vector3(10.0f, 1.0f, 10.0f));
	/******************************************
	******** SPATIAL PARTITIONING *************
	*******************************************/
	//Setup the Spatial Parition and pass it to EntityManager to manage
	CSpatialPartition::GetInstance()->Init(100, 100, 10, 10);
	CSpatialPartition::GetInstance()->SetMesh("GRIDMESH");
	CSpatialPartition::GetInstance()->SetCamera(&camera);
	CSpatialPartition::GetInstance()->SetLevelofDetails(20000.f, 80000.f);
	EntityManager::GetInstance()->SetSpatialPartition(CSpatialPartition::GetInstance());

	// Create entities into the scene
	Create::Entity("reference", Vector3(0.0f, 0.0f, 0.0f)); // Reference
	Create::Entity("lightball", Vector3(lights[0]->position.x, lights[0]->position.y, lights[0]->position.z)); // Lightball

	MeshBuilder::GetInstance()->GenerateCube("cubeSG", Color(1.0f, 0.64f, 0.0f), 1.0f);

	/*****************************************************
	=\/========= Asset = non destructible =============\/=
	******************************************************/
	GenericEntity* Tree = Create::Entity("cubeSG", Vector3(20.0f, -10.0f, 0.0f), Vector3(20.f, 20.f, 20.f)); //Somehow cannot use Asset, it doesnt render
	Tree->SetCollider(true);
	Tree->SetAABB(Vector3(20.f, 20.f, 20.f), Vector3(-20.f, -20.f, -20.f));
	Tree->InitLOD("Tree_High", "Tree_Med", "Tree_Low");



	GenericEntity* Pillar = Create::Entity("cubeSG", Vector3(50.f, 0.0f, 50.f), Vector3(20.f, 20.f, 20.f)); //Somehow cannot use Asset, it doesnt render
	Pillar->InitLOD("Pillar_High", "Pillar_Med", "Pillar_Low");

	GenericEntity* Pillar2 = Create::Entity("cubeSG", Vector3(-50.f, 0.0f, 50.f), Vector3(20.f, 20.f, 20.f)); //Somehow cannot use Asset, it doesnt render
	Pillar2->InitLOD("Pillar_High", "Pillar_Med", "Pillar_Low");

	GenericEntity* Pillar3 = Create::Entity("cubeSG", Vector3(50.f, 0.0f, -50.f), Vector3(20.f, 20.f, 20.f)); //Somehow cannot use Asset, it doesnt render
	Pillar3->InitLOD("Pillar_High", "Pillar_Med", "Pillar_Low");

	GenericEntity* Pillar4 = Create::Entity("cubeSG", Vector3(-50.f, 0.0f, -50.f), Vector3(20.f, 20.f, 20.f)); //Somehow cannot use Asset, it doesnt render
	Pillar4->InitLOD("Pillar_High", "Pillar_Med", "Pillar_Low");

	SpawnSandBag();

	/*****************************************************
	=\/=============== Destructibles ==================\/=
	******************************************************/
	//GenericEntity* aCube = Create::Entity("cube", Vector3(-20.0f, 0.0f,-20.0f));
	//aCube->SetCollider(true);
	//aCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	//aCube->InitLOD("cube", "sphere", "cubeSG");

	//GenericEntity* anotherCube = Create::Entity("cube", Vector3(-20.0f, 1.1f, -20.0f));
	//anotherCube->SetCollider(true);
	//anotherCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	////Add the pointer to this new entity to the SceneGraph
	//CSceneNode* anotherNode = theNode->AddChild(anotherCube);
	//if (anotherCube == NULL)
	//	cout << "EntityManager::AddEntity: Unable to add to scenegraph!\n";

	//=======================================================
	playerInfo->SetTerrain(groundEntity);

	// Create multiple MasterBlocks
	for (int i = 0; i < 2; ++i)
	{
		Vector3 Pos, Target;

		switch (i)
		{
		case 0:
			Pos = Vector3(0, 25, 100);
			Target = Vector3(0, 25, 0);
			break;
		case 1:
			Pos = Vector3(0, 25, -100);
			Target = Vector3(0, 25, 0);
			break;
		}
		CEnemy* temp = new CEnemy();
		temp->Init((CEnemy::COLOR)i, Pos, Target);
		temp->SetTerrain(groundEntity);
		temp->InitSpawnChild(10);

		MasterSpheres.push_back(temp);
	}

	// Setup the 2D entities
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	float fontSize = 25.0f;
	float halfFontSize = fontSize / 2.0f;
	for (int i = 0; i < 10; ++i)
	{
		textObj[i] = Create::Text2DObject("text", Vector3(-halfWindowWidth, -halfWindowHeight + fontSize*i + halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f, 1.0f, 0.0f));
	}

	gameOver = false;

	// Create a Waypoint inside WaypointManager
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "Waypoint_A_1");
	int aWayPoint = CWaypointManager::GetInstance()->AddWaypoint(Vector3(CLuaInterface::GetInstance()->getField("x"),
		CLuaInterface::GetInstance()->getField("y"),
		CLuaInterface::GetInstance()->getField("z")));
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "Waypoint_A_2");
	int anotherWaypoint = CWaypointManager::GetInstance()->AddWaypoint(aWayPoint, Vector3(CLuaInterface::GetInstance()->getField("x"),
		CLuaInterface::GetInstance()->getField("y"),
		CLuaInterface::GetInstance()->getField("z")));
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "Waypoint_A_3");
	CWaypointManager::GetInstance()->AddWaypoint(anotherWaypoint, Vector3(CLuaInterface::GetInstance()->getField("x"),
		CLuaInterface::GetInstance()->getField("y"),
		CLuaInterface::GetInstance()->getField("z")));
	CWaypointManager::GetInstance()->PrintSelf();
}

void SceneText::Update(double dt)
{
	if (playerInfo->getScore() >= 150 || KeyboardController::GetInstance()->IsKeyPressed('U'))
		gameOver = true;

	// Update our entities
	EntityManager::GetInstance()->Update(dt);

	// THIS WHOLE CHUNK TILL <THERE> CAN REMOVE INTO ENTITIES LOGIC! Or maybe into a scene function to keep the update clean
	if (KeyboardController::GetInstance()->IsKeyDown('1'))
		glEnable(GL_CULL_FACE);
	if (KeyboardController::GetInstance()->IsKeyDown('2'))
		glDisable(GL_CULL_FACE);
	if (KeyboardController::GetInstance()->IsKeyDown('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (KeyboardController::GetInstance()->IsKeyDown('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (KeyboardController::GetInstance()->IsKeyDown('5'))
	{
		lights[0]->type = Light::LIGHT_POINT;
	}
	else if (KeyboardController::GetInstance()->IsKeyDown('6'))
	{
		lights[0]->type = Light::LIGHT_DIRECTIONAL;
	}
	else if (KeyboardController::GetInstance()->IsKeyDown('7'))
	{
		lights[0]->type = Light::LIGHT_SPOT;
	}

	if (KeyboardController::GetInstance()->IsKeyDown('I'))
		lights[0]->position.z -= (float)(10.f * dt);
	if (KeyboardController::GetInstance()->IsKeyDown('K'))
		lights[0]->position.z += (float)(10.f * dt);
	if (KeyboardController::GetInstance()->IsKeyDown('J'))
		lights[0]->position.x -= (float)(10.f * dt);
	if (KeyboardController::GetInstance()->IsKeyDown('L'))
		lights[0]->position.x += (float)(10.f * dt);
	if (KeyboardController::GetInstance()->IsKeyDown('O'))
		lights[0]->position.y -= (float)(10.f * dt);
	if (KeyboardController::GetInstance()->IsKeyDown('P'))
		lights[0]->position.y += (float)(10.f * dt);

	// if the left mouse button was released
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
	{
		cout << "Left Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::RMB))
	{
		cout << "Right Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::MMB))
	{
		cout << "Middle Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) != 0.0)
	{
		cout << "Mouse Wheel has offset in X-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) << endl;
	}
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) != 0.0)
	{
		cout << "Mouse Wheel has offset in Y-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) << endl;
	}
	// <THERE>

	// Update the player position and other details based on keyboard and mouse inputs
	playerInfo->Update(dt);

	//camera.Update(dt); // Can put the camera into an entity rather than here (Then we don't have to write this)

	GraphicsManager::GetInstance()->UpdateLights(dt);

	// Update the 2 text object values. NOTE: Can do this in their own class but i'm lazy to do it now :P
	// Eg. FPSRenderEntity or inside RenderUI for LightEntity
	std::ostringstream ss;
	if (gameOver)
	{
		ss.str("");
		ss << "Final Score: " << playerInfo->getScore();
		textObj[0]->SetText(ss.str());

		ss.str("");
		ss.precision(4);
		ss << "Accuracy: " << (playerInfo->getShots() / playerInfo->getTotalShots()) << "%";
		textObj[1]->SetText(ss.str());

		ss.str("");
		ss << "Game Over!";
		textObj[2]->SetText(ss.str());
	}
	else
	{
		ss.str("");
		ss << "Score: " << playerInfo->getScore();
		textObj[0]->SetText(ss.str());

		ss.str("");
		ss.precision(5);
		float fps = (float)(1.f / dt);
		ss << "FPS: " << fps;
		textObj[1]->SetText(ss.str());

		ss.str("");
		ss.precision(4);
		if (playerInfo->getCurrWeapon() == playerInfo->getPrimaryWeapon())
			ss << "Laser Blaster " << playerInfo->getCurrWeapon()->GetMagRound() << "/" << (playerInfo->getCurrWeapon()->GetMaxTotalRound() / playerInfo->getCurrWeapon()->GetMaxMagRound());
		else if (playerInfo->getCurrWeapon() == playerInfo->getSecondaryWeapon())
			ss << "Pistol " << playerInfo->getCurrWeapon()->GetMagRound() << "/" << (playerInfo->getCurrWeapon()->GetMaxTotalRound() / playerInfo->getCurrWeapon()->GetMaxMagRound());
		else if (playerInfo->getCurrWeapon() == playerInfo->getTertiaryWeapon())
			ss << "Grenade " << playerInfo->getCurrWeapon()->GetMagRound() << "/" << (playerInfo->getCurrWeapon()->GetMaxTotalRound() / playerInfo->getCurrWeapon()->GetMaxMagRound());
		textObj[2]->SetText(ss.str());

		if (playerInfo->getOptionsState())
			RenderOptionsOnScreen();
		else
			ClearOptionsOnScreen();
	}
}

void SceneText::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GraphicsManager::GetInstance()->UpdateLightUniforms();

	// Setup 3D pipeline then render 3D
	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);
	EntityManager::GetInstance()->Render();

	// Setup 2D pipeline then render 2D
	int halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2;
	int halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2;
	GraphicsManager::GetInstance()->SetOrthographicProjection(-halfWindowWidth, halfWindowWidth, -halfWindowHeight, halfWindowHeight, -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();
	EntityManager::GetInstance()->RenderUI();
}

void SceneText::Exit()
{
	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
	playerInfo->DetachCamera();

	if (playerInfo->DropInstance() == false)
	{
#if _DEBUGMODE==1
		cout << "Unable to drop PlayerInfo class" << endl;
#endif
	}

	// Delete the lights
	delete lights[0];
	delete lights[1];
}

void SceneText::RenderOptionsOnScreen()
{
	std::ostringstream ss;
	ss.str("");
	ss << "MoveForward " << playerInfo->getkeyMoveForward();
	textObj[3]->SetText(ss.str());

	ss.str("");
	ss << "MoveBackward " << playerInfo->getkeyMoveBackward();
	textObj[4]->SetText(ss.str());

	ss.str("");
	ss << "MoveLeft " << playerInfo->getkeyMoveLeft();
	textObj[5]->SetText(ss.str());

	ss.str("");
	ss << "MoveRight" << playerInfo->getkeyMoveRight();
	textObj[6]->SetText(ss.str());

	ss.str("");
	if (playerInfo->getEditingForwardKey())
		ss << "Edit forward key to:" << playerInfo->getCurrentChar();
	else if (playerInfo->getEditingBackwardKey())
		ss << "Edit backward key to:" << playerInfo->getCurrentChar();
	else if (playerInfo->getEditingLeftKey())
		ss << "Edit left key to:" << playerInfo->getCurrentChar();
	else if (playerInfo->getEditingRightKey())
		ss << "Edit right key to:" << playerInfo->getCurrentChar();
	textObj[8]->SetText(ss.str());
}

void SceneText::ClearOptionsOnScreen()
{
	std::ostringstream ss;
	ss.str("");
	textObj[3]->SetText(ss.str());
	textObj[4]->SetText(ss.str());
	textObj[5]->SetText(ss.str());
	textObj[6]->SetText(ss.str());
	textObj[8]->SetText(ss.str());
}
