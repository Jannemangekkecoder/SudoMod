// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

std::atomic_bool g_running = true;

DWORD WINAPI ControlThread(LPVOID lpParam)
{
	if (AllocConsole()) {
		freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
		SetConsoleTitleW(L"SudoMod");
		SetConsoleCP(CP_UTF8);
		SetConsoleOutputCP(CP_UTF8);
	}
	Log_Info(R"(
   _____           _       __  __           _ 
  / ____|         | |     |  \/  |         | |
 | (___  _   _  __| | ___ | \  / | ___   __| |
  \___ \| | | |/ _` |/ _ \| |\/| |/ _ \ / _` |
  ____) | |_| | (_| | (_) | |  | | (_) | (_| |
 |_____/ \__,_|\__,_|\___/|_|  |_|\___/ \__,_|
                                              
                                              )");
	Log_Info("[Boot] Starting SudoMod");
	Hooking::Start((HMODULE)lpParam);

	while (g_running)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		std::this_thread::yield();
		if (GetAsyncKeyState(VK_END)) { g_running = false; }
	}

	Hooking::Cleanup();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ControlThread, hModule, NULL, NULL);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}