#pragma once

#include "rebuild_abi.h"

bool FABLE_FASTCALL FableStartRetailVideo(
    void* parentWindow,
    void* instance,
    const char* explicitMoviePath);

void FABLE_FASTCALL FableResizeRetailVideo(
    fable_i32 clientWidth,
    fable_i32 clientHeight);

bool FABLE_FASTCALL FableRetailVideoHasAdvanced();

bool FABLE_FASTCALL FableIsRetailVideoActive();

const char* FABLE_FASTCALL FableGetRetailVideoStatus();

void FABLE_FASTCALL FableShutdownRetailVideo();
