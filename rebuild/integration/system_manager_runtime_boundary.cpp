#include "fable_gfmain.h"
#include "fable_gfuninitialise.h"

#include <string.h>

extern "C" fable_u8 g_FableDriveManagerVTable_0129AFF4 = 0;
extern "C" void* g_FableDriveManagerPointer_013CAA28 = 0;
extern "C" fable_u8 g_FableScratchBuffer_013CB498 = 0;
extern "C" fable_u8 g_FableSystemInitialiseCount_013CA608 = 0;

namespace
{
    fable_u8 g_SystemManagerStorage[0x180];
    fable_u8 g_Allocations[8][0x2C8];
    fable_u32 g_AllocationCount;
    GFMainPhase9State* g_ActiveState;

    void StoreObject(void* slot, void* object)
    {
        *static_cast<void**>(slot) = object;
    }
}

extern "C" void* FABLE_STDCALL
FableBoundaryConvertThreadToFiber(void*)
{
    return 0;
}

extern "C" long FABLE_STDCALL
FableBoundaryCoInitialize(void*)
{
    return 0;
}

extern "C" void FABLE_STDCALL
FableBoundaryInitializeCriticalSection(void*)
{
}

extern "C" void* FABLE_STDCALL
FableBoundaryGetCurrentProcess()
{
    return 0;
}

extern "C" void* (FABLE_STDCALL*
    g_FableConvertThreadToFiber_009a6610)(void*) =
        &FableBoundaryConvertThreadToFiber;
extern "C" long (FABLE_STDCALL*
    g_FableCoInitialize_009a6610)(void*) =
        &FableBoundaryCoInitialize;
extern "C" void (FABLE_STDCALL*
    g_FableInitializeCriticalSection_009a6610)(void*) =
        &FableBoundaryInitializeCriticalSection;
extern "C" void* (FABLE_STDCALL*
    g_FableGetCurrentProcess_009a6610)() =
        &FableBoundaryGetCurrentProcess;

extern "C" bool __fastcall
FableSystemPreInitialise_009a6610(void*, void*)
{
    if (g_ActiveState != 0)
        ++g_ActiveState->exactSystemPreInitialiseCalls;
    return true;
}

extern "C" void __fastcall
FableSystemWideAssign_009a6610(void*, void*, const void*)
{
}

extern "C" void __fastcall
FableSystemInitGuid_009a6610(void*, void*, const void*)
{
}

extern "C" void FableSystemDefaultErrorCallback_009a6610()
{
}

extern "C" bool __fastcall FableSystemInitWindow_009a6610(
    void*,
    void*,
    fable_u32,
    fable_u32)
{
    if (g_ActiveState != 0)
        ++g_ActiveState->exactSystemWindowInitialiseCalls;
    return true;
}

extern "C" void __fastcall
FableSystemUninitialise_009a6610(void*, void*)
{
}

extern "C" void __fastcall
FableSystemScratchInitialise_009a6610(
    void*,
    void*,
    fable_u32)
{
}

extern "C" void __fastcall
FableSystemDisplayInitCopy_009a6610(
    void*,
    void*,
    const void*)
{
}

extern "C" void* __cdecl
FableSystemOperatorNew_009a6610(fable_u32)
{
    if (g_AllocationCount >= 8)
        return 0;
    return g_Allocations[g_AllocationCount++];
}

extern "C" void* __fastcall
FableSystemDisplayConstruct_009a6610(
    void* object,
    void*,
    void*)
{
    if (g_ActiveState != 0)
        ++g_ActiveState->exactSystemDisplayConstructCalls;
    return object;
}

extern "C" void __fastcall
FableSystemDisplayReset_009a6610(
    void* slot,
    void*,
    void* object)
{
    StoreObject(slot, object);
}

extern "C" void __fastcall
FableSystemDisplayInitialise_009a6610(
    void*,
    void*,
    void*,
    fable_u32)
{
}

extern "C" void __fastcall
FableSystemCharStringDestroy_009a6610(void*, void*)
{
}

