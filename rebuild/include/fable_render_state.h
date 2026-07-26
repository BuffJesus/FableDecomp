#pragma once

#include "rebuild_abi.h"

struct FableRenderStateDevice;

typedef fable_i32 (__stdcall *FableD3DSetRenderState)(
    FableRenderStateDevice* device,
    fable_u32 state,
    fable_u32 value);
typedef fable_i32 (__stdcall *FableD3DSetTextureStageState)(
    FableRenderStateDevice* device,
    fable_u32 stage,
    fable_u32 state,
    fable_u32 value);
typedef fable_i32 (__stdcall *FableD3DSetSamplerState)(
    FableRenderStateDevice* device,
    fable_u32 sampler,
    fable_u32 state,
    fable_u32 value);

struct FableRenderStateDeviceVTable
{
    void* methods000[57];
    FableD3DSetRenderState SetRenderState;
    void* methods0E8[9];
    FableD3DSetTextureStageState SetTextureStageState;
    void* method110;
    FableD3DSetSamplerState SetSamplerState;
};

struct FableRenderStateDevice
{
    FableRenderStateDeviceVTable* vtable;
};

struct CRenderStateInfo
{
    fable_u32 CurrentState;
    fable_u32 DesiredState;
    fable_u32 BookmarkMask;
    fable_u32 StateParam;
    fable_u8 DirtyListFlag;
    fable_u8 StateType;
    fable_u8 TextureStage;
    fable_u8 padding13;
};

struct CRenderStateManagerRealiseView
{
    void* vtable0000;
    FableRenderStateDevice* PD3DDevice0004;
    fable_u8 StateStack0008[0x2000];
    CRenderStateInfo* StateUpdateList2008[256];
    CRenderStateInfo* AllStatesList2408[256];
    fable_i32 StateStackSize2808;
    fable_i32 StateUpdateListSize280C;

    void RealiseRenderState();
};

FABLE_STATIC_ASSERT(sizeof(CRenderStateInfo) == 0x14);
FABLE_STATIC_ASSERT(sizeof(CRenderStateManagerRealiseView) == 0x2810);
