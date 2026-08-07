#pragma once

#include "rebuild_abi.h"
#include "fable_visual_boot.h"
#include "fable_render2d_vertex_queue.h"

struct FableRender2DVertex
{
    fable_u8 bytes[0x20];
};

struct FableRender2DSpriteVertex
{
    float x;
    float y;
    float z;
    float rhw;
    fable_u32 diffuseColour;
    fable_u32 specularColour;
    float u;
    float v;
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
FABLE_STATIC_ASSERT(sizeof(FableRender2DSpriteVertex) == 0x20);
FABLE_STATIC_ASSERT(sizeof(FableRender2DWindow) == 0x10);
FABLE_STATIC_ASSERT(sizeof(FableRender2DPlanRecord) == 0x3C);

struct FableUiRender2DBinding
{
    unsigned long definitionId;
    fable_u32 textureIdentity;
    float u0;
    float v0;
    float u1;
    float v1;
    fable_u32 diffuseColour;
};

struct FableUiRender2DAppendTarget
{
    FableRender2DSpriteVertex* vertices;
    fable_u32 vertexCapacity;
    fable_u32* vertexCount;
    FableRender2DPlanRecord* records;
    fable_u32 recordCapacity;
    fable_u32* recordCount;
    const FableUiVector2* clipMinimum;
    const FableUiVector2* clipMaximum;
};

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

// Consumes the native CQuickDrawTriInfo vector without converting it into a
// second authored record format. The layout is proven by
// Render2DQuickDrawInfoView's static assertions and the recovered
// CRenderManager2D queue offsets.
bool FABLE_FASTCALL FableBuildRender2DBatchPlanFromNativeQueue(
    const CRenderManager2DQueueView* manager,
    FableRender2DPlanOutput& output);

// Adapts live generated CTable children into the normal Render2D vertex and
// record queues. Definition identity selects the retail sprite texture/UV
// binding; state-zero position, size, and zoom produce each final quad.
// Capacity and binding validation are transactional: failure leaves both
// queue counts unchanged.
bool FABLE_FASTCALL FableAppendUiGeneratedComponentsToRender2D(
    const FableUiGeneratedComponentVector* generated,
    const FableUiRender2DBinding* bindings,
    fable_u32 bindingCount,
    float parentX,
    float parentY,
    float scaleX,
    float scaleY,
    FableUiRender2DAppendTarget* target);
