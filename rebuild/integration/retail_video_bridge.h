#pragma once

#include "rebuild_abi.h"

enum FableRetailVideoProcessResult
{
    FableRetailVideoPending = 0,
    FableRetailVideoCompleted = 1,
    FableRetailVideoFailed = 2
};

bool FABLE_FASTCALL FableStartRetailVideo(
    void* parentWindow,
    void* instance,
    const char* explicitMoviePath);

void FABLE_FASTCALL FableSetRetailVideoPreferUpscaled(
    bool preferUpscaled);

void FABLE_FASTCALL FableResizeRetailVideo(
    fable_i32 clientWidth,
    fable_i32 clientHeight);

bool FABLE_FASTCALL FableRetailVideoHasAdvanced();

FableRetailVideoProcessResult FABLE_FASTCALL
FableProcessRetailVideo();

void FABLE_FASTCALL FableSkipRetailVideo();

bool FABLE_FASTCALL FableIsRetailVideoActive();

bool FABLE_FASTCALL FableIsRetailVideoUsingUpscaledSource();

const char* FABLE_FASTCALL FableGetRetailVideoStatus();

void FABLE_FASTCALL FableShutdownRetailVideo();
