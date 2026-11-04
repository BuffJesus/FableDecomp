#pragma once

#include "rebuild_abi.h"
#include "fable_render_state_manager.h"

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
    void* methods0E8[7];
    fable_i32 (__stdcall *SetTexture)(FableRenderStateDevice*,unsigned long,IDirect3DBaseTexture9*); // slot65
    void* method108;
    FableD3DSetTextureStageState SetTextureStageState;
    void* method110;
    FableD3DSetSamplerState SetSamplerState;
    void* methods118[30];
    // IDirect3DDevice9::SetStreamSource, SDK slot100 / +0x190.
    fable_i32 (__stdcall *SetStreamSource)(FableRenderStateDevice*,
        unsigned int, IDirect3DVertexBuffer9*, unsigned int, unsigned int);
    void* methods194[3];
    fable_i32 (__stdcall *SetIndices)(FableRenderStateDevice*,IDirect3DIndexBuffer9*); // slot104
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

    // Bootstrap storage adapter; native member is owned by the shared manager.
    void RealiseRenderState(){reinterpret_cast<CRenderStateManager*>(this)->RealiseRenderState();}
};

FABLE_STATIC_ASSERT(sizeof(CRenderStateInfo) == 0x14);
FABLE_STATIC_ASSERT(sizeof(CRenderStateManagerRealiseView) == 0x2810);
