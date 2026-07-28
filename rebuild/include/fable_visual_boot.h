#pragma once

#include "fable_boot.h"

// Authored reconstruction checkpoint, not a recovered retail renderer.
// It proves that the matched WinMain and reconstructed GFMain phases can hand
// control to a real Win32 window while the retail display path is recovered.
void FABLE_FASTCALL FableSetVisualProgressDisplayState(
    bool present,
    bool active);

bool FABLE_FASTCALL FableIsRetailVisualAssetEmbedded();

bool FABLE_FASTCALL FableShouldPlayBootVideos(const char* commandLine);

struct FableUiVector2
{
    float x;
    float y;
};

struct FableUiResolvedTransform
{
    FableUiVector2 position;
    FableUiVector2 truePosition;
    FableUiVector2 scale;
    FableUiVector2 trueScale;
};

// Recovered from CTable::Draw @ 0x00550DC0. The retail component keeps
// ordinary and UI-scaled ("true") transforms in parallel.
void FABLE_FASTCALL FableResolveUiComponentTransform(
    const FableUiVector2* localPosition,
    const FableUiVector2* localScale,
    const FableUiResolvedTransform* parent,
    FableUiResolvedTransform* resolved);

// Recovered from CTable::Draw's generated-child loop. Generated component
// positions use the ordinary table origin directly, while their true render
// positions scale the local offset by CManager::GetUIScale().
void FABLE_FASTCALL FableResolveUiGeneratedChildOrigins(
    const FableUiVector2* childLocalPosition,
    const FableUiVector2* uiScale,
    const FableUiResolvedTransform* table,
    FableUiVector2* ordinaryPosition,
    FableUiVector2* truePosition);

struct FableUiTableLineSecondary
{
    FableUiVector2 position;
    fable_i32 resourceKey;
};

struct FableUiTableLinePlan
{
    FableUiVector2 primaryPosition;
    FableUiVector2 primaryZoom;
    FableUiVector2 finalCursor;
    unsigned int secondaryCount;
    unsigned int generatedChildCount;
};

enum FableUiTableSpritePlanKind
{
    FableUiTableSpriteCorner = 0,
    FableUiTableSpriteHorizontalLine = 1,
    FableUiTableSpriteVerticalLine = 2
};

struct FableUiTableSpritePlanInput
{
    unsigned int availableSpriteMask;
    unsigned int horizontalSeparatorCount;
    unsigned int verticalSeparatorCount;
    unsigned int horizontalRepeatCount;
    unsigned int verticalRepeatCount;
    fable_i32 horizontalInteriorResourceKeyOffset;
    fable_i32 verticalInteriorResourceKeyOffset;
    unsigned int topRightCornerVisible;
    unsigned int bottomLeftCornerVisible;
    unsigned int bottomRightCornerVisible;
};

struct FableUiTableSpritePlanEvent
{
    unsigned int kind;
    unsigned int primarySpriteKey;
    fable_i32 secondarySpriteKey;
    unsigned int separatorIndex;
    unsigned int repeatCount;
    fable_i32 resourceKeyOffset;
};

struct FableUiTableSpritePlan
{
    unsigned int logicalEventCount;
    unsigned int writtenEventCount;
};

struct FableUiTableGeometryInput
{
    unsigned int availableSpriteMask;
    unsigned long horizontalRepeatCount;
    unsigned long verticalRepeatCount;
    FableUiVector2 horizontalEdgeSize;
    FableUiVector2 verticalEdgeSize;
    FableUiVector2 cornerSize[4];
    const unsigned long* horizontalSeparatorTiles;
    unsigned int horizontalSeparatorCount;
    const unsigned long* verticalSeparatorTiles;
    unsigned int verticalSeparatorCount;
};

struct FableUiTableInteriorLine
{
    FableUiVector2 position;
    unsigned long repeatCount;
    fable_i32 resourceKeyOffset;
};

struct FableUiTableGeometryPlan
{
    FableUiVector2 cornerPosition[4];
    unsigned int emittedCornerMask;
    FableUiVector2 topLineStart;
    FableUiVector2 bottomLineStart;
    FableUiVector2 leftLineStart;
    FableUiVector2 rightLineStart;
    float horizontalExtent;
    float verticalExtent;
    unsigned int logicalHorizontalInteriorCount;
    unsigned int writtenHorizontalInteriorCount;
    unsigned int logicalVerticalInteriorCount;
    unsigned int writtenVerticalInteriorCount;
};

struct FableUiListSelectionPlan
{
    long selectedChild;
    long firstVisibleChild;
    long lastVisibleChild;
    FableUiVector2 scrollDelta;
    bool requestsScroll;
};

struct FableUiFrontEndListOffsetPlan
{
    float interpolationScalar;
    FableUiVector2 positionOffset;
};

struct FableUiListRecomputedChild
{
    FableUiVector2 position;
    unsigned char alpha;
};

struct FableUiListRecomputeInput
{
    unsigned int childCount;
    FableUiVector2 definitionPositionOffset;
    unsigned char alphaFalloff;
    float initialAlpha;
    FableUiVector2 initialPosition;
};

struct FableUiListRecomputePlan
{
    unsigned int logicalChildCount;
    unsigned int writtenChildCount;
    unsigned int positionStateMask;
    unsigned int alphaStateMask;
    FableUiVector2 finalPosition;
    float finalAlpha;
};

