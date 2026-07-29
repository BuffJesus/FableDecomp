#include <stdio.h>
#include <string.h>

extern "C" void __fastcall
CFrontEndScreen_AddActionOnLeftClicked_009fd870(void* screen, void*, void* action);

struct ControlBlock
{
    long references;
    void (__fastcall* release)(void*, void*);
    void* payload;
};

struct CountedAction
{
    void* action;
    ControlBlock* control;
};

struct Screen
{
    unsigned char prefix[0x280];
    void* left_click_action;
    ControlBlock* left_click_control;
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

extern "C" void __cdecl FableFrontEndLeftClick9fd870FreeControl(void* control)
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
    ControlBlock old_control;
    ControlBlock new_control;
    CountedAction incoming;
    void* old_action = (void*)0x11111111;
    void* new_action = (void*)0x22222222;

    memset(&screen, 0xCC, sizeof(screen));
    old_control.references = 3;
    old_control.release = ReleasePayload;
    old_control.payload = (void*)0x33333333;
    screen.left_click_action = old_action;
    screen.left_click_control = &old_control;
    incoming.action = new_action;
    incoming.control = &old_control;
    ResetEvents();
    CFrontEndScreen_AddActionOnLeftClicked_009fd870(&screen, 0, &incoming);
    ok &= Check(screen.left_click_action == old_action,
                "same control must preserve existing action");
    ok &= Check(screen.left_click_control == &old_control &&
                old_control.references == 3,
                "same control must preserve ownership");
    ok &= Check(g_release_count == 0 && g_free_count == 0,
                "same control must not release");

    memset(&screen, 0, sizeof(screen));
    screen.left_click_action = old_action;
    incoming.action = new_action;
    incoming.control = 0;
    ResetEvents();
    CFrontEndScreen_AddActionOnLeftClicked_009fd870(&screen, 0, &incoming);
    ok &= Check(screen.left_click_action == old_action &&
                screen.left_click_control == 0,
                "two null controls must preserve existing action");

    new_control.references = 5;
    new_control.release = ReleasePayload;
    new_control.payload = (void*)0x44444444;
    screen.left_click_action = old_action;
    screen.left_click_control = 0;
    incoming.action = new_action;
    incoming.control = &new_control;
    ResetEvents();
    CFrontEndScreen_AddActionOnLeftClicked_009fd870(&screen, 0, &incoming);
    ok &= Check(screen.left_click_action == new_action &&
                screen.left_click_control == &new_control,
                "new counted action must be stored");
    ok &= Check(new_control.references == 6,
                "new control reference must increment");
    ok &= Check(g_release_count == 0 && g_free_count == 0,
                "null old control must not release");

    old_control.references = 2;
    old_control.release = ReleasePayload;
    old_control.payload = (void*)0x55555555;
    new_control.references = 7;
    screen.left_click_action = old_action;
    screen.left_click_control = &old_control;
    incoming.action = new_action;
    incoming.control = &new_control;
    ResetEvents();
    CFrontEndScreen_AddActionOnLeftClicked_009fd870(&screen, 0, &incoming);
    ok &= Check(old_control.references == 1 &&
                new_control.references == 8,
                "replacement must transfer counted ownership");
    ok &= Check(screen.left_click_action == new_action &&
                screen.left_click_control == &new_control,
                "replacement must update both stored fields");
    ok &= Check(g_release_count == 0 && g_free_count == 0,
                "shared old control must not be destroyed");

    old_control.references = 1;
    old_control.release = ReleasePayload;
    old_control.payload = (void*)0x66666666;
    screen.left_click_action = old_action;
    screen.left_click_control = &old_control;
    incoming.action = new_action;
    incoming.control = 0;
    ResetEvents();
    CFrontEndScreen_AddActionOnLeftClicked_009fd870(&screen, 0, &incoming);
    ok &= Check(old_control.references == 0,
                "final old reference must reach zero");
    ok &= Check(g_release_count == 1 &&
                g_released_payload == old_control.payload,
                "final old control must release its payload");
    ok &= Check(g_free_count == 1 && g_freed_control == &old_control,
                "final old control must be freed");
    ok &= Check(g_release_sequence == 1 && g_free_sequence == 2,
                "payload release must precede control free");
    ok &= Check(screen.left_click_action == new_action &&
                screen.left_click_control == 0,
                "null replacement must be stored");

    if (!ok)
        return 1;

    puts("PASS CFrontEndScreen::AddActionOnLeftClicked 0x009FD870");
    return 0;
}
