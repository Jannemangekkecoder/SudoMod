//Hooking.cpp
#pragma once
#include "stdafx.h"


using namespace Memory;

ScriptThread* (*GetActiveThread)() = nullptr;
HMODULE _hmoduleDLL;
HANDLE mainFiber;
DWORD wakeAt;

std::vector<LPVOID> Hooking::m_hooks;
uint64_t* Hooking::m_framecount;
GetNumberOfEvents* Hooking::m_get_number_of_events;
GetLabelText* Hooking::m_get_label_text;
GetEventData* Hooking::m_get_event_data;
RegisterFile* Hooking::m_register_file;
TriggerScriptEvent* Hooking::m_trigger_script_event;
PVOID Hooking::m_model_spawn_bypass;
void* Hooking::m_native_return;
eGameState* Hooking::m_gamestate;
uint64_t Hooking::m_world_pointer;
__int64** Hooking::m_global_pointer;
static Hooking::NativeRegistrationNew** m_registrationTable;
static std::unordered_map<uint64_t, Hooking::NativeHandler>	m_handlerCache;
static std::vector<LPVOID> m_hookedNative;

const int EVENT_COUNT = 78;
static std::vector<void*> EventPtr;
static char EventRestore[EVENT_COUNT] = {};


/* Start Hooking */
void Hooking::Start(HMODULE hmoduleDLL)
{
	_hmoduleDLL = hmoduleDLL;
	FindPatterns();
	if (!InitializeHooks()) Cleanup();
}
BOOL Hooking::InitializeHooks()
{
	BOOL returnVal = TRUE;

	if (!iHook.Initialize()) {

		Log_Error("Failed to initialize InputHook");
		returnVal = FALSE;
	}

	if (!HookNatives()) {

		Log_Error("Failed to initialize NativeHooks");
		returnVal = FALSE;
	}

	return returnVal;
}

uint64_t CMetaData::m_begin = 0;
uint64_t CMetaData::m_end = 0;
DWORD CMetaData::m_size = 0;

uint64_t CMetaData::begin()
{
	return m_begin;
}
uint64_t CMetaData::end()
{
	return m_end;
}
DWORD CMetaData::size()
{
	return m_size;
}

void CMetaData::init()
{
	if (m_begin && m_size)
		return;

	m_begin = (uint64_t)GetModuleHandleA(nullptr);
	const IMAGE_DOS_HEADER* headerDos = (const IMAGE_DOS_HEADER*)m_begin;
	const IMAGE_NT_HEADERS* headerNt = (const IMAGE_NT_HEADERS64*)((const BYTE*)headerDos + headerDos->e_lfanew);
	m_size = headerNt->OptionalHeader.SizeOfCode;
	m_end = m_begin + m_size;
	return;
}

GetNumberOfEvents* OG_GET_NUMBER_OF_EVENTS = nullptr;
int32_t HK_GET_NUMBER_OF_EVENTS(int32_t unk)
{
	static uint64_t	last_frame = 0;
	if (last_frame != *Hooking::m_framecount)
	{
		last_frame = *Hooking::m_framecount;
		Hooking::onTickInit();
	}
	return OG_GET_NUMBER_OF_EVENTS(unk);
}

GetLabelText* OG_GET_LABEL_TEXT = nullptr;
const char* HK_GET_LABEL_TEXT(void* _this, const char* label)
{
	if (std::strcmp(label, "HUD_JOINING") == 0) return "Isn't SudoMod the fucking best?";
	if (std::strcmp(label, "HUD_TRANSP") == 0) return "Isn't SudoMod the fucking best?";
	return OG_GET_LABEL_TEXT(_this, label);
}

