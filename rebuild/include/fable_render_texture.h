#pragma once

#include "rebuild_abi.h"

struct FableTextureDevice;
typedef long (__stdcall *FableSetTextureCall)(
    FableTextureDevice* device,
    fable_u32 stage,
    void* texture);

struct FableTextureDevice
{
    void** vtable;
};

struct CTextureAttachView
{
    void* deviceTexture00;
};

struct CRenderManagerCoreAttachTextureView
{
    fable_u8 unknown0000[0x3CB8];
    fable_i32 activeTextureStageCount3CB8;
    fable_u8 unknown3CBC[0x34];
    FableTextureDevice* displayDevice3CF0;
    fable_u8 unknown3CF4[0x0C];
    void* attachedTextures3D00[16];

    void AttachTextureToStage(
        fable_i32 stage,
        const CTextureAttachView* texture);
};

FABLE_STATIC_ASSERT(
    sizeof(CRenderManagerCoreAttachTextureView) == 0x3D40);
