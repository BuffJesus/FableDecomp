#include "fable_visual_boot.h"
#include "fable_video_system.h"

#include <stdio.h>

extern "C" __declspec(dllimport)
FableInstanceHandle FABLE_STDCALL GetModuleHandleA(const char* moduleName);

int main()
{
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
        !frontEndListScrollPlan.requestsInvalidAction)
    {
        return 21;
    }

    char verificationCommand[] = "--verify-visual-resource";
    const long result = FableRunVisualBootCheckpoint(
        GetModuleHandleA(0),
        verificationCommand,
        0);
    if (result != 0)
        return static_cast<int>(result);

    printf("FABLETLC_VISUAL_BOOT_BEHAVIOR PASS\n");
    return 0;
}
