// Real-object ABI/behavior fixture for
// NUISystem::CFrontEndList::ScrollUp @ 0x0054C4C0.

#include <cstdio>
#include <cstring>

struct Colour
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alpha;
};

struct Vector2
{
    float x;
    float y;
};

struct Component
{
    void** vtable;
    int id;
    Colour colour;
    Vector2 position;
    int states[4];
    int stateCount;
    Colour setColours[8];
    float colourTimes[8];
    unsigned int colourFlags[8];
    int colourCount;
    Vector2 setPositions[4];
    float positionTimes[4];
    unsigned int positionFlags[4];
    int positionCount;
};

struct FrontEndList
{
    void** vtable;
    char pad004[0x15c - 4];
    int selected;
    char pad160[0x164 - 0x160];
    Component** begin;
    Component** end;
    char pad16c[0x1f8 - 0x16c];
    unsigned char alphaFalloff;
};

struct DefObject
{
    void** vtable;
    int references;
    char payload[0x1b0];
};

struct Manager
{
    void** vtable;
};

extern "C" void __fastcall
CFrontEndList_ScrollUp_0054c4c0(void* self, void*);

extern "C" const float FableFrontEndScrollMinusOne = -1.0f;
extern "C" const float FableFrontEndScrollInv255 =
    0.003921568859368563f;
extern "C" const float FableFrontEndScrollOne = 1.0f;
extern "C" const float FableFrontEndScrollZero = 0.0f;
extern "C" const float FableFrontEndScroll255 = 255.0f;

static void* g_listVtable[0x2b0 / 4];
static void* g_componentVtable[0xe4 / 4];
static void* g_managerVtable[0xac / 4];
static void* g_defVtable[2];
static Manager g_manager;
static DefObject g_definition;
static bool g_stopAtEnds;
static float g_updateTime;
static int g_policyCalls;
static int g_updateTimeCalls;
static int g_getManagerCalls;
static int g_getDefinitionCalls;
static int g_playCalls;
static void* g_playedCriteria;
static int g_playArgument;
static int g_releaseCalls;

static unsigned int Pack(const Colour& colour)
{
    unsigned int packed;
    std::memcpy(&packed, &colour, sizeof(packed));
    return packed;
}

static unsigned char __fastcall
ListStopsAtEnds(void*, void*)
{
    ++g_policyCalls;
    return g_stopAtEnds ? 1 : 0;
}

static float __fastcall
ListGetUpdateTime(void*, void*)
{
    ++g_updateTimeCalls;
    return g_updateTime;
}

static void __fastcall
ComponentSetState(void* object, void*, int state)
{
    Component* component = (Component*)object;
    component->states[component->stateCount++] = state;
}

static Colour* __fastcall
ComponentGetColour(void* object, void*, Colour* result)
{
    Component* component = (Component*)object;
    *result = component->colour;
    return result;
}

static Vector2* __fastcall
ComponentGetPosition(void* object, void*, Vector2* result)
{
    Component* component = (Component*)object;
    *result = component->position;
    return result;
}

static void __fastcall
ComponentSetColour(
    void* object,
    void*,
    const Colour* colour,
    float updateTime,
    unsigned int flags)
{
    Component* component = (Component*)object;
    const int index = component->colourCount++;
    component->setColours[index] = *colour;
    component->colourTimes[index] = updateTime;
    component->colourFlags[index] = flags;
    component->colour = *colour;
}

static void __fastcall
ComponentSetPosition(
    void* object,
    void*,
    const Vector2* position,
    float updateTime,
    unsigned int flags)
{
    Component* component = (Component*)object;
    const int index = component->positionCount++;
    component->setPositions[index] = *position;
    component->positionTimes[index] = updateTime;
    component->positionFlags[index] = flags;
    component->position = *position;
}

static DefObject** __fastcall
ManagerGetDefinition(void*, void*, DefObject** result)
{
    ++g_getDefinitionCalls;
    *result = &g_definition;
    return result;
}

static void __fastcall
ManagerPlaySound(void*, void*, void* criteria, int argument)
{
    ++g_playCalls;
    g_playedCriteria = criteria;
    g_playArgument = argument;
}

