#include <stdio.h>

#include "rebuild_abi.h"

struct IDirect3DDevice9;

typedef fable_i32 (__stdcall *D3DSetRenderState)(
    IDirect3DDevice9* device,
    fable_u32 state,
    fable_u32 value);
typedef fable_i32 (__stdcall *D3DSetTextureStageState)(
    IDirect3DDevice9* device,
    fable_u32 stage,
    fable_u32 state,
    fable_u32 value);
typedef fable_i32 (__stdcall *D3DSetSamplerState)(
    IDirect3DDevice9* device,
    fable_u32 sampler,
    fable_u32 state,
    fable_u32 value);

struct IDirect3DDevice9VTable
{
    void* methods000[57];
    D3DSetRenderState SetRenderState;
    void* methods0E8[9];
    D3DSetTextureStageState SetTextureStageState;
    void* method110;
    D3DSetSamplerState SetSamplerState;
};

struct IDirect3DDevice9
{
    IDirect3DDevice9VTable* vtable;
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
    IDirect3DDevice9* PD3DDevice0004;
    fable_u8 StateStack0008[0x2000];
    CRenderStateInfo* StateUpdateList2008[256];
    CRenderStateInfo* AllStatesList2408[256];
    fable_i32 StateStackSize2808;
    fable_i32 StateUpdateListSize280C;

    void RealiseRenderState();
};

struct ObservedStateCall
{
    fable_u32 kind;
    IDirect3DDevice9* device;
    fable_u32 stage;
    fable_u32 state;
    fable_u32 value;
};

static ObservedStateCall g_Calls[4];
static fable_i32 g_CallCount;

static fable_i32 __stdcall ObserveRenderState(
    IDirect3DDevice9* device,
    fable_u32 state,
    fable_u32 value)
{
    ObservedStateCall& call = g_Calls[g_CallCount++];
    call.kind = 1;
    call.device = device;
    call.stage = 0;
    call.state = state;
    call.value = value;
    return 0;
}

static fable_i32 __stdcall ObserveTextureStageState(
    IDirect3DDevice9* device,
    fable_u32 stage,
    fable_u32 state,
    fable_u32 value)
{
    ObservedStateCall& call = g_Calls[g_CallCount++];
    call.kind = 2;
    call.device = device;
    call.stage = stage;
    call.state = state;
    call.value = value;
    return 0;
}

static fable_i32 __stdcall ObserveSamplerState(
    IDirect3DDevice9* device,
    fable_u32 sampler,
    fable_u32 state,
    fable_u32 value)
{
    ObservedStateCall& call = g_Calls[g_CallCount++];
    call.kind = 3;
    call.device = device;
    call.stage = sampler;
    call.state = state;
    call.value = value;
    return 0;
}

static bool Matches(
    fable_i32 index,
    fable_u32 kind,
    IDirect3DDevice9* device,
    fable_u32 stage,
    fable_u32 state,
    fable_u32 value)
{
    const ObservedStateCall& call = g_Calls[index];
    return call.kind == kind &&
        call.device == device &&
        call.stage == stage &&
        call.state == state &&
        call.value == value;
}

int main()
{
    IDirect3DDevice9VTable deviceVtable = {};
    deviceVtable.SetRenderState = ObserveRenderState;
    deviceVtable.SetTextureStageState = ObserveTextureStageState;
    deviceVtable.SetSamplerState = ObserveSamplerState;
    IDirect3DDevice9 device = {&deviceVtable};

    CRenderStateInfo unchanged = {4, 4, 0, 10, 1, 1, 0, 0};
    CRenderStateInfo render = {1, 2, 0, 20, 1, 1, 0, 0};
    CRenderStateInfo texture = {3, 4, 0, 30, 1, 2, 5, 0};
    CRenderStateInfo sampler = {5, 6, 0, 40, 1, 3, 7, 0};
    CRenderStateInfo unsupported = {7, 8, 0, 50, 1, 4, 9, 0};

    CRenderStateManagerRealiseView manager = {};
    manager.PD3DDevice0004 = &device;
    manager.StateUpdateList2008[0] = &unchanged;
    manager.StateUpdateList2008[1] = &render;
    manager.StateUpdateList2008[2] = &texture;
    manager.StateUpdateList2008[3] = &sampler;
    manager.StateUpdateList2008[4] = &unsupported;
    manager.StateUpdateListSize280C = 5;

    manager.RealiseRenderState();

    if (manager.StateUpdateListSize280C != 0 ||
        unchanged.DirtyListFlag != 0 ||
        render.DirtyListFlag != 0 ||
        texture.DirtyListFlag != 0 ||
        sampler.DirtyListFlag != 0 ||
        unsupported.DirtyListFlag != 0 ||
        unchanged.CurrentState != 4 ||
        render.CurrentState != 2 ||
        texture.CurrentState != 4 ||
        sampler.CurrentState != 6 ||
        unsupported.CurrentState != 8 ||
        g_CallCount != 3 ||
        !Matches(0, 1, &device, 0, 20, 2) ||
        !Matches(1, 2, &device, 5, 30, 4) ||
        !Matches(2, 3, &device, 7, 40, 6))
    {
        printf("FABLETLC_REALISE_RENDER_STATE_BEHAVIOR FAIL code=1\n");
        return 1;
    }

    manager.StateUpdateListSize280C = -2;
    manager.RealiseRenderState();
    if (manager.StateUpdateListSize280C != 0 || g_CallCount != 3)
    {
        printf("FABLETLC_REALISE_RENDER_STATE_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    printf("FABLETLC_REALISE_RENDER_STATE_BEHAVIOR PASS\n");
    return 0;
}
