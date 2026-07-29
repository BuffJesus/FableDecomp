// Real-object ABI/behavior fixture for
// NUISystem::CFrontEndManager::Init @ 0x005958F5.

#include <cstdio>
#include <cstring>

extern "C" void __fastcall
CFrontEndManager_Init_005958f5(
    void* manager,
    void*,
    unsigned long graphicsPointer,
    unsigned long graphicsCounter,
    unsigned long meshPointer,
    unsigned long meshCounter,
    void* inputManager,
    void* definitionManager);

enum Event
{
    kResetTail = 1,
    kCopyMesh,
    kGetManager,
    kSetGraphics,
    kAddObserver,
    kGetService,
    kDefaults,
    kGetNames,
    kRegister,
    kDestroyNames,
    kDestroyGraphics,
    kDestroyMesh
};

struct VectorProbe
{
    unsigned long* first;
    unsigned long* last;
    unsigned long* end;
};

static Event g_events[24];
static int g_eventCount;
static unsigned char g_baseManager[0x30];
static unsigned char g_profileService;
static unsigned long g_profiles[2];
static int g_profileCount;
static void* g_expectedManager;
static void* g_resetTail;
static unsigned long g_copiedMesh[2];
static unsigned long g_forwardedMesh[2];
static int g_getManagerCalls;
static void* g_observer;
static int g_getServiceCalls;
static int g_defaultsCalls;
static void* g_namesAddress;
static void* g_registeredProfiles[2];
static int g_registerCalls;
static void* g_destroyedNames;
static unsigned long g_destroyedGraphics[2];
static unsigned long g_destroyedMesh[2];

static void AddEvent(Event event)
{
    g_events[g_eventCount++] = event;
}

extern "C" void __fastcall
FableFrontEndInitResetTail(void* tail, void*)
{
    AddEvent(kResetTail);
    g_resetTail = tail;
}

extern "C" void __fastcall
FableFrontEndInitCopyCountedPointer(
    void* destination,
    void*,
    const void* source)
{
    AddEvent(kCopyMesh);
    const unsigned long* values = (const unsigned long*)source;
    g_copiedMesh[0] = values[0];
    g_copiedMesh[1] = values[1];
    ((unsigned long*)destination)[0] = values[0];
    ((unsigned long*)destination)[1] = values[1];
}

extern "C" void* __cdecl
FableFrontEndInitGetBaseManager()
{
    AddEvent(kGetManager);
    ++g_getManagerCalls;
    return g_baseManager;
}

extern "C" void __fastcall
FableFrontEndInitSetGraphicsBank(
    void* manager,
    void*,
    unsigned long pointer,
    unsigned long counter)
{
    AddEvent(kSetGraphics);
    if (manager == g_baseManager)
    {
        g_forwardedMesh[0] = pointer;
        g_forwardedMesh[1] = counter;
    }
}

extern "C" void __fastcall
FableFrontEndInitAddObserver(
    void* manager,
    void*,
    void* observer)
{
    AddEvent(kAddObserver);
    if (manager == g_baseManager)
        g_observer = observer;
}

extern "C" void* __cdecl
FableFrontEndInitGetProfileService()
{
    AddEvent(kGetService);
    ++g_getServiceCalls;
    return &g_profileService;
}

extern "C" void __fastcall
FableFrontEndInitSetProfileDefaults(void* service, void*)
{
    AddEvent(kDefaults);
    if (service == &g_profileService)
        ++g_defaultsCalls;
}

extern "C" void __fastcall
FableFrontEndInitGetProfileNames(
    void* manager,
    void*,
    void* names)
{
    AddEvent(kGetNames);
    g_namesAddress = names;
    VectorProbe* vector = (VectorProbe*)names;
    if (manager == g_expectedManager)
    {
        vector->first = g_profiles;
        vector->last = g_profiles + g_profileCount;
        vector->end = vector->last;
    }
}

extern "C" void __fastcall
FableFrontEndInitRegisterProfile(
    void* service,
    void*,
    void* profile)
{
    AddEvent(kRegister);
    if (service == &g_profileService)
        g_registeredProfiles[g_registerCalls++] = profile;
}

extern "C" void __fastcall
FableFrontEndInitDestroyProfileNames(void* names, void*)
{
    AddEvent(kDestroyNames);
    g_destroyedNames = names;
}

extern "C" void __fastcall
FableFrontEndInitDestroyGraphicsPointer(void* pointer, void*)
{
    AddEvent(kDestroyGraphics);
    g_destroyedGraphics[0] = ((unsigned long*)pointer)[0];
    g_destroyedGraphics[1] = ((unsigned long*)pointer)[1];
}

