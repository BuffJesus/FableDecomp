#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/4a/CWorld_LoadHeroStateInternal_004a2d70.cpp"

struct TestWideString_004a2d70
{
    void* data;
};

struct TestWideBuffer_004a2d70
{
    const unsigned short* begin;
    const unsigned short* end;
};

extern "C" const unsigned short CWorld_HeroStateSuffix_004a2d70[] = {
    '.', 'h', 's', 0
};

static int g_ProfileManager;
static int g_DriveManager;
extern "C" void* CWorld_DriveManager_004a2d70 = &g_DriveManager;

static const unsigned short g_LoadedText[] = {
    'A', 'u', 't', 'o', '.', 'h', 's'
};
static const unsigned short g_OtherText[] = {
    'O', 't', 'h', 'e', 'r', '.', 'h', 's'
};
static TestWideBuffer_004a2d70 g_LoadedBuffer = {
    g_LoadedText,
    g_LoadedText + 7
};
static TestWideBuffer_004a2d70 g_AutoBuffer = {
    g_LoadedText,
    g_LoadedText + 7
};
static TestWideBuffer_004a2d70 g_OtherBuffer = {
    g_OtherText,
    g_OtherText + 8
};
static TestWideBuffer_004a2d70 g_SaveWithSuffixBuffer;
static TestWideBuffer_004a2d70 g_AutoBaseBuffer;

static volatile bool g_PathExists;
static volatile bool g_LoadResult;
static volatile bool g_AutoMatchesLoaded;
static volatile int g_AppendCalls;
static volatile int g_LoadCalls;
static volatile int g_DeleteCalls;
static volatile int g_CompareCalls;
static volatile int g_DestroyCalls;
static volatile int g_LatchSeen;
static volatile int g_ResolveSelector;
static volatile int g_LoadProgress;
static void* volatile g_ResolvedPathObject;
static void* volatile g_AutoPathObject;
static const void* volatile g_DeletedPathObject;
static void* volatile g_DeleteManager;
static const unsigned short* volatile g_AppendSuffixes[2];

extern "C" void* __fastcall
CWorld_AppendWideSuffix_004a2d70(
    void* destination,
    const void*,
    const unsigned short* suffix)
{
    const int append = g_AppendCalls++;
    g_AppendSuffixes[append] = suffix;
    ((TestWideString_004a2d70*)destination)->data =
        append == 0 ? (void*)&g_SaveWithSuffixBuffer :
        (g_AutoMatchesLoaded ? (void*)&g_AutoBuffer : (void*)&g_OtherBuffer);
    if (append == 1)
        g_AutoPathObject = destination;
    return destination;
}

extern "C" void* __cdecl CWorld_GetProfileManager_004a2d70()
{
    return &g_ProfileManager;
}

extern "C" void* __fastcall
CWorld_GetPathNameFromFileName_004a2d70(
    void* manager,
    void*,
    void* destination,
    const void*,
    int directorySelector)
{
    if (manager != &g_ProfileManager)
        g_ResolveSelector = -2;
    else
        g_ResolveSelector = directorySelector;
    ((TestWideString_004a2d70*)destination)->data = &g_LoadedBuffer;
    g_ResolvedPathObject = destination;
    return destination;
}

extern "C" bool __fastcall
CWorld_PathExists_004a2d70(const void* path, void*)
{
    if (path != g_ResolvedPathObject)
        return false;
    return g_PathExists;
}

extern "C" bool __fastcall
CWorld_LoadGameStateInternal_004a2d70(
    void* world,
    void*,
    const void* path,
    int showProgress)
{
    ++g_LoadCalls;
    g_LatchSeen = *((unsigned char*)world + 0x102);
    g_LoadProgress = showProgress;
    if (path != g_ResolvedPathObject)
        return false;
    return g_LoadResult;
}

