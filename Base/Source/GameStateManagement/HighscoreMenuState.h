#pragma once

#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "../FPSCamera.h"
#include "../SpriteEntity.h"

#define TEXTARRAYLIMIT 3

class TextEntity;
class SceneManager;
class TextEntity;
class CHighscoreMenuState : public Scene
{
public:
	CHighscoreMenuState();
	~CHighscoreMenuState();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	virtual bool getKeepState() { return false; }

private:
	FPSCamera camera;
	SpriteEntity* MenuStateBackground;
	TextEntity* textObj[TEXTARRAYLIMIT];
};
