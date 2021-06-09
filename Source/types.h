#pragma once

using Void = std::uint32_t;
using Any = std::uint32_t;
using Hash = std::uint32_t;
using Entity = std::int32_t;
using Player = std::int32_t;
using FireId = std::int32_t;
using Ped = Entity;
using Vehicle = Entity;
using Cam = std::int32_t;
using CarGenerator = std::int32_t;
using Group = std::int32_t;
using Train = std::int32_t;
using Object = Entity;
using Pickup = Object;
using Weapon = std::int32_t;
using Interior = std::int32_t;
using Blip = std::int32_t;
using Texture = std::int32_t;
using TextureDict = std::int32_t;
using CoverPoint = std::int32_t;
using Camera = std::int32_t;
using TaskSequence = std::int32_t;
using ColourIndex = std::int32_t;
using Sphere = std::int32_t;
using ScrHandle = std::int32_t;

#pragma pack(push, 1)
typedef struct
{
	float x;
	DWORD _paddingx;
	float y;
	DWORD _paddingy;
	float z;
	DWORD _paddingz;
} Vector3;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct
{
	float x;
	float y;
	float z;
} Vector3_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct VECTOR2 
{
	float x, y;
};
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct VECTOR2_2 
{
	float w, h;
};
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct RGBAF 
{
	int r, g, b, a, f;
};
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct RGBA 
{
	int r, g, b, a;
};
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct RGB
{
	int r, g, b;
};
#pragma pack(pop)