#pragma once

#include "rebuild_abi.h"

// UI_FRONTEND_LIST_FOR_PROFILES serializes PositionOffsetY=28.  Keep the
// runtime hit-test and glyph renderer on the same authored row cadence.
enum FableFrontendProfileLayout
{
    FableFrontendProfileRowStep = 28,
    FableFrontendProfileListHeight = 260
};

bool FABLE_FASTCALL FableInitialiseVisualD3D9(
    void* window,
    fable_i32 backBufferWidth,
    fable_i32 backBufferHeight,
    fable_i32 artworkWidth,
    fable_i32 artworkHeight,
    fable_i32 artworkPitch,
    fable_u32 artworkBitsPerPixel,
    const void* artworkPixels,
    fable_i32 titleWidth,
    fable_i32 titleHeight,
    fable_i32 titlePitch,
    fable_u32 titleBitsPerPixel,
    const void* titlePixels,
    fable_i32 forestWidth,
    fable_i32 forestHeight,
    fable_i32 forestPitch,
    fable_u32 forestBitsPerPixel,
    const void* forestPixels,
    fable_i32 sunbeamWidth,
    fable_i32 sunbeamHeight,
    fable_i32 sunbeamPitch,
    fable_u32 sunbeamBitsPerPixel,
    const void* sunbeamPixels,
    fable_i32 menuWidth,
    fable_i32 menuHeight,
    fable_i32 menuPitch,
    fable_u32 menuBitsPerPixel,
    const void* menuPixels,
    fable_i32 coastalWidth,
    fable_i32 coastalHeight,
    fable_i32 coastalPitch,
    fable_u32 coastalBitsPerPixel,
    const void* coastalPixels,
    fable_i32 coastalSunbeamWidth,
    fable_i32 coastalSunbeamHeight,
    fable_i32 coastalSunbeamPitch,
    fable_u32 coastalSunbeamBitsPerPixel,
    const void* coastalSunbeamPixels,
    fable_i32 optionsWidth,
    fable_i32 optionsHeight,
    fable_i32 optionsPitch,
    fable_u32 optionsBitsPerPixel,
    const void* optionsPixels,
    fable_i32 helpersWidth,
    fable_i32 helpersHeight,
    fable_i32 helpersPitch,
    fable_u32 helpersBitsPerPixel,
    const void* helpersPixels,
    fable_i32 titleSegmentWidth,
    fable_i32 titleSegmentHeight,
    fable_i32 titleSegmentPitch,
    fable_u32 titleSegmentBitsPerPixel,
    const void* titleSegmentPixels,
    fable_i32 buttonLeftWidth,
    fable_i32 buttonLeftHeight,
    fable_i32 buttonLeftPitch,
    fable_u32 buttonLeftBitsPerPixel,
    const void* buttonLeftPixels,
    fable_i32 buttonMiddleWidth,
    fable_i32 buttonMiddleHeight,
    fable_i32 buttonMiddlePitch,
    fable_u32 buttonMiddleBitsPerPixel,
    const void* buttonMiddlePixels,
    fable_i32 buttonRightWidth,
    fable_i32 buttonRightHeight,
    fable_i32 buttonRightPitch,
    fable_u32 buttonRightBitsPerPixel,
    const void* buttonRightPixels,
    fable_i32 aboutWidth,
    fable_i32 aboutHeight,
    fable_i32 aboutPitch,
    fable_u32 aboutBitsPerPixel,
    const void* aboutPixels,
    fable_i32 creditsWidth,
    fable_i32 creditsHeight,
    fable_i32 creditsPitch,
    fable_u32 creditsBitsPerPixel,
    const void* creditsPixels,
    fable_i32 profilesWidth,
    fable_i32 profilesHeight,
    fable_i32 profilesPitch,
    fable_u32 profilesBitsPerPixel,
    const void* profilesPixels,
    fable_i32 spookyWidth,
    fable_i32 spookyHeight,
    fable_i32 spookyPitch,
    fable_u32 spookyBitsPerPixel,
    const void* spookyPixels,
    fable_i32 spookySunbeamWidth,
    fable_i32 spookySunbeamHeight,
    fable_i32 spookySunbeamPitch,
    fable_u32 spookySunbeamBitsPerPixel,
    const void* spookySunbeamPixels);

