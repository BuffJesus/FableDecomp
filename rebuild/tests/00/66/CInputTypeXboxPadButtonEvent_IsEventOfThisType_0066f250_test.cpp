#include <stdio.h>

extern "C" unsigned char __fastcall
CInputTypeXboxPadButtonEvent_IsEventOfThisType_0066f250(
    void* input_type, void*, const void* event);

struct InputType
{
    void* vtable;
    long button;
};

struct Event
{
    long kind;
    long button;
};

static int g_kind_calls;
static int g_button_calls;
static void* g_last_kind_event;
static void* g_last_button_event;

extern "C" long __fastcall
CInputTypeXboxPadButtonEvent_IsEventOfThisType_0066f250_GetKind(
    void* event, void*)
{
    ++g_kind_calls;
    g_last_kind_event = event;
    return ((Event*)event)->kind;
}

extern "C" long __fastcall
CInputTypeXboxPadButtonEvent_IsEventOfThisType_0066f250_GetButton(
    void* event, void*)
{
    ++g_button_calls;
    g_last_button_event = event;
    return ((Event*)event)->button;
}

static void ResetCalls()
{
    g_kind_calls = 0;
    g_button_calls = 0;
    g_last_kind_event = 0;
    g_last_button_event = 0;
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

static int RunCase(
    InputType& input_type, Event& event, unsigned char expected,
    int expected_kind_calls, int expected_button_calls)
{
    int ok = 1;
    ResetCalls();
    ok &= Check(
        CInputTypeXboxPadButtonEvent_IsEventOfThisType_0066f250(
            &input_type, 0, &event) == expected,
        "predicate return must match");
    ok &= Check(g_kind_calls == expected_kind_calls,
                "kind query count must match short-circuit path");
    ok &= Check(g_button_calls == expected_button_calls,
                "button query count must match accepted-kind path");
    ok &= Check(g_last_kind_event == &event,
                "kind helper must receive the event");
    if (expected_button_calls)
    {
        ok &= Check(g_last_button_event == &event,
                    "button helper must receive the event");
    }
    return ok;
}

int main()
{
    int ok = 1;
    InputType input_type;
    Event event;

    input_type.vtable = 0;
    input_type.button = 7;
    event.button = 7;

    event.kind = 0x13;
    ok &= RunCase(input_type, event, 1, 1, 1);

    event.kind = 0x14;
    ok &= RunCase(input_type, event, 1, 2, 1);

    event.kind = 0x15;
    ok &= RunCase(input_type, event, 1, 3, 1);

    event.kind = 0x16;
    ok &= RunCase(input_type, event, 0, 3, 0);

    event.kind = 0x13;
    event.button = 8;
    ok &= RunCase(input_type, event, 0, 1, 1);

    if (!ok)
        return 1;

    puts("PASS CInputTypeXboxPadButtonEvent::IsEventOfThisType 0x0066F250");
    return 0;
}
