#pragma once

void ScriptMain();

enum SubMenus { //Add Sub Menus in here
	NOMENU,
	MAINMENU,
	TEST,
	PROTECTIONS,
	PLAYERLIST,
	PLAYERLIST_SELECTED
};

inline char* StringToChar(std::string string) // Handy little thing
{
	return _strdup(string.c_str());
}