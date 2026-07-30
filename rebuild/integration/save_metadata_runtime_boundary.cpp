#include "fable_game.h"
#include "fable_gfmain.h"

#include <string.h>

extern "C" fable_u32 g_FableSecurityCookie_0139C8A8 = 0x5A17C0DE;
extern "C" fable_u8 g_FableMetPersistVTable_0122ED7C = 0;

namespace
{
    GFMainPhase9State* g_ActiveSaveMetadataState;

    void StoreNull(void* destination)
    {
        if (destination != 0)
            *static_cast<void**>(destination) = 0;
    }
}

extern "C" void* __fastcall
FableMetGetAutoSaveName_00418c3b(void* destination, void*)
{
    StoreNull(destination);
    return destination;
}

extern "C" void* __fastcall
FableMetAssignValue_00418c3b(void* destination, const void*, const void*)
{
    StoreNull(destination);
    return destination;
}

extern "C" void __fastcall
FableMetWideDtor_00418c3b(void*, void*)
{
}

extern "C" bool __fastcall
FableMetFindFirst_00418c3b(void*, void*, void*)
{
    if (g_ActiveSaveMetadataState != 0)
        ++g_ActiveSaveMetadataState->exactSaveMetadataFindFirstCalls;
    return false;
}

extern "C" void __fastcall
FableMetWideCtor_00418c3b(void* destination, void*)
{
    StoreNull(destination);
}

extern "C" void __fastcall
FableMetWideLiteralCtor_00418c3b(void* destination, void*, const wchar_t*)
{
    StoreNull(destination);
}

extern "C" void __fastcall
FableMetSplitPath_00418c3b(void*, void*, void*, void*, void*)
{
}

extern "C" void* __fastcall
FableMetAssignData_00418c3b(void* destination, const void*, const void*)
{
    StoreNull(destination);
    return destination;
}

extern "C" void __fastcall
FableMetMemoryFileCtor_00418c3b(
    void* destination,
    void*,
    const void*,
    fable_u32,
    fable_u32)
{
    if (destination != 0)
        memset(destination, 0, 0x2C);
}

extern "C" void* FableMetGetSystemManager_00418c3b()
{
    return 0;
}

extern "C" void __fastcall
FableMetDumpToFile_00418c3b(void*, void*)
{
}

extern "C" bool __fastcall
FableMetFindNext_00418c3b(void*, void*, void*)
{
    return false;
}

extern "C" void __fastcall
FableMetMemoryFileDtor_00418c3b(void*, void*)
{
}

extern "C" void __fastcall
FableMetSecurityCheck_00418c3b(fable_u32 cookie, void*)
{
    if (
        g_ActiveSaveMetadataState != 0 &&
        cookie == g_FableSecurityCookie_0139C8A8)
    {
        ++g_ActiveSaveMetadataState->exactSaveMetadataSecurityChecks;
    }
}

void FABLE_FASTCALL FableRunExactSaveMetadataBoundary(
    GFMainPhase9State& state)
{
    ++state.exactSaveMetadataCoordinatorCalls;
    g_ActiveSaveMetadataState = &state;
    CMainGameComponent::GenerateMetFilesFromLugFiles();
    g_ActiveSaveMetadataState = 0;
}
