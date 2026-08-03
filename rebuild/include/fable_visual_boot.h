#pragma once

#include "fable_boot.h"
#include "fable_ui_state.h"

// Authored reconstruction checkpoint, not a recovered retail renderer.
// It proves that the matched WinMain and reconstructed GFMain phases can hand
// control to a real Win32 window while the retail display path is recovered.
void FABLE_FASTCALL FableSetVisualProgressDisplayState(
    bool present,
    bool active);

bool FABLE_FASTCALL FableIsRetailVisualAssetEmbedded();

bool FABLE_FASTCALL FableShouldPlayBootVideos(const char* commandLine);

// Offline retail projection of UI_FRONTEND_LIST_MAIN_MENU.  The compiled
// definition retains the optional LIVE child; this checkpoint exposes the six
// rows present in the supplied offline retail capture.
fable_u32 FABLE_FASTCALL FableGetVisualFrontendMainMenuAction(
    fable_u32 row);

enum FableUiSaveBrowserAction
{
    FableUiSaveBrowserLoadAction = 0x11,
    FableUiSaveBrowserInvalidAction = 0xDC
};

struct FableUiSaveBrowserRow
{
    const char* filename;
    fable_i32 positionY;
    fable_u32 action;
};

// Recovered RefreshAvailableSavedGamesForProfile row contract: append the
// non-empty autosave first, then non-empty manual slots in ascending order;
// rows advance by 30 pixels and dispatch load only when both save parts pass
// validation. The return value is the logical row count, even when output
// capacity is smaller.
fable_u32 FABLE_FASTCALL FablePlanVisualFrontendSaveRows(
    const char* autosaveFilename,
    bool autosavePrimaryValid,
    bool autosaveCompanionValid,
    const char* const* manualFilenames,
    const bool* manualPrimaryValid,
    const bool* manualCompanionValid,
    fable_u32 manualSlotCount,
    FableUiSaveBrowserRow* rows,
    fable_u32 rowCapacity);

// Real save enumeration: each Fable TLC save is a profile subfolder of
// <MyDocuments>\My Games\Fable\Saves\ containing an "AutoSave" primary blob (and
// optional "AutoSave.qs" companion). This walks that directory and reports the
// on-disk profiles + validity so the Saved Games browser can be driven by the
// player's actual saves instead of baked placeholders. A profile is
// primary-valid when its AutoSave file exists and is non-empty; companion-valid
// when AutoSave.qs likewise exists non-empty. Returns the profile count found
// (even past outCapacity). names[i] holds the profile folder name (UTF-16).
struct FableUiSaveProfile
{
    wchar_t name[64];
    bool primaryValid;
    bool companionValid;
};

fable_u32 FABLE_FASTCALL FableEnumerateVisualFrontendSaves(
    const wchar_t* saveDirectory,
    FableUiSaveProfile* out,
    fable_u32 outCapacity);

// Runtime profile-directory enumeration for the Change Profile screen. Unlike
// save enumeration, this includes profile folders even when they contain no
// AutoSave blob; the frontend decides separately whether to show the empty
// profile branch. Names are returned in the same sorted, display-ready ASCII
// form consumed by the checkpoint's retail glyph path.
struct FableUiProfileName
{
    char name[64];
};

fable_u32 FABLE_FASTCALL FableEnumerateVisualFrontendProfiles(
    const wchar_t* saveDirectory,
    FableUiProfileName* out,
    fable_u32 outCapacity);

// CFrontEndManager::Action @ 0x0059A238 profile action vocabulary.  These
// values are kept separate from the renderer's presentation modes so the
// checkpoint can test/route the recovered manager actions without fabricating
// profile storage.
enum FableUiFrontendProfileMode
{
    FableUiFrontendProfileModeNormal = 0,
    FableUiFrontendProfileModeDelete = 1,
    FableUiFrontendProfileModeEmpty = 2,
    FableUiFrontendProfileModeNew = 3,
    FableUiFrontendProfileModeDeleteConfirm = 4
};

enum FableUiFrontendProfileInput
{
    FableUiFrontendProfileInputActivate = 0,
    FableUiFrontendProfileInputDeleteList = 1,
    FableUiFrontendProfileInputDeleteConfirm = 2,
    FableUiFrontendProfileInputKeyboardCancel = 3,
    FableUiFrontendProfileInputKeyboardConfirm = 4
};

struct FableUiFrontendProfileActionPlan
{
    fable_u32 action;
    bool dispatch;
};

// Maps the authored profile controls to the exact action IDs observed in
// CFrontEndManager::Action.  This is an action-boundary helper only: it does
// not create, delete, load, or otherwise mutate a profile.
void FABLE_FASTCALL FablePlanVisualFrontendProfileAction(
    fable_u32 mode,
    fable_u32 selection,
    fable_u32 profileCount,
    fable_u32 input,
    FableUiFrontendProfileActionPlan* plan);

enum FableUiControllerActionMask
{
    FableUiControllerUp = 1 << 0,
    FableUiControllerDown = 1 << 1,
    FableUiControllerLeft = 1 << 2,
    FableUiControllerRight = 1 << 3,
    FableUiControllerAccept = 1 << 4,
    FableUiControllerBack = 1 << 5
};

// Compatibility bridge for the period WinMM joystick surface. Directional
// state combines POV and analogue-axis thresholds; buttons 1/8 accept and
// buttons 2/7 go back.
fable_u32 FABLE_FASTCALL FableMapUiControllerState(
    fable_u32 xPosition,
    fable_u32 yPosition,
    fable_u32 pointOfView,
    fable_u32 buttons);

fable_u32 FABLE_FASTCALL FableConsumeUiControllerPressed(
    fable_u32 currentState,
    fable_u32* previousState);