GetEventData* OG_GET_EVENT_DATA = nullptr;
bool HK_GET_EVENT_DATA(std::int32_t eventGroup, std::int32_t eventIndex, std::int64_t* args, std::uint32_t argCount)
{
	auto result = OG_GET_EVENT_DATA(eventGroup, eventIndex, args, argCount);
	if (result && Online::event_logger)
	{
		Log_Info("===============================");
		std::string evgroup = std::string("Script event group: ") + std::to_string(eventGroup).c_str();
		Log_Info(_strdup(evgroup.c_str()));
		std::string evindex = std::string("Script event index: ") + std::to_string(eventIndex).c_str();
		Log_Info(_strdup(evindex.c_str()));
		std::string evargcount = std::string("Script event argcount: ") + std::to_string(argCount).c_str();
		Log_Info(_strdup(evargcount.c_str()));
		for (std::uint32_t i = 0; i < argCount; ++i) {
			std::string evargs = std::string("Script event args[") + std::to_string(i).c_str() + "] : " + std::to_string(args[i]).c_str();
			Log_Info(_strdup(evargs.c_str()));
		}
	}
	if (result && Online::event_blocker)
	{
		if (args[0] == -738295409) /* CEO BAN */
		{
			Log_Info("SudoMod || Script Event: CEO Ban");
			Log_Info("SudoMod || Status: Blocked");
			std::string sender = std::string("SudoMod || Sender: ") + PLAYER::GET_PLAYER_NAME(args[1]);
			Log_Info(_strdup(sender.c_str()));
			if (Online::event_karma)
			{
				Log_Info("SudoMod || Karma enabled, redirecting event.");
				uint64_t tseargs[3] = { -738295409, args[1], 1 };
				hooks.m_trigger_script_event(1, tseargs, 3, 1 << args[1]);
			}
		}
		return false;
	}
	return result;
}

bool Hooking::HookNatives()
{
	/* Main Hook: GNOE */
	MH_STATUS status = MH_CreateHook(Hooking::m_get_number_of_events, HK_GET_NUMBER_OF_EVENTS, (void**)&OG_GET_NUMBER_OF_EVENTS);
	if ((status != MH_OK && status != MH_ERROR_ALREADY_CREATED) || MH_EnableHook(Hooking::m_get_number_of_events) != MH_OK)
		return false;
	Hooking::m_hooks.push_back(Hooking::m_get_number_of_events);

	/* Get Label Text - Used for adding custom text to game labels */

	status = MH_CreateHook(Hooking::m_get_label_text, HK_GET_LABEL_TEXT, (void**)&OG_GET_LABEL_TEXT);
	if (status != MH_OK || MH_EnableHook(Hooking::m_get_label_text) != MH_OK)
		return false;
	Hooking::m_hooks.push_back(Hooking::m_get_label_text);

	/* Get Event Data - Used for Event logging or event blocking */
	status = MH_CreateHook(Hooking::m_get_event_data, HK_GET_EVENT_DATA, (void**)&OG_GET_EVENT_DATA);
	if (status != MH_OK || MH_EnableHook(Hooking::m_get_event_data) != MH_OK)
		return false;
	Hooking::m_hooks.push_back(Hooking::m_get_event_data);

	return true;
}

void __stdcall ScriptFunction(LPVOID lpParameter)
{
	try
	{
		ScriptMain();

	}
	catch (...)
	{
		Log_Error("Failed scriptFiber");
	}
}

void Hooking::onTickInit()
{
	if (mainFiber == nullptr)
		mainFiber = IsThreadAFiber() ? GetCurrentFiber() : ConvertThreadToFiber(nullptr);

	if (timeGetTime() < wakeAt)
		return;

	static HANDLE scriptFiber;
	if (scriptFiber)
		SwitchToFiber(scriptFiber);
	else
		scriptFiber = CreateFiber(NULL, ScriptFunction, nullptr);
}

void Hooking::FailPatterns(const char* name)
{
	Log_Error("Finding %s", name);
	Cleanup();
}

/*
//CPatternResult
*/

CPatternResult::CPatternResult(void* pVoid) :
	m_pVoid(pVoid)
{}
CPatternResult::CPatternResult(void* pVoid, void* pBegin, void* pEnd) :
	m_pVoid(pVoid),
	m_pBegin(pBegin),
	m_pEnd(pEnd)
{}
CPatternResult::~CPatternResult() {}

/*
//CPattern Public
*/

CPattern::CPattern(char* szByte, char* szMask) :
	m_szByte(szByte),
	m_szMask(szMask),
	m_bSet(false)
{}
CPattern::~CPattern() {}

CPattern& CPattern::find(int i, uint64_t startAddress)
{
	match(i, startAddress, false);
	if (m_result.size() <= i)
		m_result.push_back(nullptr);
	return *this;
}

CPattern& CPattern::virtual_find(int i, uint64_t startAddress)
{
	match(i, startAddress, true);
	if (m_result.size() <= i)
		m_result.push_back(nullptr);
	return *this;
}

CPatternResult	CPattern::get(int i)
{
	if (m_result.size() > i)
		return m_result[i];
	return nullptr;
}