static void __fastcall
DefinitionRelease(void*, void*)
{
    ++g_releaseCalls;
}

extern "C" void* __cdecl
FableFrontEndScrollGetManager()
{
    ++g_getManagerCalls;
    return &g_manager;
}

extern "C" __declspec(naked) int __cdecl
FableFrontEndScrollFtol2()
{
    __asm
    {
        sub esp, 8
        fnstcw word ptr [esp]
        mov ax, word ptr [esp]
        or ax, 0c00h
        mov word ptr [esp + 2], ax
        fldcw word ptr [esp + 2]
        fistp dword ptr [esp + 4]
        fldcw word ptr [esp]
        mov eax, dword ptr [esp + 4]
        add esp, 8
        ret
    }
}

static void ResetEnvironment()
{
    std::memset(g_listVtable, 0, sizeof(g_listVtable));
    std::memset(g_componentVtable, 0, sizeof(g_componentVtable));
    std::memset(g_managerVtable, 0, sizeof(g_managerVtable));
    std::memset(g_defVtable, 0, sizeof(g_defVtable));
    std::memset(&g_definition, 0, sizeof(g_definition));

    g_listVtable[0xd8 / 4] = (void*)&ListGetUpdateTime;
    g_listVtable[0x2ac / 4] = (void*)&ListStopsAtEnds;
    g_componentVtable[0x80 / 4] = (void*)&ComponentSetPosition;
    g_componentVtable[0x98 / 4] = (void*)&ComponentSetColour;
    g_componentVtable[0xc0 / 4] = (void*)&ComponentSetState;
    g_componentVtable[0xdc / 4] = (void*)&ComponentGetPosition;
    g_componentVtable[0xe0 / 4] = (void*)&ComponentGetColour;
    g_managerVtable[0xa0 / 4] = (void*)&ManagerPlaySound;
    g_managerVtable[0xa8 / 4] = (void*)&ManagerGetDefinition;
    g_defVtable[1] = (void*)&DefinitionRelease;
    g_manager.vtable = g_managerVtable;
    g_definition.vtable = g_defVtable;
    g_definition.references = 1;

    g_stopAtEnds = false;
    g_updateTime = -1.0f;
    g_policyCalls = 0;
    g_updateTimeCalls = 0;
    g_getManagerCalls = 0;
    g_getDefinitionCalls = 0;
    g_playCalls = 0;
    g_playedCriteria = 0;
    g_playArgument = 0;
    g_releaseCalls = 0;
}

static void InitialiseList(
    FrontEndList& list,
    Component* components,
    Component** componentPointers,
    int count,
    int selected,
    unsigned char alphaFalloff)
{
    std::memset(&list, 0, sizeof(list));
    list.vtable = g_listVtable;
    list.selected = selected;
    list.begin = componentPointers;
    list.end = componentPointers + count;
    list.alphaFalloff = alphaFalloff;

    for (int i = 0; i != count; ++i)
    {
        std::memset(&components[i], 0, sizeof(components[i]));
        components[i].vtable = g_componentVtable;
        components[i].id = i;
        components[i].colour.red = (unsigned char)(10 + i);
        components[i].colour.green = (unsigned char)(20 + i);
        components[i].colour.blue = (unsigned char)(30 + i);
        components[i].colour.alpha = (unsigned char)(40 + i);
        components[i].position.x = (float)(100 + i);
        components[i].position.y = (float)(200 + i);
        componentPointers[i] = &components[i];
    }
}

