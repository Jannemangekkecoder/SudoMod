#pragma once
#include "stdafx.h"
#include "UserInterface.h"

void ScriptMain() {
	srand(GetTickCount());
	
	using namespace Menu;
	while (true) {
		hooks.m_register_file(&gui.textureID, "C:\\SudoMod\\Textures\\SudoMod.ytd", true, "SudoMod.ytd", false); // Load YTD
		gui.Keys();

		/* Features */
		Local::UpdateLoop();
		Car::UpdateLoop();
		Gun::UpdateLoop();
		Misc::UpdateLoop();
		Other::UpdateLoop();
		Online::UpdateLoop();
		
		switch (gui.currentMenu) {

		case MAINMENU:
		{
			gui.Title("HOME");
			gui.SubMenu("Test", TEST);
			gui.SubMenu("Protections", PROTECTIONS);
			gui.SubMenu("Playerlist", PLAYERLIST);
			if (gui.Option("Unload")) { g_running = false; }
		}
		break;
		case TEST:
		{
			gui.Title("TEST");
			gui.Bool("God Mode", Local::god);
			if (gui.Option("Print Player Name"))
			{
				std::string scname = std::string("SocialClub Name: ") + PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID());
				Log_Info(_strdup(scname.c_str()));
			}
			if (gui.Option("Spawn T20")) { MenuFunctions::SpawnVehicle("T20"); }

		}
		break;
		case PROTECTIONS:
		{
			gui.Title("PROTECTIONS");
			gui.Bool("Log All Script Events", Online::event_logger);
			gui.Bool("Event Blocker", Online::event_blocker);
			gui.Bool("Karma", Online::event_karma);
		}
		break;
		case PLAYERLIST:
			gui.Title("PLAYERLIST");
			for (int i = 0; i < 32; ++i) {
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i))) {
					gui.SubMenu(PLAYER::GET_PLAYER_NAME(i), PLAYERLIST_SELECTED) ? Online::g_SelectedPlayer = i : NULL;
				}
			}
		break;
		case PLAYERLIST_SELECTED:
			gui.Title(PLAYER::GET_PLAYER_NAME(Online::g_SelectedPlayer));
			if (gui.Option("CEO Ban"))
			{
				uint64_t tseargs[3] = { -738295409, Online::g_SelectedPlayer, 1 };
				hooks.m_trigger_script_event(1, tseargs, 3, 1 << Online::g_SelectedPlayer);
				std::string sent = std::string("SudoMod || Sent Event CEO BAN to: ") + PLAYER::GET_PLAYER_NAME(Online::g_SelectedPlayer);
				Log_Info(_strdup(sent.c_str()));
			}
		break;
		}
		gui.End();
		WAIT(0);
	}
}