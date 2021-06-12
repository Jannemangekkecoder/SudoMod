#pragma once
#include "../../stdafx.h"

void Local::UpdateLoop()
{
	God(god);
}

bool Local::god = false;
void Local::God(bool enable)
{
	Memory::set_value<bool>({ 0x08, 0x189 }, enable);
}