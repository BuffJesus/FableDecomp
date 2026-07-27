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
    const void* titlePixels);

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
