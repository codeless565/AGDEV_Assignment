#pragma once
// includes the lua headers
#include <lua.hpp>
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

	void saveIntValue(const char* varName, int value, bool overwrite = false);
	void saveFloatValue(const char* varName, float value, bool overwrite = false);
	void saveBooleanValue(const char* varName, bool value, bool overwrite = false);
};