extern "C" void* __fastcall
FableSystemInputConstruct_009a6610(
    void* object,
    void*,
    const void*,
    fable_u32)
{
    if (g_ActiveState != 0)
        ++g_ActiveState->exactSystemInputConstructCalls;
    return object;
}

extern "C" void __fastcall
FableSystemInputReset_009a6610(
    void* slot,
    void*,
    void* object)
{
    StoreObject(slot, object);
}

extern "C" void __fastcall
FableSystemDriveReset_009a6610(
    void* slot,
    void*,
    void* object)
{
    StoreObject(slot, object);
}

extern "C" ESystemCode __fastcall
FableSystemDriveInitialise_009a6610(void*, void*, void*)
{
    if (g_ActiveState != 0)
        ++g_ActiveState->exactSystemDriveInitialiseCalls;
    return kSystemCodeSuccess;
}

extern "C" void* __fastcall
FableSystemProfileConstruct_009a6610(
    void* object,
    void*,
    const void*,
    fable_u32)
{
    return object;
}

extern "C" void __fastcall
FableSystemProfileReset_009a6610(
    void* slot,
    void*,
    void* object)
{
    StoreObject(slot, object);
}

extern "C" void* __fastcall
FableSystemFontConstruct_009a6610(
    void* object,
    void*,
    const void*,
    const void*)
{
    return object;
}

extern "C" void __fastcall
FableSystemFontReset_009a6610(
    void* slot,
    void*,
    void* object)
{
    StoreObject(slot, object);
}

extern "C" void* __fastcall
FableSystemTextConstruct_009a6610(void* object, void*)
{
    if (g_ActiveState != 0)
        ++g_ActiveState->exactSystemTextConstructCalls;
    return object;
}

extern "C" void __fastcall
FableSystemTextReset_009a6610(
    void* slot,
    void*,
    void* object)
{
    StoreObject(slot, object);
}

extern "C" void* __fastcall
FableSystemMeshConstruct_009a6610(void* object, void*)
{
    if (g_ActiveState != 0)
        ++g_ActiveState->exactSystemMeshConstructCalls;
    return object;
}

extern "C" void __fastcall
FableSystemMeshReset_009a6610(
    void* slot,
    void*,
    void* object)
{
    StoreObject(slot, object);
}

extern "C" void __fastcall
FableSystemAddComponent_009a6610(void*, void*, void*)
{
    if (g_ActiveState != 0)
        ++g_ActiveState->exactSystemComponentAddCalls;
}

extern "C" void* __fastcall
FableSystemColoursConstruct_009a6610(void* object, void*)
{
    if (g_ActiveState != 0)
        ++g_ActiveState->exactSystemColoursConstructCalls;
    return object;
}

extern "C" void __fastcall
FableSystemColoursReset_009a6610(
    void* slot,
    void*,
    void* object)
{
    StoreObject(slot, object);
}

extern "C" void FableSystemInitialiseCosine_009a6610()
{
}

extern "C" void FableSystemInitialiseVectorMath_009a6610()
{
}

extern "C" void __fastcall
FableSystemInitColours_009a6610(void* manager, void*)
{
    static_cast<fable_u8*>(manager)[0xDC] = 1;
}

extern "C" void __fastcall
FableSystemInitApplicationName_009a6610(void*, void*)
{
}

fable_i32 FABLE_FASTCALL
FableRunExactSystemManagerInitialiseBoundary(
    CSystemManagerInit* init,
    GFMainPhase9State& state)
{
    memset(g_SystemManagerStorage, 0, sizeof(g_SystemManagerStorage));
    memset(g_Allocations, 0, sizeof(g_Allocations));
    g_AllocationCount = 0;
    g_FableSystemInitialiseCount_013CA608 = 0;
    g_ActiveState = &state;

    CSystemManager* manager =
        reinterpret_cast<CSystemManager*>(g_SystemManagerStorage);
    const ESystemCode result = manager->Initialise(init);

    g_ActiveState = 0;
    state.exactSystemAllocationCalls = g_AllocationCount;
    state.observedExactSystemInitialiseCount =
        g_FableSystemInitialiseCount_013CA608;
    return result;
}
