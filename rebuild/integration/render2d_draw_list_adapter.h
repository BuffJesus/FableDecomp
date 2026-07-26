#pragma once

#include "rebuild_abi.h"

enum Render2DAdapterEventKind
{
    RENDER2D_ADAPTER_ENABLE_VERTEX_SHADERS = 1,
    RENDER2D_ADAPTER_DISABLE_VERTEX_SHADERS = 2,
    RENDER2D_ADAPTER_ENABLE_PIXEL_SHADERS = 3,
    RENDER2D_ADAPTER_DISABLE_PIXEL_SHADERS = 4,
    RENDER2D_ADAPTER_BEGIN_CAPTURE = 5,
    RENDER2D_ADAPTER_REQUEST_TRACKED_STATE = 6,
    RENDER2D_ADAPTER_INITIALISE_NULL_TEXTURE = 7,
    RENDER2D_ADAPTER_ASSIGN_TEXTURE = 8,
    RENDER2D_ADAPTER_UNINITIALISE_TEXTURE = 9,
    RENDER2D_ADAPTER_APPLY_STATE_BLOCK = 10,
    RENDER2D_ADAPTER_REGISTER_CONSTANT_LAYOUT = 11,
    RENDER2D_ADAPTER_SET_CONSTANT_LAYOUT = 12,
    RENDER2D_ADAPTER_APPLY_VERTEX_SHADER = 13,
    RENDER2D_ADAPTER_SET_VS_CONSTANTS = 14,
    RENDER2D_ADAPTER_UPDATE_PIXEL_SHADER = 15,
    RENDER2D_ADAPTER_UPDATE_PIXEL_SHADER_CONSTANTS = 16,
    RENDER2D_ADAPTER_UPDATE_AMBIENT = 17,
    RENDER2D_ADAPTER_UPDATE_FOG_COLOUR = 18,
    RENDER2D_ADAPTER_UPDATE_LIGHT_GLOBALS = 19,
    RENDER2D_ADAPTER_RESET_WORLD_TRANSFORM = 20,
    RENDER2D_ADAPTER_UPDATE_COMBINED_PROJECTION = 21,
    RENDER2D_ADAPTER_SET_WINDOW = 22,
    RENDER2D_ADAPTER_ATTACH_TEXTURE = 23,
    RENDER2D_ADAPTER_REALISE_RENDER_STATE = 24,
    RENDER2D_ADAPTER_DRAW_PRIMITIVE_UP = 25,
    RENDER2D_ADAPTER_ERASE_INFO_QUEUE = 26,
    RENDER2D_ADAPTER_CLEAR_VERTEX_QUEUE = 27,
    RENDER2D_ADAPTER_RESTORE_CAPTURE = 28
};

enum Render2DAdapterTextureSlot
{
    RENDER2D_TEXTURE_CURRENT = 1,
    RENDER2D_TEXTURE_CANDIDATE = 2,
    RENDER2D_TEXTURE_TEMPORARY = 3
};

enum Render2DAdapterStateValueKind
{
    RENDER2D_STATE_VALUE_GLOBAL = 1,
    RENDER2D_STATE_VALUE_LITERAL = 2,
    RENDER2D_STATE_VALUE_ARGUMENT = 3
};

enum Render2DAdapterDirtyFlag
{
    RENDER2D_DIRTY_PIXEL_SHADER = 0x00000001,
    RENDER2D_DIRTY_PIXEL_CONSTANTS = 0x00000002,
    RENDER2D_DIRTY_VERTEX_CONSTANTS_AND_AMBIENT = 0x00000004,
    RENDER2D_DIRTY_COMBINED_PROJECTION = 0x00000080,
    RENDER2D_DIRTY_LIGHT_GLOBALS = 0x00000400,
    RENDER2D_DIRTY_FOG_COLOUR = 0x00020000
};

struct Render2DAdapterFlush
{
    fable_u32 textureIdentity;
    fable_u32 primitiveType;
    fable_u32 primitiveCount;
    fable_u32 vertexIndex;
};

struct Render2DAdapterInput
{
    bool queueEmpty;
    bool entryVertexShadersEnabled;
    bool entryPixelShadersEnabled;
    bool renderStateArgument;
    bool worldTransformNeedsReset;
    fable_u32 dirtyFlagsAfterShaderApply;
    fable_u32 previousConstantLayout;
    fable_u32 firstWindowIdentity;
    const Render2DAdapterFlush* flushes;
    fable_u32 flushCount;
};

class IRender2DDrawListAdapter
{
public:
    virtual ~IRender2DDrawListAdapter()
    {
    }

    virtual void Invoke(
        fable_u32 eventKind,
        fable_u32 argument0,
        fable_u32 argument1,
        fable_u32 argument2,
        fable_u32 argument3) = 0;
};

void FABLE_FASTCALL FableDriveRender2DDrawListAdapter(
    const Render2DAdapterInput& input,
    IRender2DDrawListAdapter& adapter);
