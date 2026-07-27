#include "render2d_draw_list_adapter.h"

namespace
{
    void Emit(
        IRender2DDrawListAdapter& adapter,
        fable_u32 kind,
        fable_u32 argument0,
        fable_u32 argument1,
        fable_u32 argument2,
        fable_u32 argument3)
    {
        adapter.Invoke(
            kind,
            argument0,
            argument1,
            argument2,
            argument3);
    }

    void RequestState(
        IRender2DDrawListAdapter& adapter,
        fable_u32 coreOffset,
        fable_u32 valueKind,
        fable_u32 value)
    {
        Emit(
            adapter,
            RENDER2D_ADAPTER_REQUEST_TRACKED_STATE,
            coreOffset,
            valueKind,
            value,
            0);
    }

    void EmitStateRequests(
        const Render2DAdapterInput& input,
        IRender2DDrawListAdapter& adapter)
    {
        RequestState(
            adapter, 0x2890, RENDER2D_STATE_VALUE_GLOBAL, 0x01396FB0);
        RequestState(
            adapter, 0x282C, RENDER2D_STATE_VALUE_GLOBAL, 0x01396FA4);
        RequestState(
            adapter, 0x2818, RENDER2D_STATE_VALUE_GLOBAL, 0x01396FA0);
        RequestState(
            adapter, 0x30C4, RENDER2D_STATE_VALUE_GLOBAL, 0x01396E58);
        RequestState(
            adapter, 0x3164, RENDER2D_STATE_VALUE_GLOBAL, 0x01396E58);
        RequestState(
            adapter, 0x2E44, RENDER2D_STATE_VALUE_GLOBAL, 0x01396E64);
        RequestState(
            adapter, 0x2EE4, RENDER2D_STATE_VALUE_GLOBAL, 0x01396DD0);
        RequestState(
            adapter, 0x2F84, RENDER2D_STATE_VALUE_GLOBAL, 0x01396DD0);
        RequestState(
            adapter, 0x2944, RENDER2D_STATE_VALUE_LITERAL, 0);
        RequestState(
            adapter,
            0x2868,
            RENDER2D_STATE_VALUE_ARGUMENT,
            input.renderStateArgument ? 1 : 0);
        RequestState(
            adapter, 0x287C, RENDER2D_STATE_VALUE_GLOBAL, 0x01396E04);
        RequestState(
            adapter,
            0x2854,
            RENDER2D_STATE_VALUE_ARGUMENT,
            input.renderStateArgument ? 1 : 0);
        RequestState(
            adapter, 0x2A20, RENDER2D_STATE_VALUE_LITERAL, 1);
    }

    void EmitShaderUploads(
        const Render2DAdapterInput& input,
        IRender2DDrawListAdapter& adapter)
    {
        const fable_u32 dirty = input.dirtyFlagsAfterShaderApply;
        if ((dirty & RENDER2D_DIRTY_VERTEX_CONSTANTS_AND_AMBIENT) != 0)
            Emit(
                adapter,
                RENDER2D_ADAPTER_SET_VS_CONSTANTS,
                0, 0, 0, 0);
        if ((dirty & RENDER2D_DIRTY_PIXEL_SHADER) != 0)
            Emit(
                adapter,
                RENDER2D_ADAPTER_UPDATE_PIXEL_SHADER,
                0, 0, 0, 0);
        if ((dirty & RENDER2D_DIRTY_PIXEL_CONSTANTS) != 0)
            Emit(
                adapter,
                RENDER2D_ADAPTER_UPDATE_PIXEL_SHADER_CONSTANTS,
                0, 0, 0, 0);
        if ((dirty & RENDER2D_DIRTY_VERTEX_CONSTANTS_AND_AMBIENT) != 0)
            Emit(
                adapter,
                RENDER2D_ADAPTER_UPDATE_AMBIENT,
                0, 0, 0, 0);
        if ((dirty & RENDER2D_DIRTY_FOG_COLOUR) != 0)
            Emit(
                adapter,
                RENDER2D_ADAPTER_UPDATE_FOG_COLOUR,
                0, 0, 0, 0);
        if ((dirty & RENDER2D_DIRTY_LIGHT_GLOBALS) != 0)
            Emit(
                adapter,
                RENDER2D_ADAPTER_UPDATE_LIGHT_GLOBALS,
                0, 0, 0, 0);
        if (input.worldTransformNeedsReset)
            Emit(
                adapter,
                RENDER2D_ADAPTER_RESET_WORLD_TRANSFORM,
                0, 0, 0, 0);
        if ((dirty & RENDER2D_DIRTY_COMBINED_PROJECTION) != 0)
            Emit(
                adapter,
                RENDER2D_ADAPTER_UPDATE_COMBINED_PROJECTION,
                0, 0, 0, 0);
    }

