#pragma once

#include "rebuild_abi.h"

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
    const void* buttonRightPixels);

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
void FABLE_FASTCALL FableSetVisualFrontendDetailScreen(fable_u32 screen);
void FABLE_FASTCALL FableSetVisualFrontendDetailOptionValue(
    fable_u32 screen,
    fable_u32 row,
    fable_u32 value);
void FABLE_FASTCALL FableSetVisualFrontendRedefineHover(fable_u32 hover);
void FABLE_FASTCALL FableSetVisualFrontendRedefineResetHover(
    fable_u32 hover);
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

bool FABLE_FASTCALL FablePresentVisualD3D9Black();

bool FABLE_FASTCALL FableResizeVisualD3D9(
    fable_i32 backBufferWidth,
    fable_i32 backBufferHeight);

void FABLE_FASTCALL FableShutdownVisualD3D9();

bool FABLE_FASTCALL FableIsVisualD3D9Active();

bool FABLE_FASTCALL FableWasVisualD3D9Presented();