/*
//CPattern Private
*/
bool	CPattern::match(int i, uint64_t startAddress, bool virt)
{
	if (m_bSet)
		return false;
	uint64_t	begin = 0;
	uint64_t	end = 0;
	if (!virt)
	{
		CMetaData::init();
		begin = CMetaData::begin() + startAddress;
		end = CMetaData::end();
		if (begin >= end)
			return false;
	}
	int		j = 0;
	do
	{
		if (virt)
			begin = virtual_find_pattern(startAddress, (BYTE*)m_szByte, m_szMask) + 1;
		else
			begin = find_pattern(begin, end, (BYTE*)m_szByte, m_szMask) + 1;
		if (begin == NULL)
			break;
		j++;
	} while (j < i);

	m_bSet = true;
	return true;
}

bool	CPattern::byte_compare(const BYTE* pData, const BYTE* btMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++btMask)
		if (*szMask == 'x' && *pData != *btMask)
			break;
	if ((*szMask) != 0)
		return false;
	return true;
}


uint64_t	CPattern::find_pattern(uint64_t address, uint64_t end, BYTE* btMask, char* szMask)
{
	size_t len = strlen(szMask) + 1;
	for (uint64_t i = 0; i < (end - address - len); i++)
	{
		BYTE* ptr = (BYTE*)(address + i);
		if (byte_compare(ptr, btMask, szMask))
		{
			m_result.push_back(CPatternResult((void*)(address + i)));
			return address + i;
		}
	}
	return NULL;
}

uint64_t	CPattern::virtual_find_pattern(uint64_t address, BYTE* btMask, char* szMask)
{
	MEMORY_BASIC_INFORMATION mbi;
	char* pStart = nullptr;
	char* pEnd = nullptr;
	char* res = nullptr;
	size_t	maskLen = strlen(szMask);

	while (res == nullptr && sizeof(mbi) == VirtualQuery(pEnd, &mbi, sizeof(mbi)))
	{
		pStart = pEnd;
		pEnd += mbi.RegionSize;
		if (mbi.Protect != PAGE_READWRITE || mbi.State != MEM_COMMIT)
			continue;

		for (int i = 0; pStart < pEnd - maskLen && res == nullptr; ++pStart)
		{
			if (byte_compare((BYTE*)pStart, btMask, szMask))
			{
				m_result.push_back(CPatternResult((void*)pStart, mbi.BaseAddress, pEnd));
				res = pStart;
			}
		}

		mbi = {};
	}
	return (uint64_t)res;
}

void	failPat(const char* name)
{
	Log_Error("Failed to find %s pattern.", name);
}

template <typename T>
void	setPat
(
	const char* name,
	char* pat,
	char* mask,
	T** out,
	bool		rel,
	int			offset = 0,
	int			deref = 0,
	int			skip = 0
)
{
	T* ptr = nullptr;

	CPattern pattern(pat, mask);
	pattern.find(1 + skip);
	if (rel)
		ptr = pattern.get(skip).get_rel<T>(offset);
	else
		ptr = pattern.get(skip).get<T>(offset);

	while (true)
	{
		if (ptr == nullptr)
			failPat(name);

		if (deref <= 0)
			break;
		ptr = *(T**)ptr;
		--deref;
	}

	*out = ptr;
	return;
}

template <typename T>
void	setFn
(
	const char* name,
	char* pat,
	char* mask,
	T* out,
	int			skip = 0
)
{
	char* ptr = nullptr;

	CPattern pattern(pat, mask);
	pattern.find(1 + skip);
	ptr = pattern.get(skip).get<char>(0);

	if (ptr == nullptr)
		failPat(name);

	*out = (T)ptr;
	return;
}