struct FableUiFrontEndListScrollPlan
{
    long previousSelectedChild;
    long selectedChild;
    FableUiVector2 rowTranslation;
    unsigned long previousChildState;
    unsigned long selectedChildState;
    unsigned int logicalAlphaCount;
    unsigned int writtenAlphaCount;
    bool moved;
    bool blockedAtBoundary;
    bool requestsInvalidAction;
    bool requestsMoveAction;
};

// Recovered from CTable::ConstructHorizontalLine @ 0x005518E0 and
// ConstructVerticalLine @ 0x00551BC0. The primary clone receives the repeat
// count as its axis zoom. A secondary clone is emitted only when the table's
// resource-index vector contains (iteration - resourceKeyOffset), and only
// emitted clones advance the running cursor. The generated-child count
// includes the one primary clone and is independent of output truncation.
void FABLE_FASTCALL FablePlanUiTableLine(
    bool vertical,
    const FableUiVector2* start,
    const FableUiVector2* primarySize,
    const FableUiVector2* secondarySize,
    unsigned long repetitions,
    unsigned long resourceKeyOffset,
    const fable_i32* resourceKeys,
    unsigned int resourceKeyCount,
    FableUiTableLineSecondary* secondaryOutput,
    unsigned int secondaryCapacity,
    FableUiTableLinePlan* plan);

// Recovered call ordering from CTable::ConstructSpritesToDraw @ 0x00551EA0.
// Corner keys 0..3 precede horizontal top/interior/bottom lines (4/12/5),
// followed by vertical left/interior/right lines (6/12/7). Tee companions
// use the exact ETableSprites keys decoded from frontend.bin.
void FABLE_FASTCALL FablePlanUiTableSpriteComposition(
    const FableUiTableSpritePlanInput* input,
    FableUiTableSpritePlanEvent* eventOutput,
    unsigned int eventOutputCapacity,
    FableUiTableSpritePlan* plan);

// Exact cursor and separator geometry recovered from
// CTable::ConstructSpritesToDraw @ 0x00551EA0. The tiled extents describe the
// inner span between corners. Corner dimensions then move the four edge
// cursors, and separator tile indices are converted back into pixel positions.
void FABLE_FASTCALL FablePlanUiTableGeometry(
    const FableUiTableGeometryInput* input,
    FableUiTableInteriorLine* horizontalInteriorOutput,
    unsigned int horizontalInteriorCapacity,
    FableUiTableInteriorLine* verticalInteriorOutput,
    unsigned int verticalInteriorCapacity,
    FableUiTableGeometryPlan* plan);

// Recovered from CList::SetSelectedChild @ 0x005360B1. The requested index is
// stored even when invalid. A valid child outside the first contiguous visible
// window requests a vertical 25-pixel correction per displaced row.
void FABLE_FASTCALL FablePlanUiListSelection(
    long requestedChild,
    const unsigned char* visibleChildren,
    unsigned int childCount,
    FableUiListSelectionPlan* plan);

// Recovered from CFrontEndList::InitialiseOffsets @ 0x0054C480. Unlike the
// generic CList implementation, this override ignores child count, sets the
// interpolation scalar to one, and applies no additional position offset.
void FABLE_FASTCALL FablePlanUiFrontEndListInitialOffsets(
    unsigned long childCount,
    FableUiFrontEndListOffsetPlan* plan);

// Recovered from generic CList::InitialiseOffsets @ 0x00536B4F. Odd lists
// begin at childCount/2 offsets before the centre; even lists use one fewer.
// The optional parent flag zeros only the initial position.
void FABLE_FASTCALL FablePlanUiListInitialOffsets(
    unsigned long childCount,
    const FableUiVector2* definitionPositionOffset,
    unsigned char alphaFalloff,
    bool zeroInitialPosition,
    FableUiFrontEndListOffsetPlan* plan);

// Recovered from CList::DoRecomputeOffsets @ 0x0053C332. States
// 0/1/4/5/6 receive each generated position, states 1/4/5 receive the
// clamped x87-converted alpha, and the position/alpha increments reflect at
// the list centre.
void FABLE_FASTCALL FablePlanUiListRecomputeOffsets(
    const FableUiListRecomputeInput* input,
    FableUiListRecomputedChild* childOutput,
    unsigned int childOutputCapacity,
    FableUiListRecomputePlan* plan);

// Recovered from CFrontEndList::ScrollUp @ 0x0054C4C0 and ScrollDown
// @ 0x0054C810. Lists with zero or one child reject movement. A list which
// stops at its ends also rejects movement at the relevant boundary; otherwise
// selection wraps. Successful movement changes the old/new child states to
// 4/3 and rotates row positions by exactly 30 pixels. When end stopping is
// active, child alpha falls off with distance from the new selection.
void FABLE_FASTCALL FablePlanUiFrontEndListScroll(
    bool scrollDown,
    unsigned int childCount,
    long selectedChild,
    bool stopsAtEnds,
    unsigned char alphaFalloff,
    unsigned char* childAlphaOutput,
    unsigned int childAlphaCapacity,
    FableUiFrontEndListScrollPlan* plan);

long FABLE_FASTCALL FableRunVisualBootCheckpoint(
    FableInstanceHandle instance,
    char* commandLine,
    int showCommand);