struct FableUiControllerRepeatState
{
    fable_u32 lastMovement;
    fable_u32 lastSelectionTimeMs;
    bool isRepeating;
};

// Mirrors CFrontendGameComponent::ChangeSelection @ 0x00494380:
// a changed direction fires immediately, then repeats after the initial
// delay and subsequently at the shorter held delay.
fable_u32 FABLE_FASTCALL FableConsumeUiControllerActions(
    fable_u32 currentState,
    fable_u32 currentTimeMs,
    fable_u32* previousState,
    FableUiControllerRepeatState* repeatState);

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

struct FableUiComponentPrototype
{
    unsigned long definitionId;
    FableUiVector2 size;
};

struct FableUiComponentState
{
    FableUiVector2 position;
    FableUiVector2 zoom;
};

struct FableUiRuntimeComponent
{
    unsigned long definitionId;
    unsigned int sourceSpriteKey;
    unsigned int initialised;
    unsigned int stateMask;
    FableUiVector2 size;
    FableUiComponentState state[1];
};

struct FableUiComponentLifetimeCounters
{
    unsigned int componentAllocations;
    unsigned int componentDeletions;
    unsigned int controlAllocations;
    unsigned int controlDeletions;
    unsigned int retains;
    unsigned int releases;
};

struct FableUiCountedReference
{
    unsigned int referenceCount;
    FableUiRuntimeComponent* component;
    FableUiComponentLifetimeCounters* counters;
};

struct FableUiCountedComponent
{
    FableUiRuntimeComponent* component;
    FableUiCountedReference* reference;
};

struct FableUiGeneratedComponentVector
{
    FableUiCountedComponent* values;
    unsigned int size;
    unsigned int capacity;
    FableUiComponentLifetimeCounters* counters;
};

struct FableUiTableRuntimeInput
{
    FableUiTableGeometryInput geometry;
    FableUiComponentPrototype sprite[13];
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

struct FableUiRuntimeStateMap
{
    CUIStateRecoveredLayout state[7];
    unsigned int stateMask;
};

struct FableUiRuntimeListChild
{
    FableUiRuntimeStateMap states;
    unsigned long currentState;
    FableUiVector2 currentPosition;
    FableUiStateColour currentColour;
};

enum FableUiFrontEndSoundRequest
{
    FableUiFrontEndSoundNone = 0,
    FableUiFrontEndSoundUpDown = 1,
    FableUiFrontEndSoundError = 2
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
    unsigned long soundRequest;
    unsigned long soundCriteriaDefinitionOffset;
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

// Live ownership step recovered from CTable::ConstructSpritesToDraw,
// ConstructHorizontalLine, and ConstructVerticalLine. Every emitted sprite is
// cloned from its definition-bearing prototype, initialised, receives a state
// zero position (and line zoom), and is appended through retail-style
// CCountedPointer copy/release semantics. The output vector owns one reference
// to each component on success.
bool FABLE_FASTCALL FableConstructUiTableComponents(
    const FableUiTableRuntimeInput* input,
    FableUiGeneratedComponentVector* generated);

// Releases every counted component held by the generated-child vector. This
// mirrors destruction/clear of CTable's final generated component vector and
// resets the vector to an empty reusable state.
void FABLE_FASTCALL FableReleaseUiGeneratedComponents(
    FableUiGeneratedComponentVector* generated);

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

// Applies the recovered DoRecomputeOffsets plan to real CUIState-shaped maps.
// Positions are written to states 0/1/4/5/6, alpha to 1/4/5, state 1 is
// adopted as the child's live position/colour, and each completed state map
// is copied into the list's ScrollingChildrenStates-style snapshot vector.
// Capacity failure is transactional.
bool FABLE_FASTCALL FableApplyUiListRecomputedStates(
    const FableUiListRecomputeInput* input,
    FableUiRuntimeListChild* children,
    unsigned int childCapacity,
    FableUiRuntimeStateMap* stateSnapshots,
    unsigned int stateSnapshotCapacity,
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

// Retail CList::ProcessEvent @ 0x0053673b dispatches semantic list events
// 0 (up) and 1 (down) to its scroll virtuals.  Keep that event vocabulary at
// the frontend boundary instead of making callers invent a second direction
// protocol.  The planner remains the isolated state/layout adapter until the
// live CList object is linked into this checkpoint.
enum FableUiFrontEndListEvent
{
    FableUiFrontEndListEventUp = 0,
    FableUiFrontEndListEventDown = 1
};

void FABLE_FASTCALL FablePlanUiFrontEndListProcessEvent(
    unsigned int event,
    unsigned int childCount,
    long selectedChild,
    bool stopsAtEnds,
    unsigned char alphaFalloff,
    unsigned char* childAlphaOutput,
    unsigned int childAlphaCapacity,
    FableUiFrontEndListScrollPlan* plan);

// Applies the recovered CFrontEndList ScrollUp/ScrollDown transaction to live
// children. Wrapping lists rotate colour ownership and change the old/new
// states to 4/3 without translating their authored rows. Non-wrapping lists
// additionally apply the 30-pixel position step and distance alpha falloff.
// Rejected/boundary movement leaves the children unchanged.
bool FABLE_FASTCALL FableApplyUiFrontEndListScroll(
    bool scrollDown,
    bool wrapping,
    unsigned char alphaFalloff,
    FableUiRuntimeListChild* children,
    unsigned int childCount,
    unsigned int childCapacity,
    long selectedChild,
    FableUiFrontEndListScrollPlan* plan);

long FABLE_FASTCALL FableRunVisualBootCheckpoint(
    FableInstanceHandle instance,
    char* commandLine,
    int showCommand);