void Hooking::FindPatterns()
{
	char* c_location = nullptr;
	void* v_location = nullptr;

	Log_Info("[Pattern Scanner] Found and Hooked Frame Count");
	c_location = Memory::pattern("F3 0F 10 0D ? ? ? ? 44 89 6B 08").count(1).get(0).get<char>(4);
	c_location == nullptr ? FailPatterns("Frame Count") : Hooking::m_framecount = reinterpret_cast<uint64_t*>((c_location + *reinterpret_cast<int*>(c_location) + 4) - 8);

	Log_Info("[Pattern Scanner] Found and Hooked Get Number Of Events");
	c_location = Memory::pattern("48 83 EC 28 33 D2 85 C9").count(1).get(0).get<char>(0);
	c_location == nullptr ? FailPatterns("Get Number Of Events") : Hooking::m_get_number_of_events = reinterpret_cast<decltype(Hooking::m_get_number_of_events)>(c_location);

	Log_Info("[Pattern Scanner] Found and Hooked Get Label Text");
	c_location = Memory::pattern("48 89 5C 24 ? 57 48 83 EC 20 48 8B DA 48 8B F9 48 85 D2 75 44 E8").count(1).get(0).get<char>(0);
	c_location == nullptr ? FailPatterns("Get Label Text") : Hooking::m_get_label_text = reinterpret_cast<decltype(Hooking::m_get_label_text)>(c_location);

	Log_Info("[Pattern Scanner] Found and Hooked Get Event Data");
	c_location = Memory::pattern("48 85 C0 74 14 4C 8B 10").count(1).get(0).get<char>(-28);
	c_location == nullptr ? FailPatterns("Get Event Data") : Hooking::m_get_event_data = reinterpret_cast<decltype(Hooking::m_get_event_data)>(c_location);

	Log_Info("[Pattern Scanner] Found Register File");
	c_location = Memory::pattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 7C 24 ? 41 54 41 56 41 57 48 83 EC 50 48 8B EA 4C 8B FA 48 8B D9 4D 85 C9").count(1).get(0).get<char>(0);
	c_location == nullptr ? FailPatterns("Register File") : Hooking::m_register_file = reinterpret_cast<decltype(Hooking::m_register_file)>(c_location);

	Log_Info("[Pattern Scanner] Found Trigger Script Event");
	c_location = Memory::pattern("48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 56 48 81 EC ? ? ? ? 45 8B F0 41 8B F9").count(1).get(0).get<char>(0);
	c_location == nullptr ? FailPatterns("Trigger Script Event") : Hooking::m_trigger_script_event = reinterpret_cast<decltype(Hooking::m_trigger_script_event)>(c_location);

	Log_Info("[Pattern Scanner] Found Game State");
	c_location = Memory::pattern("83 3D ? ? ? ? ? 75 17 8B 42 20 25").count(1).get(0).get<char>(2);
	c_location == nullptr ? FailPatterns("Game State") : Hooking::m_gamestate = reinterpret_cast<decltype(Hooking::m_gamestate)>(c_location + *(int32_t*)c_location + 5);

	Log_Info("[Pattern Scanner] Found Vector3 Function");
	v_location = Memory::pattern("83 79 18 00 48 8B D1 74 4A FF 4A 18").count(1).get(0).get<void>(0);
	if (v_location != nullptr) scrNativeCallContext::SetVectorResults = (void(*)(scrNativeCallContext*))(v_location);

	Log_Info("[Pattern Scanner] Found Native Registration Table");
	c_location = Memory::pattern("76 32 48 8B 53 40 48 8D 0D").count(1).get(0).get<char>(9);
	c_location == nullptr ? FailPatterns("Native Registration Table") : m_registrationTable = reinterpret_cast<decltype(m_registrationTable)>(c_location + *(int32_t*)c_location + 4);

	Log_Info("[Pattern Scanner] Found Model Spawn Bypass");
	c_location = Memory::pattern("48 8B C8 FF 52 30 84 C0 74 05 48").count(1).get(0).get<char>(8);
	c_location == nullptr ? FailPatterns("Model Spawn Bypass") : Hooking::m_model_spawn_bypass = reinterpret_cast<decltype(Hooking::m_model_spawn_bypass)>(c_location);

	Log_Info("[Pattern Scanner] Found Native Return Adress");
	c_location = Memory::pattern("FF E3").count(1).get(0).get<char>(0);
	c_location == nullptr ? FailPatterns("Native Return Adress") : Hooking::m_native_return = reinterpret_cast<decltype(Hooking::m_native_return)>(c_location);

	Log_Info("[Pattern Scanner] Found World Pointer");
	c_location = Memory::pattern("48 8B 05 ? ? ? ? 45 ? ? ? ? 48 8B 48 08 48 85 C9 74 07").count(1).get(0).get<char>(0);
	c_location == nullptr ? FailPatterns("World Pointer") : Hooking::m_world_pointer = reinterpret_cast<uint64_t>(c_location) + *reinterpret_cast<int*>(reinterpret_cast<uint64_t>(c_location) + 3) + 7;

	Log_Info("[Pattern Scanner] Found Active Script Thread");
	c_location = Memory::pattern("E8 ? ? ? ? 48 8B 88 10 01 00 00").count(1).get(0).get<char>(1);
	c_location == nullptr ? FailPatterns("Active Script Thread") : GetActiveThread = reinterpret_cast<decltype(GetActiveThread)>(c_location + *(int32_t*)c_location + 4);

	Log_Info("[Pattern Scanner] Found Global Pointer");
	c_location = Memory::pattern("4C 8D 05 ? ? ? ? 4D 8B 08 4D 85 C9 74 11").count(1).get(0).get<char>(0);
	__int64 patternAddr = NULL;
	c_location == nullptr ? FailPatterns("Global Pointer") : patternAddr = reinterpret_cast<decltype(patternAddr)>(c_location);
	Hooking::m_global_pointer = (__int64**)(patternAddr + *(int*)(patternAddr + 3) + 7);

	Log_Info("[Pattern Scanner] Found Event Hook");
	if ((c_location = Memory::pattern("48 83 EC 28 E8 ? ? ? ? 48 8B 0D ? ? ? ? 4C 8D 0D ? ? ? ? 4C 8D 05 ? ? ? ? BA 03").count(1).get(0).get<char>(0)))
	{
		int i = 0, j = 0, matches = 0, found = 0;
		char* pattern = "\x4C\x8D\x05";
		while (found != EVENT_COUNT)
		{
			if (c_location[i] == pattern[j])
			{
				if (++matches == 3)
				{
					EventPtr.push_back((void*)(reinterpret_cast<uint64_t>(c_location + i - j) + *reinterpret_cast<int*>(c_location + i + 1) + 7));
					found++;
					j = matches = 0;
				}
				j++;
			}
			else
			{
				matches = j = 0;
			}
			i++;
		}
	}

	Log_Info("[Hooking] Loading CrossMap");
	CrossMapping::initNativeMap();

	while (*Hooking::m_gamestate != GameStatePlaying) { // Game State Check
		Sleep(200);
	}
}

