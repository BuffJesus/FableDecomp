#include <stdio.h>
#include <string.h>

extern "C" void __fastcall CFrontEndScreen_dtor_00835330(void* screen, void*);

struct ControlBlock
{
    long references;
    void (__fastcall* release)(void*, void*);
    void* payload;
};

struct Screen
{
    unsigned long header0;
    unsigned long header1;
    unsigned char padding[0x18];
    void* action;
    ControlBlock* control;
};

static int g_release_count;
static int g_free_count;
static int g_sequence;
static int g_release_sequence;
static int g_free_sequence;
static void* g_released_payload;
static void* g_freed_control;

static void ResetEvents()
{
    g_release_count = 0;
    g_free_count = 0;
    g_sequence = 0;
    g_release_sequence = 0;
    g_free_sequence = 0;
    g_released_payload = 0;
    g_freed_control = 0;
}

static void __fastcall ReleasePayload(void* payload, void*)
{
    ++g_release_count;
    g_released_payload = payload;
    g_release_sequence = ++g_sequence;
}

extern "C" void __cdecl FableFrontEndScreenDtor835330FreeControl(void* control)
{
    ++g_free_count;
    g_freed_control = control;
    g_free_sequence = ++g_sequence;
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

int main()
{
    int ok = 1;
    Screen screen;
    ControlBlock control;

    memset(&screen, 0xCC, sizeof(screen));
    screen.header0 = 0x11111111;
    screen.header1 = 0x22222222;
    screen.action = (void*)0x33333333;
    screen.control = 0;
    ResetEvents();
    CFrontEndScreen_dtor_00835330(&screen, 0);
    ok &= Check(screen.header0 == 0 && screen.header1 == 0,
                "destructor must clear both header words");
    ok &= Check(screen.action == 0 && screen.control == 0,
                "destructor must clear null counted action");
    ok &= Check(g_release_count == 0 && g_free_count == 0,
                "null control must not release");

    control.references = 2;
    control.release = ReleasePayload;
    control.payload = (void*)0x44444444;
    screen.header0 = 0x55555555;
    screen.header1 = 0x66666666;
    screen.action = (void*)0x77777777;
    screen.control = &control;
    ResetEvents();
    CFrontEndScreen_dtor_00835330(&screen, 0);
    ok &= Check(control.references == 1,
                "shared control reference must decrement");
    ok &= Check(screen.header0 == 0 && screen.header1 == 0,
                "shared path must clear both header words");
    ok &= Check(screen.action == 0 && screen.control == 0,
                "shared path must clear counted action");
    ok &= Check(g_release_count == 0 && g_free_count == 0,
                "shared control must not be destroyed");

    control.references = 1;
    control.release = ReleasePayload;
    control.payload = (void*)0x88888888;
    screen.header0 = 0x99999999;
    screen.header1 = 0xAAAAAAAA;
    screen.action = (void*)0xBBBBBBBB;
    screen.control = &control;
    ResetEvents();
    CFrontEndScreen_dtor_00835330(&screen, 0);
    ok &= Check(control.references == 0,
                "final control reference must reach zero");
    ok &= Check(g_release_count == 1 &&
                g_released_payload == control.payload,
                "final control must invoke payload release");
    ok &= Check(g_free_count == 1 && g_freed_control == &control,
                "final control must be freed");
    ok &= Check(g_release_sequence == 1 && g_free_sequence == 2,
                "payload release must precede control free");
    ok &= Check(screen.header0 == 0 && screen.header1 == 0,
                "final path must clear both header words");
    ok &= Check(screen.action == 0 && screen.control == 0,
                "final path must clear counted action");

    if (!ok)
        return 1;

    puts("PASS CFrontEndScreen::~CFrontEndScreen 0x00835330");
    return 0;
}
