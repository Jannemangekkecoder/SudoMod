#pragma once
#include "../../stdafx.h"

namespace MenuFunctions
{
	void SpawnVehicle(const char* carname)
	{
		Hash hash = MISC::GET_HASH_KEY(carname);

		STREAMING::REQUEST_MODEL(hash);
		while (!STREAMING::HAS_MODEL_LOADED(hash)) WAIT(0);

		auto pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
		*(unsigned short*)hooks.m_model_spawn_bypass = 0x9090;
		Vehicle vehicle = VEHICLE::CREATE_VEHICLE(hash, pos.x, pos.y, pos.z, 0.f, TRUE, FALSE, FALSE);
		*(unsigned short*)hooks.m_model_spawn_bypass = 0x0574;

		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
		if (NETWORK::NETWORK_IS_SESSION_STARTED())
		{
			DECORATOR::DECOR_SET_INT(vehicle, "MPBitset", 0);
			ENTITY::_SET_ENTITY_SOMETHING(vehicle, TRUE); // True means it can be deleted by the engine when switching lobbies etc, false means the script is expected to clean it up.
			auto networkId = NETWORK::VEH_TO_NET(vehicle);
			if (NETWORK::NETWORK_GET_ENTITY_IS_NETWORKED(vehicle)) { NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(networkId, true); }
			VEHICLE::SET_VEHICLE_IS_STOLEN(vehicle, FALSE);
		}
	}
}