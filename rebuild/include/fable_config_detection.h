#pragma once

#include "rebuild_abi.h"

class ConfigDetection;
struct CConfigOptions;

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

// fablelib/main.cpp globals (FableWin.pdb names): the DLL's detection result
// (0x013BD6E8) and the engine options it seeds (0x01375070).
extern ConfigDetection* pConfig;
extern CConfigOptions GConfigOptions;

// bbblibrary/errordialog.cpp: DisplayErrorDialog @ 0x0099DCB0 (Error points
// at {code, extra}), InitExternalStrings @ 0x0099E260.
extern "C" int FABLE_STDCALL DisplayErrorDialog(int* Error, int ForceExit);
extern "C" void FABLE_FASTCALL InitExternalStrings();
