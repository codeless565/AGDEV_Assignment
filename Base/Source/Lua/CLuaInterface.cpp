#include "CLuaInterface.h"
#include <iostream>
#include <string>
using namespace std;

// Allocating and initializing CLuaInterface's static data member
// The pointer is allocated but not the object's constructor

CLuaInterface *CLuaInterface::s_instance = 0;

CLuaInterface::CLuaInterface()
	:theLuaState(NULL)
{
}

CLuaInterface::~CLuaInterface()
{
}


// Initialisation of the Lua Interface Class
bool CLuaInterface::Init()
{
	bool result = false;

	// 1. Create lua state
	theLuaState = lua_open();

	if (theLuaState)
	{
		// 2. Load lua auxiliary libraries
		luaL_openlibs(theLuaState);

		// 3. Load lua script
		luaL_dofile(theLuaState, "Image//DM2240.lua");

		result = true;
	}

	return result;
}

// Run the Lua Interface Class
void CLuaInterface::Run()
{
	if (theLuaState == NULL)
		return;

	// 4. Read the variables
	// lua_getglobal(lua_State*, const char*)
	lua_getglobal(theLuaState, "title");
	// extract value, index -1 as variable is already retrieved using lua_getglobal
	const char *title = lua_tostring(theLuaState, -1);

	lua_getglobal(theLuaState, "width");
	int width = lua_tointeger(theLuaState, -1);

	lua_getglobal(theLuaState, "height");
	int height = lua_tointeger(theLuaState, -1);

	// Display on screen
	cout << title << endl;
	cout << "-----------------------------------------" << endl;
	cout << "Width of screen : " << width << endl;
	cout << "Height of screen : " << height << endl;
}

void CLuaInterface::Drop()
{
	// Close the lua state
	if (theLuaState)
		lua_close(theLuaState);
}

int CLuaInterface::getIntValue(const char * varName)
{
	lua_getglobal(theLuaState, varName);
	int tempValue = lua_tointeger(theLuaState, -1);
	return tempValue;
}

float CLuaInterface::getFloatValue(const char * varName)
{
	lua_getglobal(theLuaState, varName);
	float tempValue = lua_tonumber(theLuaState, -1);
	return tempValue;
}

bool CLuaInterface::getBooleanValue(const char* varName)
{
	lua_getglobal(theLuaState, varName);
	bool tempValue = lua_toboolean(theLuaState, -1);
	return tempValue;
}

void CLuaInterface::saveIntValue(const char * varName, int value, bool overwrite)
{
	std::string temp = varName;
	temp.append(" = ");
	temp.append(std::to_string(value));
	temp.append("\n");

	if (theLuaState)
	{
		lua_getglobal(theLuaState, "SaveToLuaFile");
		lua_pushstring(theLuaState, temp.c_str());
		lua_pushinteger(theLuaState, overwrite);
	}
	lua_call(theLuaState, 2, 0);
}

void CLuaInterface::saveFloatValue(const char * varName, float value, bool overwrite)
{
	std::string temp = varName;
	temp.append(" = ");
	temp.append(std::to_string(value));
	temp.append("\n");

	if (theLuaState)
	{
		lua_getglobal(theLuaState, "SaveToLuaFile");
		lua_pushstring(theLuaState, temp.c_str());
		lua_pushinteger(theLuaState, overwrite);
	}
	lua_call(theLuaState, 2, 0);
}

void CLuaInterface::saveBooleanValue(const char * varName, bool value, bool overwrite)
{
	std::string temp = varName;
	temp.append(" = ");
	temp.append(std::to_string(value));
	temp.append("\n");

	if (theLuaState)
	{
		lua_getglobal(theLuaState, "SaveToLuaFile");
		lua_pushstring(theLuaState, temp.c_str());
		lua_pushinteger(theLuaState, overwrite);
	}
	lua_call(theLuaState, 2, 0);
}
