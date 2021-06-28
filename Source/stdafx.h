// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// Windows Library Files:
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Winmm.lib")

// Windows Header Files:
#include <windows.h>
#include <Mmsystem.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <Psapi.h>
#include <MinHook.h>
#include <timeapi.h>
#include <time.h>
#include <atomic>
#include <chrono>
#include <thread>

// Additional Header Files:
#include "StringToChar.h"
#include "Memory.h"
#include "types.h"
#include "InputHook.h"
#include "keyboard.h"
#include "CrossMapping.h"
#include "NativeInvoker.h"
#include "nativeCaller.h"
#include "natives.h"
#include "Hooking.h"
#include "Logger.h"
#include "enums.h"
#include "globalHandle.h"

// Menu Files:
#include "UserInterface.h"
#include "Script.h"
#include "types.h"

#include <locale>
#include <codecvt>
#include <stdio.h>

/* Features */
#include "Features/Local/Local.h"
#include "Features/Car/Car.h"
#include "Features/Gun/Gun.h"
#include "Features/Misc/Misc.h"
#include "Features/Other/Other.h"
#include "Features/Online/Online.h"
#include "Features/MenuFunctions.h"

extern std::atomic_bool g_running;