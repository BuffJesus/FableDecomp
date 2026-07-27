#include <stdio.h>
#include <string.h>

#include "../../integration/render2d_draw_list_adapter.h"

struct TraceEvent
{
    fable_u32 kind;
    fable_u32 arguments[4];
};

class TraceAdapter : public IRender2DDrawListAdapter
{
public:
    TraceAdapter()
        : count(0),
          overflow(false)
    {
    }

    virtual void Invoke(
        fable_u32 kind,
        fable_u32 argument0,
        fable_u32 argument1,
        fable_u32 argument2,
        fable_u32 argument3)
    {
        if (count >= 64)
        {
            overflow = true;
            return;
        }
        TraceEvent& event = events[count++];
        event.kind = kind;
        event.arguments[0] = argument0;
        event.arguments[1] = argument1;
        event.arguments[2] = argument2;
        event.arguments[3] = argument3;
    }

    TraceEvent events[64];
    fable_u32 count;
    bool overflow;
};

static bool IsEvent(
    const TraceEvent& event,
    fable_u32 kind,
    fable_u32 argument0,
    fable_u32 argument1,
    fable_u32 argument2,
    fable_u32 argument3)
{
    return
        event.kind == kind &&
        event.arguments[0] == argument0 &&
        event.arguments[1] == argument1 &&
        event.arguments[2] == argument2 &&
        event.arguments[3] == argument3;
}

int main()
{
    Render2DAdapterInput empty;
    memset(&empty, 0, sizeof(empty));
    empty.queueEmpty = true;
    TraceAdapter emptyTrace;
    FableDriveRender2DDrawListAdapter(empty, emptyTrace);
    if (
        emptyTrace.count != 2 ||
        emptyTrace.overflow ||
        !IsEvent(
            emptyTrace.events[0],
            RENDER2D_ADAPTER_UNINITIALISE_TEXTURE,
            RENDER2D_TEXTURE_CURRENT,
            0, 0, 0) ||
        !IsEvent(
            emptyTrace.events[1],
            RENDER2D_ADAPTER_UNINITIALISE_TEXTURE,
            RENDER2D_TEXTURE_CANDIDATE,
            0, 0, 0))
    {
        printf("FABLETLC_RENDER2D_DRAW_LIST_ADAPTER FAIL code=1\n");
        return 1;
    }

    Render2DAdapterFlush flushes[3] = {
        {0xAA, 4, 3, 5},
        {0xBB, 2, 0, 99},
        {0xCC, 2, 7, 14}
    };
    Render2DAdapterInput full;
    memset(&full, 0, sizeof(full));
    full.entryPixelShadersEnabled = true;
    full.renderStateArgument = true;
    full.worldTransformNeedsReset = true;
    full.dirtyFlagsAfterShaderApply =
        RENDER2D_DIRTY_PIXEL_SHADER |
        RENDER2D_DIRTY_PIXEL_CONSTANTS |
        RENDER2D_DIRTY_VERTEX_CONSTANTS_AND_AMBIENT |
        RENDER2D_DIRTY_COMBINED_PROJECTION |
        RENDER2D_DIRTY_LIGHT_GLOBALS |
        RENDER2D_DIRTY_FOG_COLOUR;
    full.previousConstantLayout = 7;
    full.firstWindowIdentity = 0x1234;
    full.flushes = flushes;
    full.flushCount = 3;

    TraceAdapter trace;
    FableDriveRender2DDrawListAdapter(full, trace);
    static const fable_u32 expectedKinds[47] = {
        1, 4, 5,
        6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
        7, 8, 9, 10, 11, 12, 13,
        14, 15, 16, 17, 18, 19, 20, 21, 22,
        23, 24, 25,
        23, 24, 25,
        26, 27, 23, 12, 28, 2, 3, 9, 9
    };
    bool pass = trace.count == 47 && !trace.overflow;
    for (fable_u32 index = 0; pass && index < 47; ++index)
        pass = trace.events[index].kind == expectedKinds[index];

    static const fable_u32 stateOffsets[13] = {
        0x2890, 0x282C, 0x2818, 0x30C4, 0x3164,
        0x2E44, 0x2EE4, 0x2F84, 0x2944, 0x2868,
        0x287C, 0x2854, 0x2A20
    };
    for (fable_u32 state = 0; pass && state < 13; ++state)
        pass = trace.events[3 + state].arguments[0] ==
            stateOffsets[state];

    pass =
        pass &&
        IsEvent(
            trace.events[34],
            RENDER2D_ADAPTER_DRAW_PRIMITIVE_UP,
            4, 3, 5 * 0x20, 0x20) &&
        IsEvent(
            trace.events[37],
            RENDER2D_ADAPTER_DRAW_PRIMITIVE_UP,
            2, 7, 14 * 0x20, 0x20) &&
        IsEvent(
            trace.events[40],
            RENDER2D_ADAPTER_ATTACH_TEXTURE,
            0, 0, 0, 0) &&
        IsEvent(
            trace.events[41],
            RENDER2D_ADAPTER_SET_CONSTANT_LAYOUT,
            7, 0, 0, 0);
    if (!pass)
    {
        printf("FABLETLC_RENDER2D_DRAW_LIST_ADAPTER FAIL code=2\n");
        return 2;
    }

    Render2DAdapterInput clean;
    memset(&clean, 0, sizeof(clean));
    clean.entryVertexShadersEnabled = true;
    clean.dirtyFlagsAfterShaderApply = 0x80000000;
    clean.previousConstantLayout = 9;
    clean.firstWindowIdentity = 0x5678;
    TraceAdapter cleanTrace;
    FableDriveRender2DDrawListAdapter(clean, cleanTrace);
    if (
        cleanTrace.count != 29 ||
        cleanTrace.events[0].kind != RENDER2D_ADAPTER_BEGIN_CAPTURE ||
        cleanTrace.events[20].kind !=
            RENDER2D_ADAPTER_APPLY_VERTEX_SHADER ||
        cleanTrace.events[21].kind != RENDER2D_ADAPTER_SET_WINDOW ||
        cleanTrace.events[22].kind !=
            RENDER2D_ADAPTER_ERASE_INFO_QUEUE ||
        cleanTrace.events[26].kind !=
            RENDER2D_ADAPTER_RESTORE_CAPTURE ||
        cleanTrace.events[27].kind !=
            RENDER2D_ADAPTER_UNINITIALISE_TEXTURE ||
        cleanTrace.events[28].kind !=
            RENDER2D_ADAPTER_UNINITIALISE_TEXTURE)
    {
        printf("FABLETLC_RENDER2D_DRAW_LIST_ADAPTER FAIL code=3\n");
        return 3;
    }

    printf("FABLETLC_RENDER2D_DRAW_LIST_ADAPTER PASS\n");
    return 0;
}
