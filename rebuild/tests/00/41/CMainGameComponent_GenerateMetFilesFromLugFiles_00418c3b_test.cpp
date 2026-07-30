#include <stdio.h>
#include <string.h>

#include "fable_game.h"

extern "C" fable_u32 g_FableSecurityCookie_0139C8A8 = 0x5A17C0DE;
extern "C" fable_u8 g_FableMetPersistVTable_0122ED7C = 0;

namespace
{
    struct FakeMetadataManager
    {
        void** vtable;
    };

    struct FakeSystemManager
    {
        fable_u8 padding00[0x7C];
        FakeMetadataManager* metadata;
    };

    void* g_metadataVTable[52];
    FakeMetadataManager g_metadataManager;
    FakeSystemManager g_systemManager;
    bool g_hasFile;
    fable_u32 g_autoNameCalls;
    fable_u32 g_assignValueCalls;
    fable_u32 g_assignDataCalls;
    fable_u32 g_findFirstCalls;
    fable_u32 g_findNextCalls;
    fable_u32 g_splitCalls;
    fable_u32 g_fileConstructs;
    fable_u32 g_metadataCalls;
    fable_u32 g_dumpCalls;
    fable_u32 g_fileDestructs;
    fable_u32 g_wideDestructs;
    fable_u32 g_securityChecks;
    bool g_cookieMatched;

    const wchar_t kAutoName[] = L"AUTO\\";
    const wchar_t kStem[] = L"Hero";
    const wchar_t kExtension[] = L".lug";
    const wchar_t kJoined[] = L"AUTO\\Hero.met";

    void Reset(bool hasFile)
    {
        g_hasFile = hasFile;
        g_autoNameCalls = 0;
        g_assignValueCalls = 0;
        g_assignDataCalls = 0;
        g_findFirstCalls = 0;
        g_findNextCalls = 0;
        g_splitCalls = 0;
        g_fileConstructs = 0;
        g_metadataCalls = 0;
        g_dumpCalls = 0;
        g_fileDestructs = 0;
        g_wideDestructs = 0;
        g_securityChecks = 0;
        g_cookieMatched = false;
    }

    void Store(void* destination, const void* value)
    {
        *reinterpret_cast<const void**>(destination) = value;
    }

    void __fastcall GenerateMetadata(
        void*,
        void*,
        const void*,
        const void*,
        const void*)
    {
        ++g_metadataCalls;
    }

    bool CheckNoFiles()
    {
        Reset(false);
        CMainGameComponent::GenerateMetFilesFromLugFiles();
        return
            g_autoNameCalls == 1 &&
            g_assignValueCalls == 1 &&
            g_findFirstCalls == 1 &&
            g_splitCalls == 0 &&
            g_fileConstructs == 0 &&
            g_metadataCalls == 0 &&
            g_wideDestructs == 2 &&
            g_securityChecks == 1 &&
            g_cookieMatched;
    }

    bool CheckOneFile()
    {
        Reset(true);
        CMainGameComponent::GenerateMetFilesFromLugFiles();
        return
            g_autoNameCalls == 3 &&
            g_assignValueCalls == 2 &&
            g_assignDataCalls == 3 &&
            g_findFirstCalls == 1 &&
            g_findNextCalls == 1 &&
            g_splitCalls == 1 &&
            g_fileConstructs == 1 &&
            g_metadataCalls == 1 &&
            g_dumpCalls == 1 &&
            g_fileDestructs == 1 &&
            g_wideDestructs == 11 &&
            g_securityChecks == 1 &&
            g_cookieMatched;
    }
}

extern "C" void* __fastcall
FableMetGetAutoSaveName_00418c3b(void* destination, void*)
{
    ++g_autoNameCalls;
    Store(destination, kAutoName);
    return destination;
}

extern "C" void* __fastcall
FableMetAssignValue_00418c3b(
    void* destination,
    const void*,
    const void*)
{
    ++g_assignValueCalls;
    Store(destination, kJoined);
    return destination;
}

extern "C" void __fastcall FableMetWideDtor_00418c3b(void*, void*)
{
    ++g_wideDestructs;
}

extern "C" bool __fastcall
FableMetFindFirst_00418c3b(void*, void*, void* output)
{
    ++g_findFirstCalls;
    if (g_hasFile)
    {
        wcscpy(reinterpret_cast<wchar_t*>(output), L"Hero.lug");
    }
    return g_hasFile;
}

extern "C" void __fastcall FableMetWideCtor_00418c3b(
    void* destination,
    void*)
{
    Store(destination, 0);
}

extern "C" void __fastcall FableMetWideLiteralCtor_00418c3b(
    void* destination,
    void*,
    const wchar_t* value)
{
    Store(destination, value);
}

extern "C" void __fastcall FableMetSplitPath_00418c3b(
    void*,
    void*,
    void*,
    void* stem,
    void* extension)
{
    ++g_splitCalls;
    Store(stem, kStem);
    Store(extension, kExtension);
}

extern "C" void* __fastcall FableMetAssignData_00418c3b(
    void* destination,
    const void*,
    const void*)
{
    ++g_assignDataCalls;
    Store(destination, kJoined);
    return destination;
}

extern "C" void __fastcall FableMetMemoryFileCtor_00418c3b(
    void* destination,
    void*,
    const void*,
    fable_u32 access,
    fable_u32 sharing)
{
    ++g_fileConstructs;
    memset(destination, 0, 44);
    if (access != 3 || sharing != 2)
    {
        g_fileConstructs = 0xFFFFFFFF;
    }
}

extern "C" void* FableMetGetSystemManager_00418c3b()
{
    return &g_systemManager;
}

extern "C" void __fastcall FableMetDumpToFile_00418c3b(void*, void*)
{
    ++g_dumpCalls;
}

extern "C" bool __fastcall
FableMetFindNext_00418c3b(void*, void*, void*)
{
    ++g_findNextCalls;
    return false;
}

extern "C" void __fastcall FableMetMemoryFileDtor_00418c3b(void*, void*)
{
    ++g_fileDestructs;
}

extern "C" void __fastcall
FableMetSecurityCheck_00418c3b(fable_u32 cookie, void*)
{
    ++g_securityChecks;
    g_cookieMatched = cookie == g_FableSecurityCookie_0139C8A8;
}

int main()
{
    memset(g_metadataVTable, 0, sizeof(g_metadataVTable));
    g_metadataVTable[0xCC / 4] =
        reinterpret_cast<void*>(&GenerateMetadata);
    g_metadataManager.vtable = g_metadataVTable;
    g_systemManager.metadata = &g_metadataManager;

    if (!CheckNoFiles() || !CheckOneFile())
    {
        printf(
            "auto=%lu value=%lu data=%lu first=%lu next=%lu split=%lu "
            "ctor=%lu metadata=%lu dump=%lu file_dtor=%lu wide_dtor=%lu "
            "security=%lu cookie=%u\n",
            g_autoNameCalls,
            g_assignValueCalls,
            g_assignDataCalls,
            g_findFirstCalls,
            g_findNextCalls,
            g_splitCalls,
            g_fileConstructs,
            g_metadataCalls,
            g_dumpCalls,
            g_fileDestructs,
            g_wideDestructs,
            g_securityChecks,
            g_cookieMatched ? 1 : 0);
        puts("FABLETLC_GENERATE_MET_FILES_BEHAVIOR FAIL");
        return 1;
    }

    puts("FABLETLC_GENERATE_MET_FILES_BEHAVIOR PASS");
    return 0;
}
