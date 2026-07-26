#pragma once

#include "rebuild_abi.h"

class CShaderRenderManager
{
public:
    fable_u8 unknown000[0x3D8];
    fable_u32 updateFlags3D8;

    void OnPostViewportChanged();
};

FABLE_STATIC_ASSERT(sizeof(CShaderRenderManager) == 0x3DC);
