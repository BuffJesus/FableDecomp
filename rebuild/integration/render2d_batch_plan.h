#pragma once

#include "rebuild_abi.h"

struct FableRender2DVertex
{
    fable_u8 bytes[0x20];
};

struct FableRender2DWindow
{
    fable_u32 values[4];
};

struct FableRender2DNormalPayload
{
    fable_u32 stateBlock;
    fable_u8 reserved24[0x18];
};

struct FableRender2DTextPayload
{
    fable_u32 position[3];
    fable_u32 scale;
    fable_u32 reserved30;
    fable_u32 colour;
    fable_u32 fontBank;
};

union FableRender2DPayload
{
    FableRender2DNormalPayload normal;
    FableRender2DTextPayload text;
};

struct FableRender2DPlanRecord
{
    fable_u8 flags;
    fable_u8 reserved01[3];
    FableRender2DWindow window;
    fable_u32 wideStringStorage;
    fable_u32 textureIdentity;
    fable_u32 textureOwnership;
    FableRender2DPayload payload;
};

FABLE_STATIC_ASSERT(sizeof(FableRender2DVertex) == 0x20);
FABLE_STATIC_ASSERT(sizeof(FableRender2DWindow) == 0x10);
FABLE_STATIC_ASSERT(sizeof(FableRender2DPlanRecord) == 0x3C);

enum FableRender2DPlanEventKind
{
    FABLE_RENDER2D_PLAN_SET_WINDOW = 1,
    FABLE_RENDER2D_PLAN_REAPPLY_SHADER = 2,
    FABLE_RENDER2D_PLAN_FLUSH = 3,
    FABLE_RENDER2D_PLAN_BIND_TEXTURE = 4,
    FABLE_RENDER2D_PLAN_APPLY_STATE_BLOCK = 5,
    FABLE_RENDER2D_PLAN_SET_TOPOLOGY = 6,
    FABLE_RENDER2D_PLAN_SUSPEND_SHADERS = 7,
    FABLE_RENDER2D_PLAN_RENDER_TEXT = 8,
    FABLE_RENDER2D_PLAN_RESUME_SHADERS = 9
};

struct FableRender2DPlanEvent
{
    fable_u32 kind;
    fable_u32 argument0;
    fable_u32 argument1;
    fable_u32 argument2;
};

struct FableRender2DPlanOutput
{
    FableRender2DPlanEvent* events;
    fable_u32 capacity;
    fable_u32 count;
    bool overflow;
};

void FABLE_FASTCALL FableBuildRender2DBatchPlan(
    const FableRender2DPlanRecord* records,
    fable_u32 recordCount,
    FableRender2DPlanOutput& output);
