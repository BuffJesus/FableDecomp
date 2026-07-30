#pragma once

#include "rebuild_abi.h"

typedef void (FABLE_STDCALL *FableConfigReleaseFunction)();
typedef void* (FABLE_STDCALL *FableGetProcAddressFunction)(
    void* module,
    const char* name);
typedef int (FABLE_STDCALL *FableFreeLibraryFunction)(void* module);
typedef void* (FABLE_STDCALL *FableLoadLibraryAFunction)(const char* name);
typedef int (FABLE_STDCALL *FableMessageBoxAFunction)(
    void* window,
    const char* text,
    const char* caption,
    fable_u32 type);

extern void* g_FableConfigDetectionModule_013B7C8C;
extern FableGetProcAddressFunction g_FableGetProcAddress_0143FE44;
extern FableFreeLibraryFunction g_FableFreeLibrary_0143FE3C;
extern FableLoadLibraryAFunction g_FableLoadLibraryA_0143FE40;
extern FableMessageBoxAFunction g_FableMessageBoxA_014402C4;

bool FABLE_FASTCALL GFConfigDetection();
bool FABLE_FASTCALL GFFreeConfigDetection();
