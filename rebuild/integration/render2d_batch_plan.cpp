#include "render2d_batch_plan.h"

namespace
{
    bool WindowsEqual(
        const FableRender2DWindow& left,
        const FableRender2DWindow& right)
    {
        for (fable_u32 index = 0; index < 4; ++index)
        {
            if (left.values[index] != right.values[index])
            {
                return false;
            }
        }
        return true;
    }

    void EmitEvent(
        FableRender2DPlanOutput& output,
        fable_u32 kind,
        fable_u32 argument0,
        fable_u32 argument1,
        fable_u32 argument2)
    {
        if (output.count >= output.capacity)
        {
            output.overflow = true;
            return;
        }

        FableRender2DPlanEvent& event =
            output.events[output.count++];
        event.kind = kind;
        event.argument0 = argument0;
        event.argument1 = argument1;
        event.argument2 = argument2;
    }

    void FlushBatch(
        FableRender2DPlanOutput& output,
        fable_u32 primitiveCount,
        bool lineList,
        fable_u32& vertexIndex)
    {
        if (primitiveCount == 0)
        {
            return;
        }

        EmitEvent(
            output,
            FABLE_RENDER2D_PLAN_FLUSH,
            lineList ? 2 : 4,
            primitiveCount,
            vertexIndex);
        vertexIndex += primitiveCount * (lineList ? 2 : 3);
    }
}

void FABLE_FASTCALL FableBuildRender2DBatchPlan(
    const FableRender2DPlanRecord* records,
    fable_u32 recordCount,
    FableRender2DPlanOutput& output)
{
    output.count = 0;
    output.overflow = false;
    if (recordCount == 0)
    {
        return;
    }

    FableRender2DWindow currentWindow = records[0].window;
    fable_u32 currentTexture = 0;
    fable_u32 currentStateBlock = 1;
    bool lineList = false;
    bool reapplyShader = false;
    fable_u32 batchStart = 0;
    fable_u32 vertexIndex = 0;

    EmitEvent(
        output,
        FABLE_RENDER2D_PLAN_SET_WINDOW,
        0,
        0,
        0);

    for (fable_u32 index = 0; index < recordCount; ++index)
    {
        const FableRender2DPlanRecord& record = records[index];
        const bool textRecord = (record.flags & 2) != 0;
        bool windowChanged = false;
        bool topologyChanged = false;
        bool textureChanged = false;
        bool stateChanged = false;

        if (!textRecord)
        {
            if (reapplyShader)
            {
                EmitEvent(
                    output,
                    FABLE_RENDER2D_PLAN_REAPPLY_SHADER,
                    index,
                    0,
                    0);
                reapplyShader = false;
            }

            windowChanged =
                !WindowsEqual(currentWindow, record.window);
            topologyChanged =
                lineList != ((record.flags & 1) != 0);
            textureChanged =
                currentTexture != record.textureIdentity;
            stateChanged =
                currentStateBlock != record.payload.normal.stateBlock;
        }

        const fable_u32 accumulated = index - batchStart;
        if (
            accumulated > 2000 ||
            textRecord ||
            windowChanged ||
            topologyChanged ||
            textureChanged ||
            stateChanged)
        {
            FlushBatch(
                output,
                accumulated,
                lineList,
                vertexIndex);
            batchStart = index;

            if (!textRecord)
            {
                if (textureChanged)
                {
                    currentTexture = record.textureIdentity;
                    EmitEvent(
                        output,
                        FABLE_RENDER2D_PLAN_BIND_TEXTURE,
                        currentTexture,
                        0,
                        0);
                }
                if (stateChanged)
                {
                    currentStateBlock =
                        record.payload.normal.stateBlock;
                    EmitEvent(
                        output,
                        FABLE_RENDER2D_PLAN_APPLY_STATE_BLOCK,
                        currentStateBlock,
                        0,
                        0);
                }
                if (windowChanged)
                {
                    currentWindow = record.window;
                    EmitEvent(
                        output,
                        FABLE_RENDER2D_PLAN_SET_WINDOW,
                        index,
                        0,
                        0);
                }
                if (topologyChanged)
                {
                    lineList = !lineList;
                    EmitEvent(
                        output,
                        FABLE_RENDER2D_PLAN_SET_TOPOLOGY,
                        lineList ? 2 : 4,
                        0,
                        0);
                }
            }
            else
            {
                if (!WindowsEqual(currentWindow, record.window))
                {
                    currentWindow = record.window;
                    EmitEvent(
                        output,
                        FABLE_RENDER2D_PLAN_SET_WINDOW,
                        index,
                        0,
                        0);
                }

                EmitEvent(
                    output,
                    FABLE_RENDER2D_PLAN_SUSPEND_SHADERS,
                    index,
                    0,
                    0);
                EmitEvent(
                    output,
                    FABLE_RENDER2D_PLAN_RENDER_TEXT,
                    index,
                    0,
                    0);
                EmitEvent(
                    output,
                    FABLE_RENDER2D_PLAN_RESUME_SHADERS,
                    index,
                    0,
                    0);

                batchStart = index + 1;
                currentTexture = 0;
                currentStateBlock = 9999;
                lineList = false;
                reapplyShader = true;
            }
        }
    }

    FlushBatch(
        output,
        recordCount - batchStart,
        lineList,
        vertexIndex);
}
