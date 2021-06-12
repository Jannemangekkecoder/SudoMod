#pragma once 

namespace Online
{
	void UpdateLoop();

	/* Protections */
	extern bool event_logger;
	extern bool event_blocker;
	extern bool event_karma;

	inline std::uint32_t g_SelectedPlayer{};
}