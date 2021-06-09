#pragma once
#include "stdafx.h"
#include "UserInterface.h"
 
bool god = false;
int wantedLvl = 0;
void FeatureLoop() // Please for the love of god, make a better feature system.
{
	if (god) // I know you can just do Mem Value and then enable, but i want to show the skids how to loop a bool ;)
	{
		Memory::set_value<bool>({ 0x08, 0x189 }, true);
	}
	else
	{
		Memory::set_value<bool>({ 0x08, 0x189 }, false);
	}
}

void ScriptMain() {
	srand(GetTickCount());
	
	using namespace Menu;
	while (true) {
		gui.Keys();
		FeatureLoop();
		/* Features */
		
		switch (gui.currentMenu[gui.menuLevel]) {

		case mainmenu:
		{
			gui.SubTitle("HOME");
			gui.SubMenu("Test", TEST);
			if (gui.Option("Unload")) { g_running = false; }
		}
		break;
		case TEST:
		{
			gui.SubTitle("TEST");
			gui.Int("Wanted Level", wantedLvl, 0, 5); { Memory::set_value<int>({ 0x08, 0x10C8, 0x888 }, wantedLvl); }
			gui.Bool("God Mode", god);
			if (gui.Option("Test Option"))
			{
				Log_Info("Test Option Pressed!");
				Log_Info(PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID()));
			}

		}
		break;
		}
		if (gui.opened)
		{
			gui.Title();
			gui.End();
		}
		WAIT(0);
	}
}