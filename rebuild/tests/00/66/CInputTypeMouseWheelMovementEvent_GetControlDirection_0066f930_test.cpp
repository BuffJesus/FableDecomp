#include <stdio.h>

extern "C" void* __fastcall
CInputTypeMouseWheelMovementEvent_GetControlDirection_0066f930(
    void* input_type, void*, void* result, const void* event);

struct Direction
{
    float x;
    float y;
};

struct Event
{
    float delta;
};

static int g_delta_calls;
static void* g_last_event;

extern "C" float __fastcall
CInputTypeMouseWheelMovementEvent_GetControlDirection_0066f930_GetDelta(
    void* event, void*)
{
    ++g_delta_calls;
    g_last_event = event;
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

static int RunCase(Event& event)
{
    int ok = 1;
    Direction result;
    long input_marker = 0x13572468;
    void* returned;

    g_delta_calls = 0;
    g_last_event = 0;
    result.x = 99.0f;
    result.y = 99.0f;
    returned =
        CInputTypeMouseWheelMovementEvent_GetControlDirection_0066f930(
            &input_marker, 0, &result, &event);
    ok &= Check(returned == &result,
                "hidden result buffer must be returned");
    ok &= Check(g_delta_calls == 1 && g_last_event == &event,
                "delta helper must receive the event once");
    ok &= Check(result.x == 0.0f && result.y == event.delta,
                "direction must contain zero X and the wheel delta in Y");
    ok &= Check(input_marker == 0x13572468,
                "input type object must be ignored");
    return ok;
}

int main()
{
    int ok = 1;
    Event event;

    event.delta = 2.5f;
    ok &= RunCase(event);

    event.delta = -3.75f;
    ok &= RunCase(event);

    event.delta = 0.0f;
    ok &= RunCase(event);

    if (!ok)
        return 1;

    puts("PASS CInputTypeMouseWheelMovementEvent::GetControlDirection 0x0066F930");
    return 0;
}
