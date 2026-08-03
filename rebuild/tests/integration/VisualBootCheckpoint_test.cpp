#include "fable_visual_boot.h"
#include "../../integration/fable_visual_d3d9.h"
#include "../../integration/frontend_list_layout.h"
#include "fable_video_system.h"
#include "../../integration/frontend_input_dispatch.h"

#include <stdio.h>
#include <direct.h>
#include <wchar.h>

extern "C" __declspec(dllimport)
FableInstanceHandle FABLE_STDCALL GetModuleHandleA(const char* moduleName);

struct FrontendManagerDispatchTestComponent
{
    bool accepts;
    unsigned int conditionCalls;
    unsigned int processCalls;
    unsigned int lastEvent;
};

bool FrontendManagerDispatchTestCondition(void* component, unsigned int event)
{
    FrontendManagerDispatchTestComponent* target =
        static_cast<FrontendManagerDispatchTestComponent*>(component);
    ++target->conditionCalls;
    target->lastEvent = event;
    return target->accepts;
}

void FrontendManagerDispatchTestProcess(void* component, unsigned int event)
{
    FrontendManagerDispatchTestComponent* target =
        static_cast<FrontendManagerDispatchTestComponent*>(component);
    ++target->processCalls;
    target->lastEvent = event;
}

int main()
{
    unsigned int action = 0;
    unsigned int listEvent = 0;
    if (
        !FableRetailFrontendWheelAction(1.0f, &action) ||
        action != FableRetailFrontendActionWheelUp ||
        !FableRetailFrontendListEventFromAction(action, &listEvent) ||
        listEvent != FableRetailFrontendListEventUp ||
        !FableRetailFrontendWheelAction(-120.0f, &action) ||
        action != FableRetailFrontendActionWheelDown ||
        !FableRetailFrontendListEventFromAction(action, &listEvent) ||
        listEvent != FableRetailFrontendListEventDown ||
        FableRetailFrontendWheelAction(0.0001f, &action) ||
        FableRetailFrontendWheelAction(-0.0001f, &action) ||
        FableRetailFrontendListEventFromAction(0x1A, &listEvent))
    {
        return 119;
    }

    if (
        FableRetailFrontendMouseWheelMovementFromRaw(120) != 0.12f ||
        FableRetailFrontendMouseWheelMovementFromRaw(-120) != -0.12f ||
        FableRetailFrontendMouseWheelMovementFromRaw(1) != 0.001f ||
        !FableRetailFrontendWheelAction(
            FableRetailFrontendMouseWheelMovementFromRaw(1),
            &action) ||
        action != FableRetailFrontendActionWheelUp ||
        !FableRetailFrontendWheelAction(
            FableRetailFrontendMouseWheelMovementFromRaw(-1),
            &action) ||
        action != FableRetailFrontendActionWheelDown)
    {
        return 143;
    }

    FableRetailFrontendListProcessContext listProcessContext = {
        true,
        true,
        true,
        true};
    if (!FableRetailFrontendListProcessEvent(
            FableRetailFrontendListEventUp,
            listProcessContext,
            &listEvent) ||
        listEvent != FableRetailFrontendListEventUp ||
        !FableRetailFrontendListProcessEvent(
            FableRetailFrontendListEventDown,
            listProcessContext,
            &listEvent) ||
        listEvent != FableRetailFrontendListEventDown)
    {
        return 122;
    }
    listProcessContext.viewportConditionPasses = false;
    if (FableRetailFrontendListProcessEvent(
            FableRetailFrontendListEventUp,
            listProcessContext,
            &listEvent))
    {
        return 123;
    }
    listProcessContext.viewportConditionPasses = true;
    listProcessContext.hoverConditionPasses = false;
    if (FableRetailFrontendListProcessEvent(
            FableRetailFrontendListEventUp,
            listProcessContext,
            &listEvent))
    {
        return 124;
    }
    listProcessContext.hoverConditionPasses = true;
    listProcessContext.managerActionConditionPasses = false;
    if (FableRetailFrontendListProcessEvent(
            FableRetailFrontendListEventUp,
            listProcessContext,
            &listEvent))
    {
        return 125;
    }
    listProcessContext.managerActionConditionPasses = true;
    listProcessContext.componentConditionPasses = false;
    if (FableRetailFrontendListProcessEvent(
            FableRetailFrontendListEventUp,
            listProcessContext,
            &listEvent))
    {
        return 126;
    }
    listProcessContext.componentConditionPasses = true;
    if (FableRetailFrontendListProcessEvent(2, listProcessContext, &listEvent) ||
        FableRetailFrontendListProcessEvent(
            FableRetailFrontendListEventUp,
            listProcessContext,
            0))
    {
        return 127;
    }

    FrontendManagerDispatchTestComponent currentComponent = {
        true,
        0,
        0,
        0};
    FrontendManagerDispatchTestComponent registeredComponent = {
        true,
        0,
        0,
        0};
    const FableRetailFrontendManagerEventTarget currentTarget = {
        &currentComponent,
        FrontendManagerDispatchTestCondition,
        FrontendManagerDispatchTestProcess};
    const FableRetailFrontendManagerEventTarget registeredTargets[1] = {
        {
            &registeredComponent,
            FrontendManagerDispatchTestCondition,
            FrontendManagerDispatchTestProcess}
    };
    if (
        FableRetailFrontendManagerProcessEvent(
            &currentTarget,
            registeredTargets,
            1,
            FableRetailFrontendListEventDown) != 1 ||
        currentComponent.conditionCalls != 1 ||
        currentComponent.processCalls != 1 ||
        registeredComponent.conditionCalls != 0 ||
        registeredComponent.processCalls != 0)
    {
        return 131;
    }
    currentComponent.accepts = false;
    if (
        FableRetailFrontendManagerProcessEvent(
            &currentTarget,
            registeredTargets,
            1,
            FableRetailFrontendListEventUp) != 0 ||
        currentComponent.conditionCalls != 2 ||
        currentComponent.processCalls != 1 ||
        registeredComponent.conditionCalls != 0)
    {
        return 132;
    }
    if (
        FableRetailFrontendManagerProcessEvent(
            0,
            registeredTargets,
            1,
            FableRetailFrontendListEventUp) != 1 ||
        registeredComponent.conditionCalls != 1 ||
        registeredComponent.processCalls != 1)
    {
        return 133;
    }

    char profileEditText[128] = {};
    unsigned int profileEditLength = 0;
    if (!FableRetailFrontendProcessTextInputCharacter(
            static_cast<unsigned short>('A'),
            profileEditText,
            &profileEditLength) ||
        !FableRetailFrontendProcessTextInputCharacter(
            static_cast<unsigned short>(0x00E9),
            profileEditText,
            &profileEditLength) ||
        profileEditLength != 2 ||
        profileEditText[0] != 'A' ||
        static_cast<unsigned char>(profileEditText[1]) != 0xE9 ||
        !FableRetailFrontendProcessTextInputCharacter(
            8,
            profileEditText,
            &profileEditLength) ||
        profileEditLength != 1 ||
        !FableRetailFrontendProcessTextInputCharacter(
            8,
            profileEditText,
            &profileEditLength) ||
        profileEditLength != 0 ||
        FableRetailFrontendProcessTextInputCharacter(
            8,
            profileEditText,
            &profileEditLength))
    {
        return 128;
    }
    for (unsigned int i = 0; i != 127; ++i)
    {
        if (!FableRetailFrontendProcessTextInputCharacter(
                static_cast<unsigned short>('x'),
                profileEditText,
                &profileEditLength))
        {
            return 129;
        }
    }
    if (profileEditLength != 127 ||
        profileEditText[127] != '\0' ||
        FableRetailFrontendProcessTextInputCharacter(
            static_cast<unsigned short>('y'),
            profileEditText,
            &profileEditLength))
    {
        return 130;
    }

    bool clickablePressed = false;
    if (!FableRetailFrontendClickableProcessEvent(
            FableRetailFrontendClickableLeftClicked,
            true,
            &clickablePressed) ||
        !clickablePressed ||
        FableRetailFrontendClickableProcessEvent(
            FableRetailFrontendClickableLeftClicked,
            false,
            &clickablePressed) ||
        !FableRetailFrontendClickableProcessEvent(
            FableRetailFrontendClickableLeftUnclicked,
            true,
            &clickablePressed) ||
        clickablePressed ||
        FableRetailFrontendClickableProcessEvent(
            FableRetailFrontendClickableLeftUnclicked,
            true,
            &clickablePressed))
    {
        return 121;
    }

    if (
        kFableFrontendProfilesListArrowLayout.upArrowDefinitionIndex != 426 ||
        kFableFrontendProfilesListArrowLayout.downArrowDefinitionIndex != 423 ||
        kFableFrontendProfilesListArrowLayout.arrowX != 304 ||
        kFableFrontendProfilesListArrowLayout.upY != 80 ||
        kFableFrontendProfilesListArrowLayout.downY != 380 ||
        kFableFrontendProfilesListArrowLayout.arrowWidth != 32 ||
        kFableFrontendProfilesListArrowLayout.arrowHeight != 32 ||
        kFableFrontendDeleteListArrowLayout.upArrowDefinitionIndex != 427 ||
        kFableFrontendDeleteListArrowLayout.downArrowDefinitionIndex != 424 ||
        kFableFrontendDeleteListArrowLayout.arrowWidth != 32 ||
        kFableFrontendDeleteListArrowLayout.arrowHeight != 32 ||
        kFableFrontendSaveListArrowLayout.upArrowDefinitionIndex != 428 ||
        kFableFrontendSaveListArrowLayout.downArrowDefinitionIndex != 425 ||
        kFableFrontendSaveListArrowLayout.arrowWidth != 32 ||
        kFableFrontendSaveListArrowLayout.arrowHeight != 32 ||
        kFableFrontendRedefineListArrowLayout.upArrowDefinitionIndex != 417 ||
        kFableFrontendRedefineListArrowLayout.downArrowDefinitionIndex != 420 ||
        kFableFrontendRedefineListArrowLayout.arrowWidth != 32 ||
        kFableFrontendRedefineListArrowLayout.arrowHeight != 32)
    {
        return 120;
    }

    const fable_u8 source[] = {
        0xFF, 0x00, 0x00,
        0xFF, 0xFF, 0xFF,
        0xCD, 0xCD,
        0x00, 0x00, 0xFF,
        0x00, 0xFF, 0x00,
        0xCD, 0xCD
    };
    fable_u8 rgba[20];
    fable_u8 rgb1555[12];
    for (unsigned int byte = 0; byte < sizeof(rgba); ++byte)
        rgba[byte] = 0xCD;
    for (unsigned int wordByte = 0;
         wordByte < sizeof(rgb1555);
         ++wordByte)
    {
        rgb1555[wordByte] = 0xCD;
    }

    if (
        !FableConvertVideoRgb24Frame(
            0x15,
            source,
            8,
            rgba,
            10,
            2,
            2) ||
        rgba[0] != 0xFF ||
        rgba[1] != 0x00 ||
        rgba[2] != 0x00 ||
        rgba[3] != 0xFF ||
        rgba[4] != 0xFF ||
        rgba[5] != 0xFF ||
        rgba[6] != 0xFF ||
        rgba[7] != 0xFF ||
        rgba[8] != 0xCD ||
        rgba[9] != 0xCD ||
        rgba[10] != 0x00 ||
        rgba[11] != 0x00 ||
        rgba[12] != 0xFF ||
        rgba[13] != 0xFF ||
        rgba[14] != 0x00 ||
        rgba[15] != 0xFF ||
        rgba[16] != 0x00 ||
        rgba[17] != 0xFF ||
        rgba[18] != 0xCD ||
        rgba[19] != 0xCD)
    {
        return 2;
    }
    if (
        !FableConvertVideoRgb24Frame(
            0x19,
            source,
            8,
            rgb1555,
            6,
            2,
            2) ||
        rgb1555[0] != 0x1F ||
        rgb1555[1] != 0x80 ||
        rgb1555[2] != 0xFF ||
        rgb1555[3] != 0xFF ||
        rgb1555[4] != 0xCD ||
        rgb1555[5] != 0xCD ||
        rgb1555[6] != 0x00 ||
        rgb1555[7] != 0xFC ||
        rgb1555[8] != 0xE0 ||
        rgb1555[9] != 0x83 ||
        rgb1555[10] != 0xCD ||
        rgb1555[11] != 0xCD ||
        FableConvertVideoRgb24Frame(
            0x18,
            source,
            8,
            rgb1555,
            6,
            2,
            2))
    {
        return 3;
    }

#if defined(FABLETLC_RETAIL_FRONTEND_ARTWORK)
    if (!FableIsRetailVisualAssetEmbedded())
        return 1;
#else
    if (FableIsRetailVisualAssetEmbedded())
        return 1;
#endif

    if (
        FableShouldPlayBootVideos(0) ||
        FableShouldPlayBootVideos("") ||
        !FableShouldPlayBootVideos("--retail-video") ||
        !FableShouldPlayBootVideos("--buff-jesus") ||
        !FableShouldPlayBootVideos("--buff-jesus --retail-video") ||
        FableShouldPlayBootVideos(
            "--buff-jesus --skip-boot-videos") ||
        FableShouldPlayBootVideos(
            "--retail-video --skip-boot-videos"))
    {
        return 4;
    }

    FableUiResolvedTransform parent = {};
    parent.position.x = 40.0f;
    parent.position.y = 115.0f;
    parent.truePosition.x = 80.0f;
    parent.truePosition.y = 172.5f;
    parent.scale.x = 2.0f;
    parent.scale.y = 3.0f;
    parent.trueScale.x = 1.5f;
    parent.trueScale.y = 2.5f;
    FableUiVector2 tableLocal = {-32.0f, -2.0f};
    FableUiVector2 tableLocalScale = {0.5f, 2.0f};
    FableUiResolvedTransform table = {};
    FableResolveUiComponentTransform(
        &tableLocal,
        &tableLocalScale,
        &parent,
        &table);
    FableUiVector2 childLocal = {368.0f, -3.0f};
    FableUiVector2 uiScale = {2.0f, 1.5f};
    FableUiVector2 childOrdinary = {};
    FableUiVector2 childTrue = {};
    FableResolveUiGeneratedChildOrigins(
        &childLocal,
        &uiScale,
        &table,
        &childOrdinary,
        &childTrue);
    if (
        table.position.x != 8.0f ||
        table.position.y != 113.0f ||
        table.truePosition.x != 48.0f ||
        table.truePosition.y != 170.5f ||
        table.scale.x != 1.0f ||
        table.scale.y != 6.0f ||
        table.trueScale.x != 0.75f ||
        table.trueScale.y != 5.0f ||
        childOrdinary.x != 376.0f ||
        childOrdinary.y != 110.0f ||
        childTrue.x != 784.0f ||
        childTrue.y != 166.0f)
    {
        return 5;
    }

    const fable_i32 horizontalKeys[] = {-1, 1, 2};
    FableUiVector2 horizontalStart = {8.0f, 113.0f};
    FableUiVector2 horizontalPrimarySize = {64.0f, 30.0f};
    FableUiVector2 horizontalSecondarySize = {8.0f, 30.0f};
    FableUiTableLineSecondary horizontalSecondary[3] = {};
    FableUiTableLinePlan horizontalPlan = {};
    FablePlanUiTableLine(
        false,
        &horizontalStart,
        &horizontalPrimarySize,
        &horizontalSecondarySize,
        4,
        1,
        horizontalKeys,
        3,
        horizontalSecondary,
        3,
        &horizontalPlan);
    if (
        horizontalPlan.primaryPosition.x != 8.0f ||
        horizontalPlan.primaryPosition.y != 113.0f ||
        horizontalPlan.primaryZoom.x != 4.0f ||
        horizontalPlan.primaryZoom.y != 1.0f ||
        horizontalPlan.secondaryCount != 3 ||
        horizontalPlan.generatedChildCount != 4 ||
        horizontalSecondary[0].resourceKey != -1 ||
        horizontalSecondary[0].position.x != 72.0f ||
        horizontalSecondary[0].position.y != 113.0f ||
        horizontalSecondary[1].resourceKey != 1 ||
        horizontalSecondary[1].position.x != 80.0f ||
        horizontalSecondary[2].resourceKey != 2 ||
        horizontalSecondary[2].position.x != 88.0f ||
        horizontalPlan.finalCursor.x != 96.0f ||
        horizontalPlan.finalCursor.y != 113.0f)
    {
        return 6;
    }

    const fable_i32 verticalKeys[] = {-1, 1};
    FableUiVector2 verticalStart = {376.0f, 110.0f};
    FableUiVector2 verticalPrimarySize = {220.0f, 30.0f};
    FableUiVector2 verticalSecondarySize = {220.0f, 5.0f};
    FableUiTableLineSecondary verticalSecondary[1] = {};
    FableUiTableLinePlan verticalPlan = {};
    FablePlanUiTableLine(
        true,
        &verticalStart,
        &verticalPrimarySize,
        &verticalSecondarySize,
        3,
        1,
        verticalKeys,
        2,
        verticalSecondary,
        1,
        &verticalPlan);
    if (
        verticalPlan.primaryZoom.x != 1.0f ||
        verticalPlan.primaryZoom.y != 3.0f ||
        verticalPlan.secondaryCount != 2 ||
        verticalPlan.generatedChildCount != 3 ||
        verticalSecondary[0].resourceKey != -1 ||
        verticalSecondary[0].position.x != 376.0f ||
        verticalSecondary[0].position.y != 140.0f ||
        verticalPlan.finalCursor.x != 376.0f ||
        verticalPlan.finalCursor.y != 150.0f)
    {
        return 7;
    }

    FableUiTableSpritePlanInput spriteInput = {};
    spriteInput.availableSpriteMask = (1UL << 13) - 1;
    spriteInput.horizontalSeparatorCount = 2;
    spriteInput.verticalSeparatorCount = 3;
    spriteInput.horizontalRepeatCount = 8;
    spriteInput.verticalRepeatCount = 6;
    spriteInput.horizontalInteriorResourceKeyOffset = 4;
    spriteInput.verticalInteriorResourceKeyOffset = 5;
    spriteInput.topRightCornerVisible = 1;
    spriteInput.bottomLeftCornerVisible = 1;
    spriteInput.bottomRightCornerVisible = 1;
    FableUiTableSpritePlanEvent spriteEvents[13] = {};
    FableUiTableSpritePlan spritePlan = {};
    FablePlanUiTableSpriteComposition(
        &spriteInput,
        spriteEvents,
        13,
        &spritePlan);
    if (
        spritePlan.logicalEventCount != 13 ||
        spritePlan.writtenEventCount != 13 ||
        spriteEvents[0].kind != FableUiTableSpriteCorner ||
        spriteEvents[0].primarySpriteKey != 0 ||
        spriteEvents[3].primarySpriteKey != 3 ||
        spriteEvents[4].kind != FableUiTableSpriteHorizontalLine ||
        spriteEvents[4].primarySpriteKey != 4 ||
        spriteEvents[4].secondarySpriteKey != 9 ||
        spriteEvents[4].repeatCount != 8 ||
        spriteEvents[5].secondarySpriteKey != 12 ||
        spriteEvents[5].separatorIndex != 0 ||
        spriteEvents[5].resourceKeyOffset != 4 ||
        spriteEvents[7].primarySpriteKey != 5 ||
        spriteEvents[7].secondarySpriteKey != 8 ||
        spriteEvents[8].kind != FableUiTableSpriteVerticalLine ||
        spriteEvents[8].primarySpriteKey != 6 ||
        spriteEvents[8].secondarySpriteKey != 10 ||
        spriteEvents[9].secondarySpriteKey != 12 ||
        spriteEvents[9].resourceKeyOffset != 5 ||
        spriteEvents[11].separatorIndex != 2 ||
        spriteEvents[12].primarySpriteKey != 7 ||
        spriteEvents[12].secondarySpriteKey != 11)
    {
        return 8;
    }

    FableUiTableSpritePlanInput sparseSpriteInput = {};
    sparseSpriteInput.availableSpriteMask = 1UL << 4;
    sparseSpriteInput.horizontalSeparatorCount = 2;
    sparseSpriteInput.horizontalRepeatCount = 3;
    FableUiTableSpritePlanEvent sparseSpriteEvent[1] = {};
    FableUiTableSpritePlan sparseSpritePlan = {};
    FablePlanUiTableSpriteComposition(
        &sparseSpriteInput,
        sparseSpriteEvent,
        1,
        &sparseSpritePlan);
    if (
        sparseSpritePlan.logicalEventCount != 3 ||
        sparseSpritePlan.writtenEventCount != 1 ||
        sparseSpriteEvent[0].primarySpriteKey != 4 ||
        sparseSpriteEvent[0].secondarySpriteKey != -1 ||
        sparseSpriteEvent[0].repeatCount != 3)
    {
        return 9;
    }

    const unsigned long horizontalSeparatorTiles[] = {1, 3};
    const unsigned long verticalSeparatorTiles[] = {2};
    FableUiTableGeometryInput geometryInput = {};
    geometryInput.availableSpriteMask = (1UL << 4) - 1;
    geometryInput.horizontalRepeatCount = 5;
    geometryInput.verticalRepeatCount = 4;
    geometryInput.horizontalEdgeSize.x = 10.0f;
    geometryInput.horizontalEdgeSize.y = 3.0f;
    geometryInput.verticalEdgeSize.x = 4.0f;
    geometryInput.verticalEdgeSize.y = 8.0f;
    geometryInput.cornerSize[0].x = 20.0f;
    geometryInput.cornerSize[0].y = 16.0f;
    geometryInput.cornerSize[1].x = 5.0f;
    geometryInput.cornerSize[1].y = 9.0f;
    geometryInput.cornerSize[2].x = 8.0f;
    geometryInput.cornerSize[2].y = 4.0f;
    geometryInput.cornerSize[3].x = 7.0f;
    geometryInput.cornerSize[3].y = 6.0f;
    geometryInput.horizontalSeparatorTiles = horizontalSeparatorTiles;
    geometryInput.horizontalSeparatorCount = 2;
    geometryInput.verticalSeparatorTiles = verticalSeparatorTiles;
    geometryInput.verticalSeparatorCount = 1;
    FableUiTableInteriorLine horizontalInterior[1] = {};
    FableUiTableInteriorLine verticalInterior[1] = {};
    FableUiTableGeometryPlan geometryPlan = {};
    FablePlanUiTableGeometry(
        &geometryInput,
        horizontalInterior,
        1,
        verticalInterior,
        1,
        &geometryPlan);
    if (
        geometryPlan.emittedCornerMask != 0xf ||
        geometryPlan.horizontalExtent != 50.0f ||
        geometryPlan.verticalExtent != 32.0f ||
        geometryPlan.cornerPosition[0].x != 0.0f ||
        geometryPlan.cornerPosition[0].y != 0.0f ||
        geometryPlan.cornerPosition[1].x != 70.0f ||
        geometryPlan.cornerPosition[1].y != 0.0f ||
        geometryPlan.cornerPosition[2].x != 0.0f ||
        geometryPlan.cornerPosition[2].y != 48.0f ||
        geometryPlan.cornerPosition[3].x != 58.0f ||
        geometryPlan.cornerPosition[3].y != 41.0f ||
        geometryPlan.topLineStart.x != 20.0f ||
        geometryPlan.topLineStart.y != 0.0f ||
        geometryPlan.bottomLineStart.x != 8.0f ||
        geometryPlan.bottomLineStart.y != 39.0f ||
        geometryPlan.leftLineStart.x != 0.0f ||
        geometryPlan.leftLineStart.y != 16.0f ||
        geometryPlan.rightLineStart.x != 65.0f ||
        geometryPlan.rightLineStart.y != 9.0f ||
        geometryPlan.logicalHorizontalInteriorCount != 2 ||
        geometryPlan.writtenHorizontalInteriorCount != 1 ||
        horizontalInterior[0].position.x != 10.0f ||
        horizontalInterior[0].position.y != 24.0f ||
        horizontalInterior[0].repeatCount != 5 ||
        horizontalInterior[0].resourceKeyOffset != 1 ||
        geometryPlan.logicalVerticalInteriorCount != 1 ||
        geometryPlan.writtenVerticalInteriorCount != 1 ||
        verticalInterior[0].position.x != 40.0f ||
        verticalInterior[0].position.y != 8.0f ||
        verticalInterior[0].repeatCount != 3 ||
        verticalInterior[0].resourceKeyOffset != 1)
    {
        return 10;
    }

    FableUiTableRuntimeInput runtimeTable = {};
    runtimeTable.geometry = geometryInput;
    runtimeTable.geometry.availableSpriteMask = (1UL << 13) - 1;
    for (unsigned int spriteKey = 0; spriteKey != 13; ++spriteKey)
    {
        runtimeTable.sprite[spriteKey].definitionId =
            1000 + spriteKey;
        runtimeTable.sprite[spriteKey].size.x = 1.0f;
        runtimeTable.sprite[spriteKey].size.y = 1.0f;
    }
    runtimeTable.sprite[0].size = geometryInput.cornerSize[0];
    runtimeTable.sprite[1].size = geometryInput.cornerSize[1];
    runtimeTable.sprite[2].size = geometryInput.cornerSize[2];
    runtimeTable.sprite[3].size = geometryInput.cornerSize[3];
    runtimeTable.sprite[4].size =
        geometryInput.horizontalEdgeSize;
    runtimeTable.sprite[5].size =
        geometryInput.horizontalEdgeSize;
    runtimeTable.sprite[6].size =
        geometryInput.verticalEdgeSize;
    runtimeTable.sprite[7].size =
        geometryInput.verticalEdgeSize;
    runtimeTable.sprite[8].size.x = 2.0f;
    runtimeTable.sprite[9].size.x = 2.0f;
    runtimeTable.sprite[10].size.y = 2.0f;
    runtimeTable.sprite[11].size.y = 2.0f;

    FableUiCountedComponent generatedStorage[20] = {};
    FableUiComponentLifetimeCounters lifetime = {};
    FableUiGeneratedComponentVector generated = {
        generatedStorage,
        0,
        20,
        &lifetime};
    const bool runtimeTableConstructed =
        FableConstructUiTableComponents(
            &runtimeTable,
            &generated);
    if (
        !runtimeTableConstructed ||
        generated.size != 20 ||
        lifetime.componentAllocations != 20 ||
        lifetime.controlAllocations != 20 ||
        lifetime.retains != 20 ||
        lifetime.releases != 20 ||
        lifetime.componentDeletions != 0 ||
        lifetime.controlDeletions != 0)
    {
        printf(
            "table constructed=%u size=%u alloc=%u ctrl=%u "
            "retain=%u release=%u delete=%u ctrlDelete=%u\n",
            runtimeTableConstructed ? 1U : 0U,
            generated.size,
            lifetime.componentAllocations,
            lifetime.controlAllocations,
            lifetime.retains,
            lifetime.releases,
            lifetime.componentDeletions,
            lifetime.controlDeletions);
        return 22;
    }
    for (
        unsigned int generatedIndex = 0;
        generatedIndex != generated.size;
        ++generatedIndex)
    {
        if (
            generated.values[generatedIndex].component == 0 ||
            generated.values[generatedIndex].reference == 0 ||
            generated.values[generatedIndex].
                reference->referenceCount != 1 ||
            generated.values[generatedIndex].
                component->initialised != 1 ||
            generated.values[generatedIndex].
                component->stateMask != 1)
        {
            return 23;
        }
    }
    if (
        generated.values[0].component->sourceSpriteKey != 0 ||
        generated.values[0].component->definitionId != 1000 ||
        generated.values[0].component->state[0].position.x != 0.0f ||
        generated.values[0].component->state[0].position.y != 0.0f ||
        generated.values[4].component->sourceSpriteKey != 4 ||
        generated.values[4].component->state[0].position.x != 20.0f ||
        generated.values[4].component->state[0].position.y != 0.0f ||
        generated.values[4].component->state[0].zoom.x != 5.0f ||
        generated.values[4].component->state[0].zoom.y != 1.0f ||
        generated.values[5].component->sourceSpriteKey != 9 ||
        generated.values[5].component->state[0].position.x != 30.0f ||
        generated.values[6].component->sourceSpriteKey != 4 ||
        generated.values[6].component->state[0].position.x != 10.0f ||
        generated.values[6].component->state[0].position.y != 24.0f ||
        generated.values[7].component->sourceSpriteKey != 12 ||
        generated.values[8].component->sourceSpriteKey != 4 ||
        generated.values[8].component->state[0].position.y != 40.0f ||
        generated.values[10].component->sourceSpriteKey != 5 ||
        generated.values[12].component->sourceSpriteKey != 6 ||
        generated.values[12].component->state[0].zoom.x != 1.0f ||
        generated.values[12].component->state[0].zoom.y != 4.0f ||
        generated.values[15].component->sourceSpriteKey != 6 ||
        generated.values[15].component->state[0].position.x != 40.0f ||
        generated.values[15].component->state[0].position.y != 8.0f ||
        generated.values[17].component->sourceSpriteKey != 7)
    {
        return 24;
    }
    FableReleaseUiGeneratedComponents(&generated);
    if (
        generated.size != 0 ||
        lifetime.releases != 40 ||
        lifetime.componentDeletions != 20 ||
        lifetime.controlDeletions != 20)
    {
        return 25;
    }

    FableUiCountedComponent shortStorage[1] = {};
    FableUiComponentLifetimeCounters failedLifetime = {};
    FableUiGeneratedComponentVector shortGenerated = {
        shortStorage,
        0,
        1,
        &failedLifetime};
    if (
        FableConstructUiTableComponents(
            &runtimeTable,
            &shortGenerated) ||
        shortGenerated.size != 0 ||
        failedLifetime.componentAllocations !=
            failedLifetime.componentDeletions ||
        failedLifetime.controlAllocations !=
            failedLifetime.controlDeletions)
    {
        return 26;
    }

    const unsigned char visibleChildren[] = {0, 1, 1, 1, 0, 0};
    FableUiListSelectionPlan listSelectionPlan = {};
    FablePlanUiListSelection(
        5,
        visibleChildren,
        6,
        &listSelectionPlan);
    if (
        listSelectionPlan.selectedChild != 5 ||
        listSelectionPlan.firstVisibleChild != 1 ||
        listSelectionPlan.lastVisibleChild != 3 ||
        listSelectionPlan.scrollDelta.x != 0.0f ||
        listSelectionPlan.scrollDelta.y != -50.0f ||
        !listSelectionPlan.requestsScroll)
    {
        return 11;
    }
    FablePlanUiListSelection(
        0,
        visibleChildren,
        6,
        &listSelectionPlan);
    if (
        listSelectionPlan.selectedChild != 0 ||
        listSelectionPlan.scrollDelta.y != 25.0f ||
        !listSelectionPlan.requestsScroll)
    {
        return 12;
    }
    FablePlanUiListSelection(
        2,
        visibleChildren,
        6,
        &listSelectionPlan);
    if (
        listSelectionPlan.selectedChild != 2 ||
        listSelectionPlan.requestsScroll)
    {
        return 13;
    }
    FablePlanUiListSelection(
        99,
        visibleChildren,
        6,
        &listSelectionPlan);
    if (
        listSelectionPlan.selectedChild != 99 ||
        listSelectionPlan.requestsScroll)
    {
        return 14;
    }

    FableUiFrontEndListOffsetPlan frontEndListOffsetPlan = {};
    FablePlanUiFrontEndListInitialOffsets(
        7,
        &frontEndListOffsetPlan);
    if (
        frontEndListOffsetPlan.interpolationScalar != 1.0f ||
        frontEndListOffsetPlan.positionOffset.x != 0.0f ||
        frontEndListOffsetPlan.positionOffset.y != 0.0f)
    {
        return 15;
    }

    const FableUiVector2 genericListDefinitionOffset = { 4.0f, 30.0f };
    FableUiFrontEndListOffsetPlan genericListInitialPlan = {};
    FablePlanUiListInitialOffsets(
        5,
        &genericListDefinitionOffset,
        64,
        false,
        &genericListInitialPlan);
    if (
        genericListInitialPlan.positionOffset.x != 8.0f ||
        genericListInitialPlan.positionOffset.y != -60.0f)
    {
        return 16;
    }

    FableUiListRecomputeInput genericListInput = {};
    genericListInput.childCount = 5;
    genericListInput.definitionPositionOffset =
        genericListDefinitionOffset;
    genericListInput.alphaFalloff = 64;
    genericListInput.initialAlpha =
        genericListInitialPlan.interpolationScalar;
    genericListInput.initialPosition =
        genericListInitialPlan.positionOffset;
    FableUiListRecomputedChild genericListChildren[5] = {};
    FableUiListRecomputePlan genericListPlan = {};
    FablePlanUiListRecomputeOffsets(
        &genericListInput,
        genericListChildren,
        5,
        &genericListPlan);
    if (
        genericListPlan.logicalChildCount != 5 ||
        genericListPlan.writtenChildCount != 5 ||
        genericListPlan.positionStateMask != 0x73 ||
        genericListPlan.alphaStateMask != 0x32 ||
        genericListChildren[0].position.x != 8.0f ||
        genericListChildren[0].position.y != -60.0f ||
        genericListChildren[1].position.x != 4.0f ||
        genericListChildren[1].position.y != -30.0f ||
        genericListChildren[2].position.x != 0.0f ||
        genericListChildren[2].position.y != 0.0f ||
        genericListChildren[3].position.x != 4.0f ||
        genericListChildren[3].position.y != 30.0f ||
        genericListChildren[4].position.x != 8.0f ||
        genericListChildren[4].position.y != 60.0f ||
        genericListChildren[0].alpha != 126 ||
        genericListChildren[1].alpha != 191 ||
        genericListChildren[2].alpha != 255 ||
        genericListChildren[3].alpha != 191 ||
        genericListChildren[4].alpha != 127)
    {
        return 17;
    }

    FableUiRuntimeListChild runtimeListChildren[5] = {};
    FableUiRuntimeStateMap runtimeListSnapshots[5] = {};
    for (unsigned int child = 0; child != 5; ++child)
    {
        runtimeListChildren[child].currentState = 3;
        runtimeListChildren[child].states.state[1].colour.red =
            static_cast<unsigned char>(10 + child);
        runtimeListChildren[child].states.state[1].colour.green = 20;
        runtimeListChildren[child].states.state[1].colour.blue = 30;
        runtimeListChildren[child].states.state[4].colour.red = 40;
        runtimeListChildren[child].states.state[5].colour.red = 50;
    }
    FableUiListRecomputePlan appliedListPlan = {};
    if (!FableApplyUiListRecomputedStates(
            &genericListInput,
            runtimeListChildren,
            5,
            runtimeListSnapshots,
            5,
            &appliedListPlan) ||
        appliedListPlan.positionStateMask != 0x73 ||
        appliedListPlan.alphaStateMask != 0x32 ||
        runtimeListChildren[0].states.stateMask != 0x73 ||
        runtimeListChildren[2].states.state[0].position.x != 0.0f ||
        runtimeListChildren[2].states.state[0].position.y != 0.0f ||
        runtimeListChildren[4].states.state[6].position.x != 8.0f ||
        runtimeListChildren[4].states.state[6].position.y != 60.0f ||
        runtimeListChildren[0].states.state[1].colour.alpha != 126 ||
        runtimeListChildren[1].states.state[4].colour.alpha != 191 ||
        runtimeListChildren[2].states.state[5].colour.alpha != 255 ||
        runtimeListChildren[0].states.state[1].colour.red != 10 ||
        runtimeListChildren[0].currentState != 3 ||
        runtimeListChildren[4].currentPosition.x != 8.0f ||
        runtimeListChildren[4].currentPosition.y != 60.0f ||
        runtimeListChildren[0].currentColour.alpha != 126 ||
        runtimeListSnapshots[3].state[4].position.y != 30.0f ||
        runtimeListSnapshots[3].state[5].colour.alpha != 191)
    {
        return 27;
    }

    FableUiRuntimeListChild capacitySentinel = {};
    capacitySentinel.states.stateMask = 0x80000000u;
    FableUiRuntimeStateMap snapshotSentinel = {};
    snapshotSentinel.stateMask = 0x40000000u;
    FableUiListRecomputePlan rejectedApplyPlan = {};
    if (
        FableApplyUiListRecomputedStates(
            &genericListInput,
            &capacitySentinel,
            1,
            &snapshotSentinel,
            1,
            &rejectedApplyPlan) ||
        capacitySentinel.states.stateMask != 0x80000000u ||
        snapshotSentinel.stateMask != 0x40000000u)
    {
        return 28;
    }

    unsigned char frontEndListAlpha[4] = {};
    FableUiFrontEndListScrollPlan frontEndListScrollPlan = {};
    FablePlanUiFrontEndListScroll(
        false,
        4,
        0,
        true,
        64,
        frontEndListAlpha,
        4,
        &frontEndListScrollPlan);
    if (
        frontEndListScrollPlan.moved ||
        !frontEndListScrollPlan.blockedAtBoundary ||
        !frontEndListScrollPlan.requestsInvalidAction ||
        frontEndListScrollPlan.requestsMoveAction ||
        frontEndListScrollPlan.soundRequest !=
            FableUiFrontEndSoundError ||
        frontEndListScrollPlan.soundCriteriaDefinitionOffset != 0x1A4 ||
        frontEndListScrollPlan.selectedChild != 0)
    {
        return 18;
    }

    FablePlanUiFrontEndListScroll(
        false,
        4,
        0,
        false,
        64,
        frontEndListAlpha,
        4,
        &frontEndListScrollPlan);
    if (
        !frontEndListScrollPlan.moved ||
        frontEndListScrollPlan.selectedChild != 3 ||
        frontEndListScrollPlan.rowTranslation.x != 0.0f ||
        frontEndListScrollPlan.rowTranslation.y != 30.0f ||
        frontEndListScrollPlan.previousChildState != 4 ||
        frontEndListScrollPlan.selectedChildState != 3 ||
        frontEndListScrollPlan.soundRequest !=
            FableUiFrontEndSoundUpDown ||
        frontEndListScrollPlan.soundCriteriaDefinitionOffset != 0x194 ||
        frontEndListScrollPlan.logicalAlphaCount != 0)
    {
        return 19;
    }

    FablePlanUiFrontEndListScroll(
        true,
        4,
        1,
        true,
        64,
        frontEndListAlpha,
        4,
        &frontEndListScrollPlan);
    if (
        !frontEndListScrollPlan.moved ||
        frontEndListScrollPlan.selectedChild != 2 ||
        frontEndListScrollPlan.rowTranslation.y != -30.0f ||
        frontEndListScrollPlan.soundRequest !=
            FableUiFrontEndSoundUpDown ||
        frontEndListScrollPlan.soundCriteriaDefinitionOffset != 0x194 ||
        frontEndListScrollPlan.logicalAlphaCount != 4 ||
        frontEndListScrollPlan.writtenAlphaCount != 4 ||
        frontEndListAlpha[0] != 126 ||
        frontEndListAlpha[1] != 190 ||
        frontEndListAlpha[2] != 255 ||
        frontEndListAlpha[3] != 190)
    {
        return 20;
    }

    FablePlanUiFrontEndListScroll(
        true,
        1,
        0,
        false,
        64,
        frontEndListAlpha,
        4,
        &frontEndListScrollPlan);
    if (
        frontEndListScrollPlan.moved ||
        !frontEndListScrollPlan.requestsInvalidAction ||
        frontEndListScrollPlan.soundRequest !=
            FableUiFrontEndSoundError ||
        frontEndListScrollPlan.soundCriteriaDefinitionOffset != 0x1A4)
    {
        return 21;
    }

    // CList::ProcessEvent @ 0x0053673b uses event 0 for up and event 1
    // for down.  The frontend adapter must preserve that retail vocabulary.
    FablePlanUiFrontEndListProcessEvent(
        FableUiFrontEndListEventDown,
        4,
        0,
        true,
        0,
        0,
        0,
        &frontEndListScrollPlan);
    if (
        !frontEndListScrollPlan.moved ||
        frontEndListScrollPlan.selectedChild != 1)
    {
        return 32;
    }
    FablePlanUiFrontEndListProcessEvent(
        FableUiFrontEndListEventUp,
        4,
        0,
        true,
        0,
        0,
        0,
        &frontEndListScrollPlan);
    if (
        frontEndListScrollPlan.moved ||
        !frontEndListScrollPlan.blockedAtBoundary)
    {
        return 33;
    }
    FablePlanUiFrontEndListProcessEvent(
        2,
        4,
        1,
        false,
        0,
        0,
        0,
        &frontEndListScrollPlan);
    if (
        frontEndListScrollPlan.moved ||
        frontEndListScrollPlan.selectedChild != 1 ||
        frontEndListScrollPlan.soundRequest !=
            FableUiFrontEndSoundNone)
    {
        return 34;
    }

    FableUiFrontendProfileActionPlan profileAction = {};
    FablePlanVisualFrontendProfileAction(
        FableUiFrontendProfileModeNormal,
        0,
        2,
        FableUiFrontendProfileInputActivate,
        &profileAction);
    if (!profileAction.dispatch ||
        profileAction.action != FableRetailFrontendManagerActionNewProfile)
        return 35;
    FablePlanVisualFrontendProfileAction(
        FableUiFrontendProfileModeNormal,
        1,
        2,
        FableUiFrontendProfileInputActivate,
        &profileAction);
    if (!profileAction.dispatch ||
        profileAction.action != FableRetailFrontendManagerActionLoadProfile)
        return 36;
    FablePlanVisualFrontendProfileAction(
        FableUiFrontendProfileModeEmpty,
        0,
        0,
        FableUiFrontendProfileInputActivate,
        &profileAction);
    if (!profileAction.dispatch ||
        profileAction.action != FableRetailFrontendManagerActionNewProfile)
        return 37;
    FablePlanVisualFrontendProfileAction(
        FableUiFrontendProfileModeDelete,
        1,
        2,
        FableUiFrontendProfileInputActivate,
        &profileAction);
    if (!profileAction.dispatch ||
        profileAction.action != FableRetailFrontendManagerActionDeleteRow)
        return 38;
    FablePlanVisualFrontendProfileAction(
        FableUiFrontendProfileModeNew,
        0,
        2,
        FableUiFrontendProfileInputKeyboardConfirm,
        &profileAction);
    if (!profileAction.dispatch ||
        profileAction.action !=
            FableRetailFrontendManagerActionKeyboardConfirm)
        return 39;
    FablePlanVisualFrontendProfileAction(
        FableUiFrontendProfileModeNew,
        0,
        2,
        FableUiFrontendProfileInputKeyboardCancel,
        &profileAction);
    if (!profileAction.dispatch ||
        profileAction.action != FableRetailFrontendManagerActionKeyboardCancel)
        return 40;
    FablePlanVisualFrontendProfileAction(
        FableUiFrontendProfileModeDelete,
        0,
        2,
        FableUiFrontendProfileInputDeleteConfirm,
        &profileAction);
    if (!profileAction.dispatch ||
        profileAction.action != FableRetailFrontendManagerActionDeleteConfirm)
        return 41;
    FablePlanVisualFrontendProfileAction(
        FableUiFrontendProfileModeDeleteConfirm,
        0,
        2,
        FableUiFrontendProfileInputActivate,
        &profileAction);
    if (!profileAction.dispatch ||
        profileAction.action != FableRetailFrontendManagerActionDeleteConfirm)
        return 42;

    FableUiRuntimeListChild wrappingChildren[4] = {};
    for (unsigned int child = 0; child != 4; ++child)
    {
        wrappingChildren[child].currentPosition.y =
            static_cast<float>(child * 30);
        wrappingChildren[child].currentColour.red =
            static_cast<unsigned char>(child + 1);
        wrappingChildren[child].currentColour.alpha = 255;
    }
    FableUiFrontEndListScrollPlan appliedScrollPlan = {};
    if (!FableApplyUiFrontEndListScroll(
            true,
            true,
            64,
            wrappingChildren,
            4,
            4,
            1,
            &appliedScrollPlan) ||
        appliedScrollPlan.selectedChild != 2 ||
        wrappingChildren[1].currentState != 4 ||
        wrappingChildren[2].currentState != 3 ||
        wrappingChildren[0].currentColour.red != 2 ||
        wrappingChildren[1].currentColour.red != 3 ||
        wrappingChildren[2].currentColour.red != 4 ||
        wrappingChildren[3].currentColour.red != 1 ||
        wrappingChildren[0].currentPosition.y != 0.0f ||
        wrappingChildren[3].currentPosition.y != 90.0f)
    {
        return 29;
    }

    FableUiRuntimeListChild boundedChildren[4] = {};
    for (unsigned int child = 0; child != 4; ++child)
    {
        boundedChildren[child].currentPosition.y =
            static_cast<float>(child * 30);
        boundedChildren[child].currentColour.red =
            static_cast<unsigned char>(child + 1);
    }
    if (!FableApplyUiFrontEndListScroll(
            true,
            false,
            64,
            boundedChildren,
            4,
            4,
            1,
            &appliedScrollPlan) ||
        boundedChildren[0].currentPosition.y != -30.0f ||
        boundedChildren[3].currentPosition.y != 60.0f ||
        boundedChildren[0].currentColour.red != 255 ||
        boundedChildren[0].currentColour.alpha != 126 ||
        boundedChildren[1].currentColour.alpha != 190 ||
        boundedChildren[2].currentColour.alpha != 255 ||
        boundedChildren[3].currentColour.alpha != 190)
    {
        return 30;
    }

    FableUiRuntimeListChild boundarySentinel[2] = {};
    boundarySentinel[0].currentState = 99;
    if (
        FableApplyUiFrontEndListScroll(
            false,
            false,
            64,
            boundarySentinel,
            2,
            2,
            0,
            &appliedScrollPlan) ||
        boundarySentinel[0].currentState != 99 ||
        !appliedScrollPlan.blockedAtBoundary)
    {
        return 31;
    }

    if (
        FableMapUiControllerState(
            0x8000,
            0x8000,
            0xFFFF,
            0) != 0 ||
        FableMapUiControllerState(
            0x2000,
            0xE000,
            0xFFFF,
            0) !=
            (FableUiControllerLeft | FableUiControllerDown) ||
        FableMapUiControllerState(
            0x8000,
            0x8000,
            4500,
            0) !=
            (FableUiControllerUp | FableUiControllerRight) ||
        FableMapUiControllerState(
            0x8000,
            0x8000,
            0xFFFF,
            (1UL << 0) | (1UL << 6)) !=
            (FableUiControllerAccept | FableUiControllerBack))
    {
        return 32;
    }

    const fable_u32 mainMenuActions[
        FableFrontendMainMenuVisibleRows] = {
        66, 16, 297, 67, 321, 314
    };
    for (
        fable_u32 row = 0;
        row != FableFrontendMainMenuVisibleRows;
        ++row)
    {
        if (
            FableGetVisualFrontendMainMenuAction(row) !=
            mainMenuActions[row])
        {
            return 33;
        }
    }
    if (FableGetVisualFrontendMainMenuAction(7) != 0)
        return 33;

    const char* manualSaveNames[5] = {
        "Manual - Save1",
        "",
        0,
        "Manual - Save4",
        "Manual - Save5"
    };
    const bool manualPrimaryValid[5] = {
        true, true, true, false, true
    };
    const bool manualCompanionValid[5] = {
        true, true, true, true, false
    };
    FableUiSaveBrowserRow saveRows[3] = {};
    const fable_u32 logicalSaveRows =
        FablePlanVisualFrontendSaveRows(
            "AutoSave",
            true,
            true,
            manualSaveNames,
            manualPrimaryValid,
            manualCompanionValid,
            5,
            saveRows,
            3);
    if (
        logicalSaveRows != 4 ||
        saveRows[0].filename == 0 ||
        saveRows[0].filename[0] != 'A' ||
        saveRows[0].positionY != 0 ||
        saveRows[0].action != FableUiSaveBrowserLoadAction ||
        saveRows[1].filename != manualSaveNames[0] ||
        saveRows[1].positionY != 30 ||
        saveRows[1].action != FableUiSaveBrowserLoadAction ||
        saveRows[2].filename != manualSaveNames[3] ||
        saveRows[2].positionY != 60 ||
        saveRows[2].action != FableUiSaveBrowserInvalidAction ||
        FablePlanVisualFrontendSaveRows(
            "",
            false,
            false,
            manualSaveNames,
            0,
            0,
            5,
            0,
            0) != 3)
    {
        return 36;
    }

    fable_u32 previousControllerState = 0;
    const fable_u32 firstControllerPress =
        FableConsumeUiControllerPressed(
            FableUiControllerUp | FableUiControllerAccept,
            &previousControllerState);
    const fable_u32 heldControllerPress =
        FableConsumeUiControllerPressed(
            FableUiControllerUp | FableUiControllerAccept,
            &previousControllerState);
    const fable_u32 changedControllerPress =
        FableConsumeUiControllerPressed(
            FableUiControllerBack,
            &previousControllerState);
    const fable_u32 releasedControllerPress =
        FableConsumeUiControllerPressed(
            0,
            &previousControllerState);
    const fable_u32 reconnectedControllerPress =
        FableConsumeUiControllerPressed(
            FableUiControllerAccept,
            &previousControllerState);
    if (
        firstControllerPress !=
            (FableUiControllerUp | FableUiControllerAccept) ||
        heldControllerPress != 0 ||
        changedControllerPress != FableUiControllerBack ||
        releasedControllerPress != 0 ||
        reconnectedControllerPress != FableUiControllerAccept ||
        previousControllerState != FableUiControllerAccept ||
        FableConsumeUiControllerPressed(
            FableUiControllerAccept,
            0) != 0)
    {
        return 34;
    }

    FableUiControllerRepeatState controllerRepeat = {};
    previousControllerState = 0;
    if (
        FableConsumeUiControllerActions(
            FableUiControllerDown,
            1000,
            &previousControllerState,
            &controllerRepeat) != FableUiControllerDown ||
        controllerRepeat.lastMovement != FableUiControllerDown ||
        controllerRepeat.lastSelectionTimeMs != 1000 ||
        controllerRepeat.isRepeating ||
        FableConsumeUiControllerActions(
            FableUiControllerDown,
            1499,
            &previousControllerState,
            &controllerRepeat) != 0 ||
        FableConsumeUiControllerActions(
            FableUiControllerDown,
            1500,
            &previousControllerState,
            &controllerRepeat) != FableUiControllerDown ||
        !controllerRepeat.isRepeating ||
        FableConsumeUiControllerActions(
            FableUiControllerDown,
            1599,
            &previousControllerState,
            &controllerRepeat) != 0 ||
        FableConsumeUiControllerActions(
            FableUiControllerDown,
            1600,
            &previousControllerState,
            &controllerRepeat) != FableUiControllerDown ||
        FableConsumeUiControllerActions(
            0,
            1601,
            &previousControllerState,
            &controllerRepeat) != 0 ||
        controllerRepeat.lastMovement != 0 ||
        FableConsumeUiControllerActions(
            FableUiControllerDown | FableUiControllerAccept,
            1602,
            &previousControllerState,
            &controllerRepeat) !=
            (FableUiControllerDown | FableUiControllerAccept))
    {
        return 35;
    }

    char verificationCommand[] = "--verify-visual-resource";
    const long result = FableRunVisualBootCheckpoint(
        GetModuleHandleA(0),
        verificationCommand,
        0);
    if (result != 0)
        return static_cast<int>(result);

    // --- Real save enumeration: walk a temp Saves dir with three profiles ---
    {
        _wmkdir(L".\\_savetest");
        _wmkdir(L".\\_savetest\\Alpha");   // AutoSave + companion  -> both valid
        _wmkdir(L".\\_savetest\\Beta");     // AutoSave only         -> primary only
        _wmkdir(L".\\_savetest\\Empty");    // no save parts         -> skipped
        FILE* part = _wfopen(L".\\_savetest\\Alpha\\AutoSave", L"wb");
        if (part) { fputc('x', part); fclose(part); }
        part = _wfopen(L".\\_savetest\\Alpha\\AutoSave.qs", L"wb");
        if (part) { fputc('x', part); fclose(part); }
        part = _wfopen(L".\\_savetest\\Beta\\AutoSave", L"wb");
        if (part) { fputc('x', part); fclose(part); }

        FableUiSaveProfile profiles[8] = {};
        const fable_u32 profileCount =
            FableEnumerateVisualFrontendSaves(L".\\_savetest", profiles, 8);
        bool okAlpha = false;
        bool okBeta = false;
        bool sawEmpty = false;
        for (fable_u32 i = 0; i != profileCount; ++i)
        {
            if (wcscmp(profiles[i].name, L"Alpha") == 0 &&
                profiles[i].primaryValid && profiles[i].companionValid)
                okAlpha = true;
            if (wcscmp(profiles[i].name, L"Beta") == 0 &&
                profiles[i].primaryValid && !profiles[i].companionValid)
                okBeta = true;
            if (wcscmp(profiles[i].name, L"Empty") == 0)
                sawEmpty = true;
        }

        _wremove(L".\\_savetest\\Alpha\\AutoSave");
        _wremove(L".\\_savetest\\Alpha\\AutoSave.qs");
        _wremove(L".\\_savetest\\Beta\\AutoSave");
        _wrmdir(L".\\_savetest\\Alpha");
        _wrmdir(L".\\_savetest\\Beta");
        _wrmdir(L".\\_savetest\\Empty");
        _wrmdir(L".\\_savetest");

        if (profileCount != 2 || !okAlpha || !okBeta || sawEmpty)
            return 60;
    }

    printf("FABLETLC_VISUAL_BOOT_BEHAVIOR PASS\n");
    return 0;
}