    void Flush(
        const Render2DAdapterFlush& flush,
        IRender2DDrawListAdapter& adapter)
    {
        if (flush.primitiveCount == 0)
            return;

        Emit(
            adapter,
            RENDER2D_ADAPTER_ATTACH_TEXTURE,
            0,
            flush.textureIdentity,
            0,
            0);
        Emit(
            adapter,
            RENDER2D_ADAPTER_REALISE_RENDER_STATE,
            0, 0, 0, 0);
        Emit(
            adapter,
            RENDER2D_ADAPTER_DRAW_PRIMITIVE_UP,
            flush.primitiveType,
            flush.primitiveCount,
            flush.vertexIndex * 0x20,
            0x20);
    }

    void UninitialiseTexture(
        IRender2DDrawListAdapter& adapter,
        fable_u32 textureSlot)
    {
        Emit(
            adapter,
            RENDER2D_ADAPTER_UNINITIALISE_TEXTURE,
            textureSlot,
            0,
            0,
            0);
    }
}

void FABLE_FASTCALL FableDriveRender2DDrawListAdapter(
    const Render2DAdapterInput& input,
    IRender2DDrawListAdapter& adapter)
{
    if (input.queueEmpty)
    {
        UninitialiseTexture(adapter, RENDER2D_TEXTURE_CURRENT);
        UninitialiseTexture(adapter, RENDER2D_TEXTURE_CANDIDATE);
        return;
    }

    if (!input.entryVertexShadersEnabled)
        Emit(
            adapter,
            RENDER2D_ADAPTER_ENABLE_VERTEX_SHADERS,
            0, 0, 0, 0);
    if (input.entryPixelShadersEnabled)
        Emit(
            adapter,
            RENDER2D_ADAPTER_DISABLE_PIXEL_SHADERS,
            0, 0, 0, 0);

    Emit(adapter, RENDER2D_ADAPTER_BEGIN_CAPTURE, 0, 0, 0, 0);
    EmitStateRequests(input, adapter);

    Emit(
        adapter,
        RENDER2D_ADAPTER_INITIALISE_NULL_TEXTURE,
        RENDER2D_TEXTURE_TEMPORARY,
        0, 0, 0);
    Emit(
        adapter,
        RENDER2D_ADAPTER_ASSIGN_TEXTURE,
        RENDER2D_TEXTURE_CURRENT,
        RENDER2D_TEXTURE_TEMPORARY,
        0, 0);
    UninitialiseTexture(adapter, RENDER2D_TEXTURE_TEMPORARY);

    Emit(adapter, RENDER2D_ADAPTER_APPLY_STATE_BLOCK, 1, 0, 0, 0);
    Emit(
        adapter,
        RENDER2D_ADAPTER_REGISTER_CONSTANT_LAYOUT,
        0,
        0x013CA8A8,
        0,
        0);
    Emit(adapter, RENDER2D_ADAPTER_SET_CONSTANT_LAYOUT, 0, 0, 0, 0);
    Emit(adapter, RENDER2D_ADAPTER_APPLY_VERTEX_SHADER, 0, 0, 0, 0);
    EmitShaderUploads(input, adapter);
    Emit(
        adapter,
        RENDER2D_ADAPTER_SET_WINDOW,
        input.firstWindowIdentity,
        0, 0, 0);

    for (fable_u32 index = 0; index < input.flushCount; ++index)
        Flush(input.flushes[index], adapter);

    Emit(adapter, RENDER2D_ADAPTER_ERASE_INFO_QUEUE, 0, 0, 0, 0);
    Emit(adapter, RENDER2D_ADAPTER_CLEAR_VERTEX_QUEUE, 0, 0, 0, 0);
    Emit(adapter, RENDER2D_ADAPTER_ATTACH_TEXTURE, 0, 0, 0, 0);
    Emit(
        adapter,
        RENDER2D_ADAPTER_SET_CONSTANT_LAYOUT,
        input.previousConstantLayout,
        0, 0, 0);
    Emit(adapter, RENDER2D_ADAPTER_RESTORE_CAPTURE, 0, 0, 0, 0);

    if (!input.entryVertexShadersEnabled)
        Emit(
            adapter,
            RENDER2D_ADAPTER_DISABLE_VERTEX_SHADERS,
            0, 0, 0, 0);
    if (input.entryPixelShadersEnabled)
        Emit(
            adapter,
            RENDER2D_ADAPTER_ENABLE_PIXEL_SHADERS,
            0, 0, 0, 0);

    UninitialiseTexture(adapter, RENDER2D_TEXTURE_CURRENT);
    UninitialiseTexture(adapter, RENDER2D_TEXTURE_CANDIDATE);
}
