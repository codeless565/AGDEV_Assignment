#pragma once

#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "../FPSCamera.h"
#include "../SpriteEntity.h"

#define TEXTARRAYLIMIT 10

class TextEntity;
class SceneManager;
class TextEntity;
class COptionsMenuState : public Scene
{
public:
	COptionsMenuState();
	~COptionsMenuState();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	virtual bool getKeepState() { return false; }

//	bool EditGraphics;
//	int GraphicsChoices;

	int windowWidth;
	int windowHeight;
private:
	FPSCamera camera;
	SpriteEntity* IntroStateBackground;
	TextEntity* textObj[TEXTARRAYLIMIT];
};
