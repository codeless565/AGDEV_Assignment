#include "ControlsManager.h"
#include "Lua\CLuaInterface.h"
#include "Application.h"

ControlsManager::ControlsManager(void) :
	keyMoveForward('W'),
	keyMoveBackward('S'),
	keyMoveLeft('A'),
	keyMoveRight('D'),
	keyOptions('O')
{
	keyMoveForward = CLuaInterface::GetInstance()->getKeyBoardValue("moveForward");
	keyMoveBackward = CLuaInterface::GetInstance()->getKeyBoardValue("moveBackward");
	keyMoveLeft = CLuaInterface::GetInstance()->getKeyBoardValue("moveLeft");
	keyMoveRight = CLuaInterface::GetInstance()->getKeyBoardValue("moveRight");
	keyOptions = CLuaInterface::GetInstance()->getKeyBoardValue("options");

	EditingForwardKey = false;
	EditingBackwardKey = false;
	EditingLeftKey = false;
	EditingRightKey = false;
	EditingOptionsKey = false;

	CurrentChar = 65;
	InvalidKeyPressed = false;
}

ControlsManager::~ControlsManager(void)
{
}

void ControlsManager::Init()
{

}

void ControlsManager::SaveOptionsState()
{
	CLuaInterface::GetInstance()->saveKeyBoardValue("moveForward", keyMoveForward);
	CLuaInterface::GetInstance()->saveKeyBoardValue("moveBackward", keyMoveBackward);
	CLuaInterface::GetInstance()->saveKeyBoardValue("moveLeft", keyMoveLeft);
	CLuaInterface::GetInstance()->saveKeyBoardValue("moveRight", keyMoveRight);
	CLuaInterface::GetInstance()->saveKeyBoardValue("options", keyOptions);
	CLuaInterface::GetInstance()->saveOptionsValue("width", Application::GetInstance().GetWindowWidth());
	CLuaInterface::GetInstance()->saveOptionsValue("height", Application::GetInstance().GetWindowHeight());

	std::remove("Image/Options.lua");
	std::rename("Image/Options2.lua", "Image/Options.lua");
}

bool ControlsManager::InvalidKey()
{
	if (CurrentChar == keyMoveForward ||
		CurrentChar == keyMoveBackward ||
		CurrentChar == keyMoveLeft ||
		CurrentChar == keyMoveRight ||
		CurrentChar == keyOptions)
		return true;

	return false;
}

void ControlsManager::ResetEditing()
{
	EditingForwardKey = false;
	EditingBackwardKey = false;
	EditingLeftKey = false;
	EditingRightKey = false;
	EditingOptionsKey = false;
}
