#ifndef CONTROLS_MANAGER_H
#define CONTROLS_MANAGER_H

#include <iostream>
#include "SingletonTemplate.h"

class ControlsManager : public Singleton<ControlsManager>
{
//protected:
//	static ControlsManager *s_instance;
private:
	// Key to move the player
	char keyMoveForward;
	char keyMoveBackward;
	char keyMoveLeft;
	char keyMoveRight;
	char keyOptions;

	bool EditingForwardKey;
	bool EditingBackwardKey;
	bool EditingLeftKey;
	bool EditingRightKey;
	bool EditingOptionsKey;

	char CurrentChar;
public:
	//static ControlsManager *GetInstance()
	//{
	//	if (!s_instance)
	//		s_instance = new ControlsManager;
	//	return s_instance;
	//}
	//static bool DropInstance()
	//{
	//	if (s_instance)
	//	{
	//		delete s_instance;
	//		s_instance = NULL;
	//		return true;
	//	}
	//	return false;
	//}
	ControlsManager(void);
	~ControlsManager(void);


	char getkeyMoveForward() { return keyMoveForward; }
	char getkeyMoveBackward() { return keyMoveBackward; }
	char getkeyMoveLeft() { return keyMoveLeft; }
	char getkeyMoveRight() { return keyMoveRight; }
	char getkeyOptions() { return keyOptions; }

	void setKeyMoveForward(char _key) { keyMoveForward = _key; }
	void setKeyMoveBackward(char _key) { keyMoveBackward = _key; }
	void setKeyMoveLeft(char _key) { keyMoveLeft = _key; }
	void setKeyMoveRight(char _key) { keyMoveRight = _key; }
	void setKeyOptions(char _key) { keyOptions = _key; }


	bool getEditingForwardKey() { return EditingForwardKey; }
	bool getEditingBackwardKey() { return EditingBackwardKey; }
	bool getEditingLeftKey() { return EditingLeftKey; }
	bool getEditingRightKey() { return EditingRightKey; }
	bool getEditingOptionsKey() { return EditingOptionsKey; }

	void setEditingForwardKey(bool _key) { EditingForwardKey = _key; }
	void setEditingBackwardKey(bool _key) { EditingBackwardKey = _key; }
	void setEditingLeftKey(bool _key) { EditingLeftKey = _key; }
	void setEditingRightKey(bool _key) { EditingRightKey = _key; }
	void setEditingOptionsKey(bool _key) { EditingOptionsKey = _key; }

	void setInvalidKeyPressed(bool _key) { InvalidKeyPressed = _key; }

	char getCurrentChar() { return CurrentChar; }
	void addCurrentChar(unsigned value) { CurrentChar += value; }

	void Init();
	void SaveOptionsState();
	bool InvalidKey();
	bool InvalidKeyPressed;
	void ResetEditing();
};

#endif