#pragma once
// includes the lua headers
#include <lua.hpp>
#include "Vector3.h"

class CLuaInterface
{
protected:
	static CLuaInterface *s_instance;
	CLuaInterface();
public:
	static CLuaInterface *GetInstance()
	{
		if (!s_instance)
			s_instance = new CLuaInterface;
		return s_instance;
	}
	static bool DropInstance()
	{
		if (s_instance)
		{
			// Drop the Lua Interface Class
			s_instance->Drop();
			delete s_instance;
			s_instance = NULL;
			return true;
		}
		return false;
	}
	virtual ~CLuaInterface();
	// Initialisation of the Lua Interface Class
	bool Init();
	// Run the Lua Interface Class
	void Run();
	// Drop the Lua Interface Class
	void Drop();
	// Pointer to the Lua State
	lua_State *theLuaState;

	int getIntValue(const char* varName);
	float getFloatValue(const char* varName);
	bool getBooleanValue(const char* varName);
	char getcharValue(const char* varName);
	Vector3 getVector3Values(const char* varName);

	void saveIntValue(const char* varName, int value, bool overwrite = false);
	void saveFloatValue(const char* varName, float value, bool overwrite = false);
	void saveBooleanValue(const char* varName, bool value, bool overwrite = false);
	

	float getDistanceSquareValue(Vector3 source, Vector3 destination);
	int getVariableValues(const char* varName, int &a, int &b, int &c, int &d);

	// Extract a field from a table
	float getField(const char *key);

	lua_State *theErrorState;
	// Ger error message using an error code
	void error(const char *errorCode);

	
	lua_State *theOptionState;
	char getKeyBoardValue(const char* varName);
	void saveKeyBoardValue(const char* varName, char value, bool overwrite = false);
};