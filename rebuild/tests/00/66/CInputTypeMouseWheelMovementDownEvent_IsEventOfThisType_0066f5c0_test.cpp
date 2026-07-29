#include <stdio.h>

extern "C" unsigned char __fastcall
CInputTypeMouseWheelMovementDownEvent_IsEventOfThisType_0066f5c0(
    void* input_type, void*, const void* event);

struct Event
{
    long kind;
    float delta;
};

extern "C" float
CInputTypeMouseWheelMovementDownEvent_IsEventOfThisType_0066f5c0_Threshold =
    0.0001f;

static int g_kind_calls;
static int g_delta_calls;
static void* g_last_kind_event;
static void* g_last_delta_event;

extern "C" long __fastcall
CInputTypeMouseWheelMovementDownEvent_IsEventOfThisType_0066f5c0_GetKind(
    void* event, void*)
{
    ++g_kind_calls;
    g_last_kind_event = event;
    return ((Event*)event)->kind;
}

extern "C" float __fastcall
CInputTypeMouseWheelMovementDownEvent_IsEventOfThisType_0066f5c0_GetDelta(
    void* event, void*)
{
    ++g_delta_calls;
    g_last_delta_event = event;
    return ((Event*)event)->delta;
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
    Event& event, unsigned char expected, int expected_delta_calls)
{
    int ok = 1;
    g_kind_calls = 0;
    g_delta_calls = 0;
    g_last_kind_event = 0;
    g_last_delta_event = 0;
    ok &= Check(
        CInputTypeMouseWheelMovementDownEvent_IsEventOfThisType_0066f5c0(
            0, 0, &event) == expected,
        "predicate return must match");
    ok &= Check(g_kind_calls == 1 && g_last_kind_event == &event,
                "kind helper must receive the event once");
    ok &= Check(g_delta_calls == expected_delta_calls,
                "delta query count must match the short-circuit path");
    if (expected_delta_calls)
    {
        ok &= Check(g_last_delta_event == &event,
                    "delta helper must receive the event");
    }
    return ok;
}

int main()
{
    int ok = 1;
    Event event;

    event.kind = 0x0e;
    event.delta = -0.25f;
    ok &= RunCase(event, 1, 1);

    event.delta = -0.0001f;
    ok &= RunCase(event, 0, 1);

    event.delta = 0.25f;
    ok &= RunCase(event, 0, 1);

    event.kind = 0x0d;
    event.delta = -1.0f;
    ok &= RunCase(event, 0, 0);

    if (!ok)
        return 1;

    puts("PASS CInputTypeMouseWheelMovementDownEvent::IsEventOfThisType 0x0066F5C0");
    return 0;
}
