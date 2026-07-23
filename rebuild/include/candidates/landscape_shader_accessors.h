#ifndef FABLETLC_LANDSCAPE_SHADER_ACCESSORS_H
#define FABLETLC_LANDSCAPE_SHADER_ACCESSORS_H

#include <stddef.h>
#include "rebuild_abi.h"

class CVertexShader
{
public:
    fable_u32 words[2];
};

struct CVertexShaderArray
{
    volatile long begin;
    volatile long end;
};

class CEngineLandscapeRenderer
{
public:
    fable_u8 pad_0000[0x4DC];
    volatile long foreground_begin;
    volatile long foreground_end;
    fable_u8 pad_04e4[4];
    volatile long foreground_spot_begin;
    volatile long foreground_spot_end;
    fable_u8 pad_04f0[0x54];
    CVertexShaderArray* foreground_shadowed;
    CVertexShaderArray* foreground_bump_shadowed;
    CVertexShaderArray* foreground_spot_shadowed;

    CVertexShader& GetForegroundVertexShader(long index);
    CVertexShader& GetForegroundSpotLightVertexShader(long index);
    CVertexShader& GetForegroundShadowedVertexShader(long index);
    CVertexShader& GetForegroundBumpShadowedVertexShader(long index);
    CVertexShader& GetForegroundSpotShadowedVertexShader(long index);
    unsigned long PeekSceneFilterFlags() const;
};

FABLE_STATIC_ASSERT(sizeof(CVertexShader) == 8);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, foreground_begin) == 0x4DC);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, foreground_spot_begin) == 0x4E8);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, foreground_shadowed) == 0x544);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, foreground_bump_shadowed) == 0x548);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, foreground_spot_shadowed) == 0x54C);

#endif
