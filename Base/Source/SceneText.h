#ifndef SCENE_TEXT_H
#define SCENE_TEXT_H

#include "Scene.h"
#include "Mtx44.h"
#include "PlayerInfo/PlayerInfo.h"
#include "GroundEntity.h"
#include "FPSCamera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "GenericEntity.h"
#include "Enemy\Enemy.h"
#include "../Base/Source/Lua/CLuaInterface.h"

#include "../Base/Source/Message/ConcreteMessages.h"
#include "../Base/Source/Message/ObjectBase.h"

class ShaderProgram;
class SceneManager;
class TextEntity;
class Light;
class SceneText : public Scene , public ObjectBase
{	
public:
	SceneText();
	~SceneText();

	bool Handle(Message * message);

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	virtual bool getKeepState() 
	{
		if (gameOver)
			return false;
		
		return true;
	}

	bool gameOver;
	void RenderOptionsOnScreen();
	void ClearOptionsOnScreen();
private:
	//SceneText(SceneManager* _sceneMgr); // This is used to register to SceneManager

	ShaderProgram* currProg;
	CPlayerInfo* playerInfo;
	GroundEntity* groundEntity;
	FPSCamera camera;
	TextEntity* textObj[10];
	Light* lights[2];

	GenericEntity* theCube;
	std::vector<CEnemy*> MasterSpheres;
	std::vector<CEnemy*> EnemyChaser;

	//static SceneText* sInstance; // The pointer to the object that gets registered

	float fps;

	void SpawnSandBag();

	//WayPoint Reading from Lua file
	void ReadWayPoint();

	// Message Functions
	void GetNextNode(CEnemy* go);
	void CheckNearbyPlayer(CEnemy* go);
};

#endif