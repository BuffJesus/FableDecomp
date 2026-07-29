#include <cstdio>

#include "../../../src/compiled/00/66/CFrontEndManager_CreateComponent_00661d20.cpp"

static int g_Sequence;
static int g_GetEngineSequence;
static int g_CreateSequence;
static CFrontEndManager_00661d20* g_ManagerSeen;
static FrontEndDisplayEngine_00661d20* g_EngineSeen;
static const FrontEndCharString_00661d20* g_NameSeen;
static void* g_ContextSeen;
static FrontEndDisplayEngine_00661d20 g_Engine;
static FrontEndComponent_00661d20 g_Component;
static FrontEndComponent_00661d20* g_CreateResult;

extern "C" FrontEndDisplayEngine_00661d20* __fastcall
FrontEndGetDisplayEngine_00661d20(
    CFrontEndManager_00661d20* manager,
    void*)
{
    g_GetEngineSequence = ++g_Sequence;
    g_ManagerSeen = manager;
    return &g_Engine;
}

extern "C" FrontEndComponent_00661d20* __fastcall
FrontEndDisplayEngineCreateComponent_00661d20(
    FrontEndDisplayEngine_00661d20* engine,
    void*,
    const FrontEndCharString_00661d20* name,
    void* optional_context)
{
    g_CreateSequence = ++g_Sequence;
    g_EngineSeen = engine;
    g_NameSeen = name;
    g_ContextSeen = optional_context;
    return g_CreateResult;
}

static void ResetProbe(FrontEndComponent_00661d20* result)
{
    g_Sequence = 0;
    g_GetEngineSequence = 0;
    g_CreateSequence = 0;
    g_ManagerSeen = 0;
    g_EngineSeen = 0;
    g_NameSeen = 0;
    g_ContextSeen = (void*)1;
    g_CreateResult = result;
}

int main()
{
    int failures = 0;
    CFrontEndManager_00661d20 manager;
    FrontEndCharString_00661d20 name;
    name.data = (void*)"main_menu";

    ResetProbe(&g_Component);
    FrontEndComponent_00661d20* result = manager.CreateComponent(name);
    if (result != &g_Component ||
        g_GetEngineSequence != 1 ||
        g_CreateSequence != 2 ||
        g_ManagerSeen != &manager ||
        g_EngineSeen != &g_Engine ||
        g_NameSeen != &name ||
        g_ContextSeen != 0)
    {
        std::printf("engine/create ABI and ordering failed\n");
        ++failures;
    }

    ResetProbe(0);
    result = manager.CreateComponent(name);
    if (result != 0 ||
        g_GetEngineSequence != 1 ||
        g_CreateSequence != 2 ||
        g_ContextSeen != 0)
    {
        std::printf("null component result forwarding failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_00661d20_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_00661d20_TEST PASS\n");
    return 0;
}
