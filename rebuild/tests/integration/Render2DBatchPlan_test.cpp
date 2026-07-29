#include <stdio.h>
#include <string.h>

#include "../../integration/render2d_batch_plan.h"

namespace
{
    FableRender2DPlanRecord MakeRecord(
        fable_u8 flags,
        fable_u32 window,
        fable_u32 texture,
        fable_u32 stateBlock)
    {
        FableRender2DPlanRecord record;
        memset(&record, 0, sizeof(record));
        record.flags = flags;
        for (fable_u32 index = 0; index < 4; ++index)
        {
            record.window.values[index] = window + index;
        }
        record.textureIdentity = texture;
        record.payload.normal.stateBlock = stateBlock;
        return record;
    }

    bool IsEvent(
        const FableRender2DPlanEvent& event,
        fable_u32 kind,
        fable_u32 argument0,
        fable_u32 argument1,
        fable_u32 argument2)
    {
        return
            event.kind == kind &&
            event.argument0 == argument0 &&
            event.argument1 == argument1 &&
            event.argument2 == argument2;
    }
}

int main()
{
    FableRender2DPlanEvent events[64];
    FableRender2DPlanOutput output = {
        events,
        64,
        0,
        false
    };

    FableBuildRender2DBatchPlan(0, 0, output);
    if (output.count != 0 || output.overflow)
    {
        printf("FABLETLC_RENDER2D_BATCH_PLAN FAIL code=1\n");
        return 1;
    }

    FableRender2DPlanRecord stable[3];
    for (fable_u32 index = 0; index < 3; ++index)
    {
        stable[index] = MakeRecord(0, 10, 0, 1);
    }
    FableBuildRender2DBatchPlan(stable, 3, output);
    if (
        output.count != 2 ||
        !IsEvent(
            events[0],
            FABLE_RENDER2D_PLAN_SET_WINDOW,
            0,
            0,
            0) ||
        !IsEvent(
            events[1],
            FABLE_RENDER2D_PLAN_FLUSH,
            4,
            3,
            0))
    {
        printf("FABLETLC_RENDER2D_BATCH_PLAN FAIL code=2\n");
        return 2;
    }

    FableRender2DPlanRecord stateSplit[3];
    stateSplit[0] = MakeRecord(0, 10, 0, 1);
    stateSplit[1] = MakeRecord(0, 10, 0, 1);
    stateSplit[2] = MakeRecord(0, 10, 0, 7);
    FableBuildRender2DBatchPlan(stateSplit, 3, output);
    if (
        output.count != 4 ||
        !IsEvent(
            events[1],
            FABLE_RENDER2D_PLAN_FLUSH,
            4,
            2,
            0) ||
        !IsEvent(
            events[2],
            FABLE_RENDER2D_PLAN_APPLY_STATE_BLOCK,
            7,
            0,
            0) ||
        !IsEvent(
            events[3],
            FABLE_RENDER2D_PLAN_FLUSH,
            4,
            1,
            6))
    {
        printf("FABLETLC_RENDER2D_BATCH_PLAN FAIL code=3\n");
        return 3;
    }

    FableRender2DPlanRecord topologySplit[2];
    topologySplit[0] = MakeRecord(0, 10, 0, 1);
    topologySplit[1] = MakeRecord(1, 10, 0, 1);
    FableBuildRender2DBatchPlan(topologySplit, 2, output);
    if (
        output.count != 4 ||
        !IsEvent(
            events[1],
            FABLE_RENDER2D_PLAN_FLUSH,
            4,
            1,
            0) ||
        !IsEvent(
            events[2],
            FABLE_RENDER2D_PLAN_SET_TOPOLOGY,
            2,
            0,
            0) ||
        !IsEvent(
            events[3],
            FABLE_RENDER2D_PLAN_FLUSH,
            2,
            1,
            3))
    {
        printf("FABLETLC_RENDER2D_BATCH_PLAN FAIL code=4\n");
        return 4;
    }

    FableRender2DPlanRecord withText[3];
    withText[0] = MakeRecord(0, 10, 0, 1);
    withText[1] = MakeRecord(2, 20, 0, 0);
    withText[2] = MakeRecord(0, 10, 3, 1);
    FableBuildRender2DBatchPlan(withText, 3, output);
    if (
        output.count != 11 ||
        !IsEvent(
            events[1],
            FABLE_RENDER2D_PLAN_FLUSH,
            4,
            1,
            0) ||
        !IsEvent(
            events[2],
            FABLE_RENDER2D_PLAN_SET_WINDOW,
            1,
            0,
            0) ||
        events[3].kind != FABLE_RENDER2D_PLAN_SUSPEND_SHADERS ||
        events[4].kind != FABLE_RENDER2D_PLAN_RENDER_TEXT ||
        events[5].kind != FABLE_RENDER2D_PLAN_RESUME_SHADERS ||
        events[6].kind != FABLE_RENDER2D_PLAN_REAPPLY_SHADER ||
        !IsEvent(
            events[7],
            FABLE_RENDER2D_PLAN_BIND_TEXTURE,
            3,
            0,
            0) ||
        !IsEvent(
            events[8],
            FABLE_RENDER2D_PLAN_APPLY_STATE_BLOCK,
            1,
            0,
            0) ||
        !IsEvent(
            events[9],
            FABLE_RENDER2D_PLAN_SET_WINDOW,
            2,
            0,
            0) ||
        !IsEvent(
            events[10],
            FABLE_RENDER2D_PLAN_FLUSH,
            4,
            1,
            3))
    {
        printf("FABLETLC_RENDER2D_BATCH_PLAN FAIL code=5\n");
        return 5;
    }

    FableRender2DPlanRecord capped[2002];
    for (fable_u32 index = 0; index < 2002; ++index)
    {
        capped[index] = MakeRecord(0, 10, 0, 1);
    }
    FableBuildRender2DBatchPlan(capped, 2002, output);
    if (
        output.count != 3 ||
        !IsEvent(
            events[1],
            FABLE_RENDER2D_PLAN_FLUSH,
            4,
            2001,
            0) ||
        !IsEvent(
            events[2],
            FABLE_RENDER2D_PLAN_FLUSH,
            4,
            1,
            6003))
    {
        printf("FABLETLC_RENDER2D_BATCH_PLAN FAIL code=6\n");
        return 6;
    }

    FableRender2DPlanOutput overflow = {
        events,
        1,
        0,
        false
    };
    FableBuildRender2DBatchPlan(withText, 3, overflow);
    if (!overflow.overflow || overflow.count != 1)
    {
        printf("FABLETLC_RENDER2D_BATCH_PLAN FAIL code=7\n");
        return 7;
    }

    FableUiRuntimeComponent componentA = {};
    componentA.definitionId = 122;
    componentA.initialised = 1;
    componentA.stateMask = 1;
    componentA.size.x = 8.0f;
    componentA.size.y = 6.0f;
    componentA.state[0].position.x = 0.0f;
    componentA.state[0].position.y = 2.0f;
    componentA.state[0].zoom.x = 1.0f;
    componentA.state[0].zoom.y = 1.0f;
    FableUiRuntimeComponent componentB = componentA;
    componentB.definitionId = 130;
    componentB.state[0].position.x = 8.0f;
    componentB.state[0].zoom.x = 80.0f;
    FableUiCountedComponent countedComponents[2] = {
        {&componentA, 0},
        {&componentB, 0}
    };
    FableUiGeneratedComponentVector generated = {
        countedComponents,
        2,
        2,
        0
    };
    FableUiRender2DBinding bindings[2] = {
        {
            122,
            0x12345678,
            0.0f,
            0.25f,
            1.0f,
            0.75f,
            0xA0FFFFFF
        },
        {
            130,
            0x87654321,
            0.0f,
            0.25f,
            1.0f,
            0.75f,
            0xA0FFFFFF
        }
    };
    FableRender2DSpriteVertex generatedVertices[12];
    FableRender2DPlanRecord generatedRecords[4];
    fable_u32 generatedVertexCount = 0;
    fable_u32 generatedRecordCount = 0;
    FableUiRender2DAppendTarget appendTarget = {
        generatedVertices,
        12,
        &generatedVertexCount,
        generatedRecords,
        4,
        &generatedRecordCount,
        0,
        0
    };
    if (
        !FableAppendUiGeneratedComponentsToRender2D(
            &generated,
            bindings,
            2,
            100.0f,
            50.0f,
            2.0f,
            3.0f,
            &appendTarget) ||
        generatedVertexCount != 12 ||
        generatedRecordCount != 4 ||
        generatedVertices[0].x != 100.0f ||
        generatedVertices[0].y != 56.0f ||
        generatedVertices[1].x != 116.0f ||
        generatedVertices[2].y != 74.0f ||
        generatedVertices[6].x != 116.0f ||
        generatedVertices[7].x != 1396.0f ||
        generatedVertices[7].u != 1.0f ||
        generatedVertices[8].v != 0.75f ||
        generatedVertices[6].diffuseColour != 0xA0FFFFFF ||
        generatedRecords[0].textureIdentity != 0x12345678 ||
        generatedRecords[0].payload.normal.stateBlock != 1 ||
        generatedRecords[1].textureIdentity != 0x12345678 ||
        generatedRecords[2].textureIdentity != 0x87654321)
    {
        printf("FABLETLC_RENDER2D_BATCH_PLAN FAIL code=8\n");
        return 8;
    }

    fable_u32 rejectedVertexCount = generatedVertexCount;
    fable_u32 rejectedRecordCount = generatedRecordCount;
    FableUiRender2DAppendTarget shortTarget = {
        generatedVertices,
        12,
        &rejectedVertexCount,
        generatedRecords,
        4,
        &rejectedRecordCount,
        0,
        0
    };
    if (
        FableAppendUiGeneratedComponentsToRender2D(
            &generated,
            bindings,
            2,
            0.0f,
            0.0f,
            1.0f,
            1.0f,
            &shortTarget) ||
        rejectedVertexCount != generatedVertexCount ||
        rejectedRecordCount != generatedRecordCount)
    {
        printf("FABLETLC_RENDER2D_BATCH_PLAN FAIL code=9\n");
        return 9;
    }

    FableUiRender2DBinding wrongBinding = bindings[0];
    wrongBinding.definitionId = 999;
    rejectedVertexCount = 0;
    rejectedRecordCount = 0;
    shortTarget.vertexCount = &rejectedVertexCount;
    shortTarget.recordCount = &rejectedRecordCount;
    if (
        FableAppendUiGeneratedComponentsToRender2D(
            &generated,
            &wrongBinding,
            1,
            0.0f,
            0.0f,
            1.0f,
            1.0f,
            &shortTarget) ||
        rejectedVertexCount != 0 ||
        rejectedRecordCount != 0)
    {
        printf("FABLETLC_RENDER2D_BATCH_PLAN FAIL code=10\n");
        return 10;
    }

    const FableUiVector2 clipMinimum = {100.0f, 50.0f};
    const FableUiVector2 clipMaximum = {500.0f, 80.0f};
    generatedVertexCount = 0;
    generatedRecordCount = 0;
    appendTarget.clipMinimum = &clipMinimum;
    appendTarget.clipMaximum = &clipMaximum;
    if (
        !FableAppendUiGeneratedComponentsToRender2D(
            &generated,
            bindings,
            2,
            100.0f,
            50.0f,
            2.0f,
            3.0f,
            &appendTarget) ||
        generatedVertexCount != 12 ||
        generatedRecordCount != 4 ||
        generatedVertices[7].x != 500.0f ||
        generatedVertices[7].u < 0.299f ||
        generatedVertices[7].u > 0.301f)
    {
        printf("FABLETLC_RENDER2D_BATCH_PLAN FAIL code=11\n");
        return 11;
    }

    printf("FABLETLC_RENDER2D_BATCH_PLAN PASS\n");
    return 0;
}
