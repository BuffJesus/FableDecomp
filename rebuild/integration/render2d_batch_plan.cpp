#include "render2d_batch_plan.h"

#include <string.h>

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

    const FableUiRender2DBinding* FindUiRender2DBinding(
        const FableUiRender2DBinding* bindings,
        fable_u32 bindingCount,
        unsigned long definitionId)
    {
        for (fable_u32 index = 0; index != bindingCount; ++index)
        {
            if (bindings[index].definitionId == definitionId)
                return &bindings[index];
        }
        return 0;
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

bool FABLE_FASTCALL FableAppendUiGeneratedComponentsToRender2D(
    const FableUiGeneratedComponentVector* generated,
    const FableUiRender2DBinding* bindings,
    fable_u32 bindingCount,
    float parentX,
    float parentY,
    float scaleX,
    float scaleY,
    FableUiRender2DAppendTarget* target)
{
    if (
        generated == 0 ||
        target == 0 ||
        target->vertexCount == 0 ||
        target->recordCount == 0 ||
        (generated->size != 0 && generated->values == 0) ||
        (bindingCount != 0 && bindings == 0) ||
        (target->vertexCapacity != 0 && target->vertices == 0) ||
        (target->recordCapacity != 0 && target->records == 0) ||
        (target->clipMinimum == 0) !=
            (target->clipMaximum == 0) ||
        (
            target->clipMinimum != 0 &&
            (
                target->clipMinimum->x >= target->clipMaximum->x ||
                target->clipMinimum->y >= target->clipMaximum->y
            )
        ) ||
        scaleX <= 0.0f ||
        scaleY <= 0.0f)
    {
        return false;
    }

    const fable_u32 initialVertexCount = *target->vertexCount;
    const fable_u32 initialRecordCount = *target->recordCount;
    if (
        initialVertexCount > target->vertexCapacity ||
        initialRecordCount > target->recordCapacity)
    {
        return false;
    }

    fable_u32 visibleComponentCount = 0;
    for (
        fable_u32 componentIndex = 0;
        componentIndex != generated->size;
        ++componentIndex)
    {
        const FableUiRuntimeComponent* component =
            generated->values[componentIndex].component;
        if (
            component == 0 ||
            component->initialised == 0 ||
            (component->stateMask & 1) == 0 ||
            FindUiRender2DBinding(
                bindings,
                bindingCount,
                component->definitionId) == 0 ||
            component->size.x <= 0.0f ||
            component->size.y <= 0.0f ||
            component->state[0].zoom.x <= 0.0f ||
            component->state[0].zoom.y <= 0.0f)
        {
            return false;
        }

        const float left =
            parentX + component->state[0].position.x * scaleX;
        const float top =
            parentY + component->state[0].position.y * scaleY;
        const float right =
            left +
            component->size.x *
                component->state[0].zoom.x *
                scaleX;
        const float bottom =
            top +
            component->size.y *
                component->state[0].zoom.y *
                scaleY;
        if (
            target->clipMinimum != 0 &&
            (
                right <= target->clipMinimum->x ||
                bottom <= target->clipMinimum->y ||
                left >= target->clipMaximum->x ||
                top >= target->clipMaximum->y
            ))
        {
            continue;
        }
        ++visibleComponentCount;
    }

    if (
        visibleComponentCount >
            (target->vertexCapacity - initialVertexCount) / 6 ||
        visibleComponentCount >
            (target->recordCapacity - initialRecordCount) / 2)
    {
        return false;
    }

    for (
        fable_u32 componentIndex = 0;
        componentIndex != generated->size;
        ++componentIndex)
    {
        const FableUiRuntimeComponent& component =
            *generated->values[componentIndex].component;
        const FableUiRender2DBinding& binding =
            *FindUiRender2DBinding(
                bindings,
                bindingCount,
                component.definitionId);
        const float unclippedLeft =
            parentX + component.state[0].position.x * scaleX;
        const float unclippedTop =
            parentY + component.state[0].position.y * scaleY;
        const float unclippedRight =
            unclippedLeft +
            component.size.x *
                component.state[0].zoom.x *
                scaleX;
        const float unclippedBottom =
            unclippedTop +
            component.size.y *
                component.state[0].zoom.y *
                scaleY;
        if (
            target->clipMinimum != 0 &&
            (
                unclippedRight <= target->clipMinimum->x ||
                unclippedBottom <= target->clipMinimum->y ||
                unclippedLeft >= target->clipMaximum->x ||
                unclippedTop >= target->clipMaximum->y
            ))
        {
            continue;
        }

        float left = unclippedLeft;
        float top = unclippedTop;
        float right = unclippedRight;
        float bottom = unclippedBottom;
        float u0 = binding.u0;
        float v0 = binding.v0;
        float u1 = binding.u1;
        float v1 = binding.v1;
        if (target->clipMinimum != 0)
        {
            if (left < target->clipMinimum->x)
            {
                u0 +=
                    (binding.u1 - binding.u0) *
                    (target->clipMinimum->x - left) /
                    (unclippedRight - unclippedLeft);
                left = target->clipMinimum->x;
            }
            if (top < target->clipMinimum->y)
            {
                v0 +=
                    (binding.v1 - binding.v0) *
                    (target->clipMinimum->y - top) /
                    (unclippedBottom - unclippedTop);
                top = target->clipMinimum->y;
            }
            if (right > target->clipMaximum->x)
            {
                u1 -=
                    (binding.u1 - binding.u0) *
                    (right - target->clipMaximum->x) /
                    (unclippedRight - unclippedLeft);
                right = target->clipMaximum->x;
            }
            if (bottom > target->clipMaximum->y)
            {
                v1 -=
                    (binding.v1 - binding.v0) *
                    (bottom - target->clipMaximum->y) /
                    (unclippedBottom - unclippedTop);
                bottom = target->clipMaximum->y;
            }
        }

        const FableRender2DSpriteVertex quad[6] = {
            {
                left, top, 0.0f, 1.0f,
                binding.diffuseColour, 0,
                u0, v0
            },
            {
                right, top, 0.0f, 1.0f,
                binding.diffuseColour, 0,
                u1, v0
            },
            {
                left, bottom, 0.0f, 1.0f,
                binding.diffuseColour, 0,
                u0, v1
            },
            {
                left, bottom, 0.0f, 1.0f,
                binding.diffuseColour, 0,
                u0, v1
            },
            {
                right, top, 0.0f, 1.0f,
                binding.diffuseColour, 0,
                u1, v0
            },
            {
                right, bottom, 0.0f, 1.0f,
                binding.diffuseColour, 0,
                u1, v1
            }
        };
        memcpy(
            target->vertices + *target->vertexCount,
            quad,
            sizeof(quad));
        *target->vertexCount += 6;

        FableRender2DPlanRecord& first =
            target->records[(*target->recordCount)++];
        memset(&first, 0, sizeof(first));
        first.textureIdentity = binding.textureIdentity;
        first.payload.normal.stateBlock = 1;
        target->records[(*target->recordCount)++] = first;
    }
    return true;
}