extern "C" void* __fastcall
CWorld_GetAutoSaveCheckPointPathName_004a2d70(
    void* manager,
    void*,
    void* destination)
{
    if (manager != &g_ProfileManager)
        g_AutoMatchesLoaded = false;
    ((TestWideString_004a2d70*)destination)->data = &g_AutoBaseBuffer;
    return destination;
}

extern "C" void __fastcall
CWorld_DestroyWideString_004a2d70(void*, void*)
{
    ++g_DestroyCalls;
}

extern "C" int __cdecl
CWorld_CompareWideCharacters_004a2d70(
    const unsigned short* left,
    const unsigned short* right,
    unsigned long count)
{
    ++g_CompareCalls;
    for (unsigned long index = 0; index != count; ++index)
    {
        if (left[index] != right[index])
            return left[index] < right[index] ? -1 : 1;
    }
    return 0;
}

extern "C" int __fastcall
CWorld_DeleteHeroStatePath_004a2d70(
    void* manager,
    void*,
    const void* path)
{
    ++g_DeleteCalls;
    g_DeleteManager = manager;
    g_DeletedPathObject = path;
    return manager == &g_DriveManager ? 0 : -1;
}

static int RunCase(
    bool pathExists,
    bool loadResult,
    bool autoMatches,
    bool expectedResult,
    int expectedLoads,
    int expectedDeletes,
    int expectedDestroys)
{
    unsigned char world[0x103];
    std::memset(world, 0, sizeof(world));
    TestWideString_004a2d70 saveName;
    saveName.data = (void*)"SaveName";

    g_PathExists = pathExists;
    g_LoadResult = loadResult;
    g_AutoMatchesLoaded = autoMatches;
    g_AppendCalls = 0;
    g_LoadCalls = 0;
    g_DeleteCalls = 0;
    g_CompareCalls = 0;
    g_DestroyCalls = 0;
    g_LatchSeen = 0;
    g_ResolveSelector = -1;
    g_LoadProgress = -1;
    g_ResolvedPathObject = 0;
    g_AutoPathObject = 0;
    g_DeletedPathObject = 0;
    g_DeleteManager = 0;
    g_AppendSuffixes[0] = 0;
    g_AppendSuffixes[1] = 0;

    typedef bool (__fastcall *LoadFunction)(void*, void*, const void*);
    LoadFunction volatile loadHero =
        &CWorld_LoadHeroStateInternal_004a2d70;
    const bool result = loadHero(world, 0, &saveName);

    int failures = 0;
    if (result != expectedResult ||
        g_LoadCalls != expectedLoads ||
        g_DeleteCalls != expectedDeletes ||
        g_DestroyCalls != expectedDestroys ||
        g_ResolveSelector != 0 ||
        world[0x102] != 0)
    {
        ++failures;
    }

    if (g_AppendSuffixes[0] != CWorld_HeroStateSuffix_004a2d70)
        ++failures;

    if (expectedLoads != 0 &&
        (g_LatchSeen != 1 || g_LoadProgress != 0))
    {
        ++failures;
    }

    if (loadResult && pathExists)
    {
        if (g_AppendCalls != 2 ||
            g_AppendSuffixes[1] != CWorld_HeroStateSuffix_004a2d70)
        {
            ++failures;
        }
        if (expectedDeletes != 0 &&
            (g_DeletedPathObject != g_AutoPathObject ||
             g_DeleteManager != &g_DriveManager ||
             g_CompareCalls != 1))
        {
            ++failures;
        }
        if (expectedDeletes == 0 && g_CompareCalls != 0)
            ++failures;
    }

    return failures;
}

int main()
{
    int failures = 0;
    failures += RunCase(false, false, false, true, 0, 0, 2);
    failures += RunCase(true, false, false, false, 1, 0, 2);
    failures += RunCase(true, true, false, true, 1, 0, 4);
    failures += RunCase(true, true, true, true, 1, 1, 4);

    if (failures != 0)
    {
        std::printf(
            "LOAD_HERO_STATE_004a2d70_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("LOAD_HERO_STATE_004a2d70_TEST PASS\n");
    return 0;
}
