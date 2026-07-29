// Real-object ABI/behavior fixture for
// NUISystem::CFrontEndManager::CFrontEndManager @ 0x005953E2.

#include <cstdio>
#include <cstring>

extern "C" void* __fastcall
CFrontEndManager_CFrontEndManager_005953e2(void*, void*);

extern "C" void* const FableFrontEndManagerVtable[] =
{
    (void*)0x11111111,
    (void*)0x22222222
};

enum Event
{
    kBase = 1,
    kString14,
    kList18,
    kStack20,
    kUsedKeys54,
    kStringAC,
    kStringB0,
    kTailC4
};

static Event g_events[10];
static int g_eventCount;
static unsigned char* g_manager;
static int g_stringCalls;
static void* g_listAllocator;

static void AddEvent(Event event)
{
    g_events[g_eventCount++] = event;
}

static unsigned long& Field(unsigned long offset)
{
    return *(unsigned long*)(g_manager + offset);
}

extern "C" void __fastcall
FableFrontEndManagerConstructBase(void* manager, void*)
{
    AddEvent(kBase);
    g_manager = (unsigned char*)manager;
    std::memset(g_manager, 0xbc, 0x14);
}

extern "C" void __fastcall
FableFrontEndManagerConstructString(void* string, void*)
{
    const unsigned long offset =
        (unsigned long)((unsigned char*)string - g_manager);
    if (offset == 0x14)
        AddEvent(kString14);
    else if (offset == 0xac)
        AddEvent(kStringAC);
    else if (offset == 0xb0)
        AddEvent(kStringB0);
    ++g_stringCalls;
    *(unsigned long*)string = 0x51000000 | offset;
}

extern "C" void __fastcall
FableFrontEndManagerConstructList(
    void* list,
    void*,
    void* allocator)
{
    AddEvent(kList18);
    g_listAllocator = allocator;
    *(unsigned long*)list = 0x18181818;
}

extern "C" void __fastcall
FableFrontEndManagerConstructStack(void* stack, void*)
{
    AddEvent(kStack20);
    *(unsigned long*)stack = 0x20202020;
}

extern "C" void __fastcall
FableFrontEndManagerConstructUsedKeys(void* usedKeys, void*)
{
    AddEvent(kUsedKeys54);
    unsigned long* fields = (unsigned long*)usedKeys;
    fields[0] = 0x54545454;
    fields[1] = 0x58585858;
    fields[2] = 0x5c5c5c5c;
}

extern "C" void __fastcall
FableFrontEndManagerConstructTail(void* tail, void*)
{
    AddEvent(kTailC4);
    *(unsigned long*)tail = 0xc4c4c4c4;
}

static bool IsZero32(unsigned long offset)
{
    return Field(offset) == 0;
}

int main()
{
    unsigned char manager[0xe0];
    std::memset(manager, 0xcd, sizeof(manager));
    g_manager = manager;
    g_eventCount = 0;
    g_stringCalls = 0;
    g_listAllocator = 0;

    void* result =
        CFrontEndManager_CFrontEndManager_005953e2(manager, 0);

    static const Event expected[] =
    {
        kBase, kString14, kList18, kStack20, kUsedKeys54,
        kStringAC, kStringB0, kTailC4
    };
    static const unsigned long zeroFields[] =
    {
        0x48, 0x4c, 0x50, 0x60,
        0x68, 0x6c, 0x70, 0x74, 0x78, 0x7c,
        0x80, 0x84, 0x88, 0x8c, 0x90, 0x94,
        0x98, 0x9c, 0xa4, 0xa8
    };
    bool zeros = true;
    for (unsigned long i = 0;
         i < sizeof(zeroFields) / sizeof(zeroFields[0]);
         ++i)
        zeros = zeros && IsZero32(zeroFields[i]);

    const bool pass =
        result == manager &&
        g_eventCount == sizeof(expected) / sizeof(expected[0]) &&
        std::memcmp(g_events, expected, sizeof(expected)) == 0 &&
        g_stringCalls == 3 &&
        g_listAllocator != 0 &&
        *(void**)manager == FableFrontEndManagerVtable &&
        Field(0x14) == (0x51000000 | 0x14) &&
        Field(0x18) == 0x18181818 &&
        Field(0x20) == 0x20202020 &&
        Field(0x54) == 0x54545454 &&
        Field(0x58) == 0x58585858 &&
        Field(0x5c) == 0x5c5c5c5c &&
        Field(0xac) == (0x51000000 | 0xac) &&
        Field(0xb0) == (0x51000000 | 0xb0) &&
        Field(0xc4) == 0xc4c4c4c4 &&
        manager[0x64] == 0 &&
        manager[0xa0] == 0 &&
        manager[0xc0] == 0 &&
        manager[0xd8] == 0 &&
        manager[0xd9] == 0 &&
        zeros;
    if (!pass)
    {
        std::printf(
            "FSE2_005953e2_TEST FAIL result=%d events=%d "
            "strings=%d allocator=%d zeros=%d\n",
            result == manager ? 1 : 0,
            g_eventCount,
            g_stringCalls,
            g_listAllocator != 0 ? 1 : 0,
            zeros ? 1 : 0);
        return 1;
    }
    std::puts("FSE2_005953e2_TEST PASS");
    return 0;
}