static int CheckReject(
    int count,
    int selected,
    bool stopAtEnds,
    int failureBase)
{
    ResetEnvironment();
    g_stopAtEnds = stopAtEnds;
    Component components[2];
    Component* componentPointers[2];
    FrontEndList list;
    InitialiseList(
        list,
        components,
        componentPointers,
        count,
        selected,
        64);

    CFrontEndList_ScrollUp_0054c4c0(&list, 0);

    if (list.selected != selected)
        return failureBase;
    if (g_getManagerCalls != 2 ||
        g_getDefinitionCalls != 1 ||
        g_playCalls != 1)
        return failureBase + 1;
    if (g_playedCriteria !=
            (void*)((char*)&g_definition + 0x1a4) ||
        g_playArgument != 1)
        return failureBase + 2;
    if (g_definition.references != 0 || g_releaseCalls != 1)
        return failureBase + 3;
    for (int i = 0; i != count; ++i)
    {
        if (components[i].stateCount != 0 ||
            components[i].colourCount != 0 ||
            components[i].positionCount != 0)
            return failureBase + 4;
    }
    return 0;
}

static int CheckSentinelWrapWithoutStopping()
{
    ResetEnvironment();
    g_stopAtEnds = false;
    g_updateTime = -1.0f;
    Component components[4];
    Component* componentPointers[4];
    FrontEndList list;
    InitialiseList(list, components, componentPointers, 4, 0, 64);
    Colour original[4];
    for (int i = 0; i != 4; ++i)
        original[i] = components[i].colour;

    CFrontEndList_ScrollUp_0054c4c0(&list, 0);

    if (list.selected != 3 ||
        components[0].stateCount != 1 ||
        components[0].states[0] != 4 ||
        components[3].stateCount != 1 ||
        components[3].states[0] != 3)
        return 30;
    if (g_updateTimeCalls != 1 || g_policyCalls != 6)
        return 31;
    if (g_playedCriteria !=
            (void*)((char*)&g_definition + 0x194) ||
        g_definition.references != 0 ||
        g_releaseCalls != 1)
        return 32;
    for (int i = 0; i != 4; ++i)
    {
        const Colour& expected = original[(i + 3) % 4];
        if (components[i].colourCount != 1 ||
            Pack(components[i].setColours[0]) != Pack(expected) ||
            components[i].colourTimes[0] != 0.0f ||
            components[i].colourFlags[0] != 0 ||
            components[i].positionCount != 0)
            return 33 + i;
    }
    return 0;
}

static int CheckTimedStoppingScrollAndAlpha()
{
    ResetEnvironment();
    g_stopAtEnds = true;
    g_updateTime = 0.5f;
    Component components[4];
    Component* componentPointers[4];
    FrontEndList list;
    InitialiseList(list, components, componentPointers, 4, 2, 64);

    CFrontEndList_ScrollUp_0054c4c0(&list, 0);

    if (list.selected != 1 ||
        components[2].stateCount != 1 ||
        components[2].states[0] != 4 ||
        components[1].stateCount != 1 ||
        components[1].states[0] != 3)
        return 40;
    if (g_updateTimeCalls != 2 || g_policyCalls != 5)
        return 41;

    const unsigned char expectedAlpha[4] = { 190, 255, 190, 126 };
    for (int i = 0; i != 4; ++i)
    {
        if (components[i].colourCount != 2)
            return 42 + i;
        const Colour& alphaColour = components[i].setColours[1];
        if (alphaColour.red != 255 ||
            alphaColour.green != 255 ||
            alphaColour.blue != 255 ||
            alphaColour.alpha != expectedAlpha[i] ||
            components[i].colourTimes[1] != 0.5f ||
            components[i].colourFlags[1] != 0)
            return 46 + i;
        if (components[i].positionCount != 1 ||
            components[i].setPositions[0].x != 0.0f ||
            components[i].setPositions[0].y != 30.0f ||
            components[i].positionTimes[0] != 0.5f ||
            components[i].positionFlags[0] != 0)
            return 50 + i;
    }
    return 0;
}

int main()
{
    if (sizeof(FrontEndList) != 0x1fc)
        return 90;

    int failure = CheckReject(1, 0, false, 1);
    if (failure != 0)
        return failure;

    failure = CheckReject(2, 0, true, 10);
    if (failure != 0)
        return failure;

    failure = CheckSentinelWrapWithoutStopping();
    if (failure != 0)
        return failure;

    failure = CheckTimedStoppingScrollAndAlpha();
    if (failure != 0)
        return failure;

    std::printf("FSE2_0054c4c0_TEST PASS\n");
    return 0;
}