void FABLE_FASTCALL FableSetVisualFrontendMainMenu(bool active);
void FABLE_FASTCALL FableSetVisualFrontendMainMenuSelection(
    fable_u32 selection);
bool FABLE_FASTCALL FableScrollVisualFrontendMainMenu(
    bool scrollDown,
    fable_u32* selected,
    fable_u32* soundRequest);
void FABLE_FASTCALL FableSetVisualFrontendOptionsMenu(bool active);
void FABLE_FASTCALL FableSetVisualFrontendOptionsSelection(
    fable_u32 selection);
void FABLE_FASTCALL FableSetVisualFrontendOptionsBackHovered(bool hovered);
void FABLE_FASTCALL FableSetVisualFrontendSaveMenu(bool active);
void FABLE_FASTCALL FableSetVisualFrontendSaveSelection(
    fable_u32 selection);
void FABLE_FASTCALL FableSetVisualFrontendAboutMenu(bool active);
void FABLE_FASTCALL FableSetVisualFrontendCreditsMenu(bool active);
void FABLE_FASTCALL FableSetVisualFrontendProfilesMenu(
    bool active,
    const char* const* names,
    fable_u32 count);
void FABLE_FASTCALL FableSetVisualFrontendProfilesSelection(
    fable_u32 selection);
void FABLE_FASTCALL FableSetVisualFrontendDetailScreen(fable_u32 screen);
void FABLE_FASTCALL FableSetVisualFrontendDetailOptionValue(
    fable_u32 screen,
    fable_u32 row,
    fable_u32 value);
void FABLE_FASTCALL FableSetVisualFrontendRedefineHover(fable_u32 hover);
void FABLE_FASTCALL FableSetVisualFrontendRedefineResetHover(
    fable_u32 hover);
void FABLE_FASTCALL FableSetVisualFrontendDetailButtonHover(
    fable_u32 hover);
void FABLE_FASTCALL FableSetVisualFrontendDetailArrowHover(
    fable_u32 row,
    fable_u32 side);
void FABLE_FASTCALL FableSetVisualFrontendRedefineSelection(
    fable_u32 selection);
void FABLE_FASTCALL FableSetVisualFrontendRedefineKey(
    fable_u32 row,
    fable_u32 keyValue);
void FABLE_FASTCALL FableSetVisualFrontendQuitPrompt(bool active);
void FABLE_FASTCALL FableSetVisualFrontendQuitHover(fable_u32 hover);

bool FABLE_FASTCALL FableRenderVisualD3D9(
    fable_i32 clientWidth,
    fable_i32 clientHeight);

// Save-preview viewport (UI_VIEW_RING_SMALL) atlas sample rect.  The saved-games
// preview panel (region minimap + ring ornament, design (314,37), 256x256) is
// baked into each save cell of the shared g_OptionsTexture atlas and emitted as
// a live Render2D quad drawn after the title rule.  This helper is the single
// source of truth for the atlas UV rect the ring quad samples, so the C++ sample
// and the Python atlas bake can be verified to coincide.  Returns false on
// invalid inputs (selection >= 4 or non-positive atlas dimensions).
bool FABLE_FASTCALL FableComputeSaveViewportAtlasRect(
    fable_u32 saveSelection,
    fable_i32 optionsAtlasWidth,
    fable_i32 optionsAtlasHeight,
    float* outLeftU,
    float* outTopV,
    float* outRightU,
    float* outBottomV);

bool FABLE_FASTCALL FablePresentVisualD3D9Black();

bool FABLE_FASTCALL FableResizeVisualD3D9(
    fable_i32 backBufferWidth,
    fable_i32 backBufferHeight);

void FABLE_FASTCALL FableShutdownVisualD3D9();

bool FABLE_FASTCALL FableIsVisualD3D9Active();

bool FABLE_FASTCALL FableWasVisualD3D9Presented();
