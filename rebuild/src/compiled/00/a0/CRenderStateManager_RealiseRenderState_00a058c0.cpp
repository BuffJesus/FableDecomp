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
    D3DSetRenderState SetRenderState;          // slot 57, +0x0e4
    void* methods0E8[9];
    D3DSetTextureStageState SetTextureStageState; // slot 67, +0x10c
    void* method110;
    D3DSetSamplerState SetSamplerState;        // slot 69, +0x114
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

void CRenderStateManagerRealiseView::RealiseRenderState()
{
    for (fable_i32 index = 0; index < StateUpdateListSize280C; ++index)
    {
        CRenderStateInfo* state = StateUpdateList2008[index];
        fable_u32 desired = state->DesiredState;
        state->DirtyListFlag = 0;
        if (desired != state->CurrentState)
        {
            switch (state->StateType)
            {
            case 1:
                PD3DDevice0004->vtable->SetRenderState(
                    PD3DDevice0004,
                    state->StateParam,
                    desired);
                break;
            case 2:
                PD3DDevice0004->vtable->SetTextureStageState(
                    PD3DDevice0004,
                    state->TextureStage,
                    state->StateParam,
                    desired);
                break;
            case 3:
                PD3DDevice0004->vtable->SetSamplerState(
                    PD3DDevice0004,
                    state->TextureStage,
                    state->StateParam,
                    desired);
                break;
            }
            state->CurrentState = state->DesiredState;
        }
    }
    StateUpdateListSize280C = 0;
}
