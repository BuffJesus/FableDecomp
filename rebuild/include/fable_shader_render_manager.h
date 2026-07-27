#pragma once

#include "rebuild_abi.h"

struct FablePixelShaderDevice
{
    void** vtable;
};

struct CCountedPixelShaderView
{
    void* unknown00;
    void* pixelShader04;
};

struct CShaderRenderManagerUpdatePixelShaderView
{
    fable_u8 unknown000[0x1D4];
    FablePixelShaderDevice* displayDevice1D4;
    fable_u8 unknown1D8[0x0C];
    CCountedPixelShaderView* currentPixelShader1E4;
    fable_u8 unknown1E8[0x1F0];
    fable_u32 shaderStateMask3D8;

    void UpdatePixelShader();
};

class CShaderRenderManager
{
public:
    fable_u8 unknown000[0x3D8];
    fable_u32 updateFlags3D8;

    void OnPostViewportChanged();
};

FABLE_STATIC_ASSERT(
    sizeof(CShaderRenderManagerUpdatePixelShaderView) == 0x3DC);
FABLE_STATIC_ASSERT(sizeof(CShaderRenderManager) == 0x3DC);
