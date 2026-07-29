#include <cstdio>

#include "../../../src/compiled/00/49/CWorld_IsHeroStateFileValid_0049d4e0.cpp"

struct TestWideString_0049d4e0
{
    void* data;
};

extern "C" const unsigned short CWorld_HeroStateSuffix_0049d4e0[] = {
    '.', 'h', 's', 0
};

static int g_ProfileManager;
static int g_AppendedToken;
static int g_ResolvedToken;
static volatile int g_Sequence;
static volatile int g_AppendSequence;
static volatile int g_ResolveSequence;
static volatile int g_ExistsSequence;
static volatile int g_DestroySequences[2];
static volatile int g_DestroyCount;
static const void* volatile g_AppendInput;
static const unsigned short* volatile g_AppendSuffix;
static void* volatile g_AppendDestination;
static void* volatile g_ResolveManager;
static void* volatile g_ResolveDestination;
static const void* volatile g_ResolveInput;
static volatile int g_ResolveSelector;
static const void* volatile g_ExistsPath;
static void* volatile g_Destroyed[2];
static volatile bool g_ExistsResult;

extern "C" void* __fastcall
CWorld_AppendHeroStateSuffix_0049d4e0(
    void* destination,
    const void* saveName,
    const unsigned short* suffix)
{
    g_AppendSequence = ++g_Sequence;
    g_AppendInput = saveName;
    g_AppendSuffix = suffix;
    g_AppendDestination = destination;
    ((TestWideString_0049d4e0*)destination)->data = &g_AppendedToken;
    return destination;
}

extern "C" void* __cdecl CWorld_GetProfileManager_0049d4e0()
{
    return &g_ProfileManager;
}

extern "C" void* __fastcall
CWorld_GetPathNameFromFileName_0049d4e0(
    void* profileManager,
    void*,
    void* destination,
    const void* fileName,
    int directorySelector)
{
    g_ResolveSequence = ++g_Sequence;
    g_ResolveManager = profileManager;
    g_ResolveDestination = destination;
    g_ResolveInput = fileName;
    g_ResolveSelector = directorySelector;
    ((TestWideString_0049d4e0*)destination)->data = &g_ResolvedToken;
    return destination;
}

extern "C" void __fastcall
CWorld_DestroyWideString_0049d4e0(void* stringValue, void*)
{
    const int destroy = g_DestroyCount++;
    g_DestroySequences[destroy] = ++g_Sequence;
    g_Destroyed[destroy] = stringValue;
}

extern "C" bool __fastcall
CWorld_PathExists_0049d4e0(const void* path, void*)
{
    g_ExistsSequence = ++g_Sequence;
    g_ExistsPath = path;
    return g_ExistsResult;
}

static int RunCase(bool exists)
{
    TestWideString_0049d4e0 saveName;
    saveName.data = (void*)"HeroSave";

    g_Sequence = 0;
    g_AppendSequence = 0;
    g_ResolveSequence = 0;
    g_ExistsSequence = 0;
    g_DestroySequences[0] = 0;
    g_DestroySequences[1] = 0;
    g_DestroyCount = 0;
    g_AppendInput = 0;
    g_AppendSuffix = 0;
    g_AppendDestination = 0;
    g_ResolveManager = 0;
    g_ResolveDestination = 0;
    g_ResolveInput = 0;
    g_ResolveSelector = -1;
    g_ExistsPath = 0;
    g_Destroyed[0] = 0;
    g_Destroyed[1] = 0;
    g_ExistsResult = exists;

    typedef bool (__fastcall *ValidationFunction)(
        void*,
        void*,
        const void*);
    ValidationFunction volatile validate =
        &CWorld_IsHeroStateFileValid_0049d4e0;
    const bool result = validate((void*)0x1234, 0, &saveName);

    int failures = 0;
    if (result != exists ||
        g_AppendSequence != 1 ||
        g_ResolveSequence != 2 ||
        g_DestroySequences[0] != 3 ||
        g_ExistsSequence != 4 ||
        g_DestroySequences[1] != 5)
    {
        ++failures;
    }

    if (g_AppendInput != &saveName ||
        g_AppendSuffix != CWorld_HeroStateSuffix_0049d4e0 ||
        g_ResolveManager != &g_ProfileManager ||
        g_ResolveInput != g_AppendDestination ||
        g_ResolveSelector != 0 ||
        g_Destroyed[0] != g_AppendDestination ||
        g_ExistsPath != g_ResolveDestination ||
        g_Destroyed[1] != g_ResolveDestination ||
        g_Destroyed[0] == g_Destroyed[1])
    {
        ++failures;
    }

    return failures;
}

int main()
{
    const int failures = RunCase(false) + RunCase(true);
    if (failures != 0)
    {
        std::printf(
            "HERO_STATE_VALID_0049d4e0_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("HERO_STATE_VALID_0049d4e0_TEST PASS\n");
    return 0;
}