static Hooking::NativeHandler _Handler(uint64_t origHash)
{
	uint64_t newHash = CrossMapping::MapNative(origHash);
	if (newHash == 0)
	{
		return nullptr;
	}

	Hooking::NativeRegistrationNew* table = m_registrationTable[newHash & 0xFF];

	for (; table; table = table->getNextRegistration())
	{
		for (uint32_t i = 0; i < table->getNumEntries(); i++)
		{
			if (newHash == table->getHash(i))
			{
				return table->handlers[i];
			}
		}
	}
	return nullptr;
}

Hooking::NativeHandler Hooking::GetNativeHandler(uint64_t origHash)
{
	auto& handler = m_handlerCache[origHash];

	if (handler == nullptr)
	{
		handler = _Handler(origHash);
	}

	return handler;
}

void WAIT(DWORD ms)
{
	wakeAt = timeGetTime() + ms;
	SwitchToFiber(mainFiber);
}

void __declspec(noreturn) Hooking::Cleanup()
{
	iHook.Remove();

	Log_Info("Cleaning up hooks");
	for (auto func : m_hooks)
	{
		MH_STATUS status;
		if ((status = MH_DisableHook(func)) == MH_OK)
		{
			Log_Info("Successfully disabled hook %p", func);
		}
		else
		{
			Log_Info("Failed to disable hook %p (%s)", func, MH_StatusToString(status));
		}
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	for (auto func : m_hooks)
	{
		MH_STATUS status;
		if ((status = MH_RemoveHook(func)) == MH_OK)
		{
			Log_Info("Successfully removed hook %p", func);
		}
		else
		{
			Log_Info("Failed to remove hook %p (%s)", func, MH_StatusToString(status));
		}
	}

	fclose(stdout);
	FreeConsole();
	FreeLibraryAndExitThread(static_cast<HMODULE>(_hmoduleDLL), 0);
}

void Hooking::defuseEvent(RockstarEvent e, bool toggle)
{
	static const unsigned char retn = 0xC3;
	char* p = (char*)EventPtr[e];
	if (toggle)
	{
		if (EventRestore[e] == 0)
			EventRestore[e] = p[0];
		*p = retn;
	}
	else
	{
		if (EventRestore[e] != 0)
			*p = EventRestore[e];
	}
}

MinHookKeepalive::MinHookKeepalive()
{
	MH_Initialize();
}

MinHookKeepalive::~MinHookKeepalive()
{
	MH_Uninitialize();
}

MinHookKeepalive g_MinHookKeepalive;