extern "C" void __fastcall
FableFrontEndInitDestroyMeshPointer(void* pointer, void*)
{
    AddEvent(kDestroyMesh);
    g_destroyedMesh[0] = ((unsigned long*)pointer)[0];
    g_destroyedMesh[1] = ((unsigned long*)pointer)[1];
}

static void Reset(unsigned char* manager, int profileCount)
{
    std::memset(manager, 0xcc, 0xe0);
    std::memset(g_baseManager, 0, sizeof(g_baseManager));
    g_eventCount = 0;
    g_profileCount = profileCount;
    g_expectedManager = manager;
    g_resetTail = 0;
    g_copiedMesh[0] = g_copiedMesh[1] = 0;
    g_forwardedMesh[0] = g_forwardedMesh[1] = 0;
    g_getManagerCalls = 0;
    g_observer = 0;
    g_getServiceCalls = 0;
    g_defaultsCalls = 0;
    g_namesAddress = 0;
    g_registeredProfiles[0] = g_registeredProfiles[1] = 0;
    g_registerCalls = 0;
    g_destroyedNames = 0;
    g_destroyedGraphics[0] = g_destroyedGraphics[1] = 0;
    g_destroyedMesh[0] = g_destroyedMesh[1] = 0;
}

static bool Run(int profileCount)
{
    unsigned char manager[0xe0];
    unsigned char inputManager;
    unsigned char definitionManager;
    const unsigned long graphicsPointer = 0x11112222;
    const unsigned long graphicsCounter = 0x33334444;
    const unsigned long meshPointer = 0x55556666;
    const unsigned long meshCounter = 0x77778888;
    Reset(manager, profileCount);

    CFrontEndManager_Init_005958f5(
        manager,
        0,
        graphicsPointer,
        graphicsCounter,
        meshPointer,
        meshCounter,
        &inputManager,
        &definitionManager);

    static const Event emptyEvents[] =
    {
        kResetTail, kCopyMesh, kGetManager, kSetGraphics,
        kGetManager, kGetManager, kAddObserver,
        kGetService, kDefaults, kGetNames,
        kDestroyNames, kDestroyGraphics, kDestroyMesh
    };
    static const Event populatedEvents[] =
    {
        kResetTail, kCopyMesh, kGetManager, kSetGraphics,
        kGetManager, kGetManager, kAddObserver,
        kGetService, kDefaults, kGetNames,
        kGetService, kRegister, kGetService, kRegister,
        kDestroyNames, kDestroyGraphics, kDestroyMesh
    };
    const Event* expected = profileCount ? populatedEvents : emptyEvents;
    const unsigned long eventCount = profileCount
        ? sizeof(populatedEvents) / sizeof(populatedEvents[0])
        : sizeof(emptyEvents) / sizeof(emptyEvents[0]);
    return g_eventCount == (int)eventCount &&
        std::memcmp(g_events, expected, eventCount * sizeof(Event)) == 0 &&
        g_resetTail == manager + 0xc4 &&
        g_copiedMesh[0] == meshPointer &&
        g_copiedMesh[1] == meshCounter &&
        g_forwardedMesh[0] == meshPointer &&
        g_forwardedMesh[1] == meshCounter &&
        g_getManagerCalls == 3 &&
        *(void**)(g_baseManager + 0x20) == &inputManager &&
        g_observer == &definitionManager &&
        *(unsigned long*)(manager + 0x98) == 0 &&
        *(unsigned long*)(manager + 0x9c) == 0 &&
        g_defaultsCalls == 1 &&
        g_destroyedNames == g_namesAddress &&
        g_destroyedGraphics[0] == graphicsPointer &&
        g_destroyedGraphics[1] == graphicsCounter &&
        g_destroyedMesh[0] == meshPointer &&
        g_destroyedMesh[1] == meshCounter &&
        g_registerCalls == profileCount &&
        (profileCount == 0 ||
            (g_registeredProfiles[0] == &g_profiles[0] &&
             g_registeredProfiles[1] == &g_profiles[1]));
}

int main()
{
    const bool empty = Run(0);
    const bool populated = Run(2);
    if (!empty || !populated)
    {
        std::printf(
            "FSE2_005958f5_TEST FAIL empty=%d populated=%d\n",
            empty ? 1 : 0,
            populated ? 1 : 0);
        return 1;
    }
    std::puts("FSE2_005958f5_TEST PASS");
    return 0;
}
