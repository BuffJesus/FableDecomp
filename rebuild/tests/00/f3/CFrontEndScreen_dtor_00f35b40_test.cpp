#include <stdio.h>
#include <string.h>

extern "C" void __fastcall CFrontEndScreen_dtor_00f35b40(void* screen, void*);

extern "C" char FableFrontEndScreenF35B40Vtable = 0;
extern "C" char FableFrontEndScreenF35B40BaseVtable = 0;

struct ControlBlock
{
    long references;
    void (__fastcall* release)(void*, void*);
    void* payload;
};

struct BasePart
{
    void* vtable;
    void* action;
    ControlBlock* control;
};

struct Screen
{
    void* vtable;
    unsigned long padding;
    BasePart base;
};

static int g_release_count;
static int g_free_count;
static int g_base_count;
static int g_sequence;
static int g_release_sequence;
static int g_free_sequence;
static int g_base_sequence;
static int g_base_saw_cleared_action;
static void* g_released_payload;
static void* g_freed_control;
static void* g_base_this;

static void ResetEvents()
{
    g_release_count = 0;
    g_free_count = 0;
    g_base_count = 0;
    g_sequence = 0;
    g_release_sequence = 0;
    g_free_sequence = 0;
    g_base_sequence = 0;
    g_base_saw_cleared_action = 0;
    g_released_payload = 0;
    g_freed_control = 0;
    g_base_this = 0;
}

static void __fastcall ReleasePayload(void* payload, void*)
{
    ++g_release_count;
    g_released_payload = payload;
    g_release_sequence = ++g_sequence;
}

extern "C" void __cdecl FableFrontEndScreenF35B40FreeControl(void* control)
{
    ++g_free_count;
    g_freed_control = control;
    g_free_sequence = ++g_sequence;
}

extern "C" void __fastcall FableFrontEndScreenF35B40BaseDtor(void* base, void*)
{
    BasePart* part = (BasePart*)base;
    ++g_base_count;
    g_base_this = base;
    g_base_saw_cleared_action = part->action == 0 && part->control == 0;
    g_base_sequence = ++g_sequence;
}

static int Check(int condition, const char* message)
{
    if (!condition)
    {
        printf("FAIL: %s\n", message);
        return 0;
    }
    return 1;
}

static int CheckCommon(const Screen& screen)
{
    int ok = 1;
    ok &= Check(screen.vtable == &FableFrontEndScreenF35B40Vtable,
                "derived vtable must be installed");
    ok &= Check(screen.base.vtable == &FableFrontEndScreenF35B40BaseVtable,
                "base vtable must be installed");
    ok &= Check(screen.base.action == 0 && screen.base.control == 0,
                "counted action must be cleared");
    ok &= Check(g_base_count == 1 && g_base_this == &screen.base,
                "base destructor must receive the embedded base subobject");
    ok &= Check(g_base_saw_cleared_action,
                "counted action must be cleared before base destruction");
    return ok;
}

int main()
{
    int ok = 1;
    Screen screen;
    ControlBlock control;

    memset(&screen, 0xCC, sizeof(screen));
    screen.base.action = (void*)0x11111111;
    screen.base.control = 0;
    ResetEvents();
    CFrontEndScreen_dtor_00f35b40(&screen, 0);
    ok &= CheckCommon(screen);
    ok &= Check(g_release_count == 0 && g_free_count == 0,
                "null control must not release");
    ok &= Check(g_base_sequence == 1,
                "null path must directly reach base destruction");

    memset(&screen, 0xCC, sizeof(screen));
    control.references = 2;
    control.release = ReleasePayload;
    control.payload = (void*)0x22222222;
    screen.base.action = (void*)0x33333333;
    screen.base.control = &control;
    ResetEvents();
    CFrontEndScreen_dtor_00f35b40(&screen, 0);
    ok &= CheckCommon(screen);
    ok &= Check(control.references == 1,
                "shared control reference must decrement");
    ok &= Check(g_release_count == 0 && g_free_count == 0,
                "shared control must not be destroyed");
    ok &= Check(g_base_sequence == 1,
                "shared path must then reach base destruction");

    memset(&screen, 0xCC, sizeof(screen));
    control.references = 1;
    control.release = ReleasePayload;
    control.payload = (void*)0x44444444;
    screen.base.action = (void*)0x55555555;
    screen.base.control = &control;
    ResetEvents();
    CFrontEndScreen_dtor_00f35b40(&screen, 0);
    ok &= CheckCommon(screen);
    ok &= Check(control.references == 0,
                "final control reference must reach zero");
    ok &= Check(g_release_count == 1 &&
                g_released_payload == control.payload,
                "final control must release its payload");
    ok &= Check(g_free_count == 1 && g_freed_control == &control,
                "final control must be freed");
    ok &= Check(g_release_sequence == 1 &&
                g_free_sequence == 2 &&
                g_base_sequence == 3,
                "release, free, and base destruction order must match");

    if (!ok)
        return 1;

    puts("PASS CFrontEndScreen::~CFrontEndScreen 0x00F35B40");
    return 0;
}
