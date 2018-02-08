#pragma once

#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "../FPSCamera.h"
#include "../SpriteEntity.h"

class SceneManager;
class CGameOverState : public Scene
{
public:
	CGameOverState();
	~CGameOverState();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	virtual bool getKeepState() { return false; }

private:
	FPSCamera camera;
	SpriteEntity* MenuStateBackground;

	// Inherited via Scene

	//	TextEntity* textObj[3];
};
