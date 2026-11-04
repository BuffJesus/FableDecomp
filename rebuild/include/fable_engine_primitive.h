#pragma once
#include "rebuild_abi.h"
// Original enum identities. Values outside the currently recovered operations
// remain opaque; VC7.1 gives these forward-declared enums their 32-bit ABI.
// Original values from Ego_r.pdb, cross-checked with retail clock dispatch.
enum EEnginePrimitiveType {
    ENGINE_PRIMITIVE_NULL = 0,
    ENGINE_PRIMITIVE_GROUP = 1,
    ENGINE_PRIMITIVE_CHANGER = 2,
    ENGINE_PRIMITIVE_FILLED_RECT = 3,
    ENGINE_PRIMITIVE_OUTLINE_RECT = 4,
    ENGINE_PRIMITIVE_TEXT = 5,
    ENGINE_PRIMITIVE_LINE = 6,
    ENGINE_PRIMITIVE_SPRITE = 7,
    ENGINE_PRIMITIVE_STATIC_MESH = 8,
    ENGINE_PRIMITIVE_ANIMATING_MESH = 9,
    ENGINE_PRIMITIVE_MORPHED_STATIC_MESH = 10,
    ENGINE_PRIMITIVE_MORPHED_ANIMATING_MESH = 11,
    ENGINE_PRIMITIVE_STATIC_MESH_FROM_FILE = 12,
    ENGINE_PRIMITIVE_ANIMATING_MESH_FROM_FILE = 13,
    ENGINE_PRIMITIVE_TARGET = 14,
    ENGINE_PRIMITIVE_LIGHT = 15,
    ENGINE_PRIMITIVE_SHADOWED_SPOT_LIGHT = 16,
    ENGINE_PRIMITIVE_FLARE_SPRITE = 17,
    ENGINE_PRIMITIVE_GAME_MESH = 18,
    ENGINE_PRIMITIVE_WEAPON_TRAIL = 19,
    ENGINE_PRIMITIVE_SPRITE_TRAIL = 20,
    ENGINE_PRIMITIVE_PARTICLE_LINE = 21,
    ENGINE_PRIMITIVE_PARTICLE_SPRITE = 22,
    ENGINE_PRIMITIVE_PARTICLE_MESH = 23,
    ENGINE_PRIMITIVE_PARTICLE_GROUP = 24,
    ENGINE_PRIMITIVE_PARTICLE_DECAL = 25,
    ENGINE_PRIMITIVE_PARTICLE_LINE_GROUP = 26,
    ENGINE_PRIMITIVE_PARTICLE_SPRITE_GROUP = 27,
    ENGINE_PRIMITIVE_PARTICLE_MESH_GROUP = 28,
    ENGINE_PRIMITIVE_PARTICLE_SPRITE_TRAIL_GROUP = 29,
    ENGINE_PRIMITIVE_PARTICLE_DECAL_GROUP = 30,
    ENGINE_PRIMITIVE_WEATHER_MASK = 31,
    ENGINE_PRIMITIVE_2D_PARTICLE_GROUP = 32,
    ENGINE_PRIMITIVE_2D_VIEWPORT = 33,
    ENGINE_PRIMITIVE_2D_SPRITE = 34,
    ENGINE_PRIMITIVE_2D_CLOCK_SPRITE = 35,
    ENGINE_PRIMITIVE_2D_STATIC_MESH = 36,
    ENGINE_PRIMITIVE_2D_ANIMATING_MESH = 37,
    ENGINE_PRIMITIVE_2D_MORPHED_ANIMATING_MESH = 38,
    ENGINE_PRIMITIVE_2D_TEXT = 39,
    ENGINE_PRIMITIVE_DECAL = 40,
    ENGINE_PRIMITIVE_MAX = 41
};
enum EEngineInterpolationSet;
class CEnginePrimitiveThemeChannelDesc
{
public:
    unsigned char ChannelIndex0,ChannelIndex1,ChannelBlendFactor,Pad;
};
class CEnginePrimitive
{
public:
    // Ego_r inline theme constructor; retail sprite constructors ignore the
    // theme identifier and clear three channels, preserving the Pad byte.
    CEnginePrimitive(EEnginePrimitiveType type,unsigned __int64,
        EEngineInterpolationSet interpolation)
        : Type(type),InterpolationSet(interpolation)
    {ThemeChannelDesc.ChannelIndex0=0;ThemeChannelDesc.ChannelIndex1=0;ThemeChannelDesc.ChannelBlendFactor=0;}
    EEnginePrimitiveType Type;
    EEngineInterpolationSet InterpolationSet;
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitive)==12);
