#include <stdio.h>

extern "C" unsigned char __fastcall
CInputTypeXboxPadLeftStickEvent_IsEventOfThisType_0066f300(
    void* input_type, void*, const void* event);

struct Event
{
    long kind;
};

static int g_kind_calls;
static void* g_last_event;

extern "C" long __fastcall
CInputTypeXboxPadLeftStickEvent_IsEventOfThisType_0066f300_GetKind(
    void* event, void*)
{
    ++g_kind_calls;
    g_last_event = event;
    return ((Event*)event)->kind;
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

static int RunCase(Event& event, unsigned char expected)
{
    int ok = 1;
    g_kind_calls = 0;
    g_last_event = 0;
    ok &= Check(
        CInputTypeXboxPadLeftStickEvent_IsEventOfThisType_0066f300(
            0, 0, &event) == expected,
        "predicate return must match");
    ok &= Check(g_kind_calls == 1,
                "event kind must be queried once");
    ok &= Check(g_last_event == &event,
                "kind helper must receive the event argument");
    return ok;
}

int main()
{
    int ok = 1;
    Event event;

    event.kind = 0x11;
    ok &= RunCase(event, 1);

    event.kind = 0x10;
    ok &= RunCase(event, 0);

    event.kind = 0x12;
    ok &= RunCase(event, 0);

    if (!ok)
        return 1;

    puts("PASS CInputTypeXboxPadLeftStickEvent::IsEventOfThisType 0x0066F300");
    return 0;
}
