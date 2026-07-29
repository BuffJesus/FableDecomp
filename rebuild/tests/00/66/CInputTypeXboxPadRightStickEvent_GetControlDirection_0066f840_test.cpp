#include <math.h>
#include <stdio.h>

extern "C" void* __fastcall
CInputTypeXboxPadRightStickEvent_GetControlDirection_0066f840(
    void* input_type, void*, void* result, const void* event);

struct Direction
{
    float x;
    float y;
};

struct Event
{
    Direction position;
};

extern "C" float
CInputTypeXboxPadRightStickEvent_GetControlDirection_0066f840_FlipY =
    -1.0f;
extern "C" float
CInputTypeXboxPadRightStickEvent_GetControlDirection_0066f840_Epsilon =
    0.0001f;
extern "C" float
CInputTypeXboxPadRightStickEvent_GetControlDirection_0066f840_Unit =
    1.0f;

static int g_position_calls;
static void* g_last_event;
static void* g_last_result;

extern "C" void* __fastcall
CInputTypeXboxPadRightStickEvent_GetControlDirection_0066f840_GetPosition(
    void* event, void*, void* result)
{
    ++g_position_calls;
    g_last_event = event;
    g_last_result = result;
    *(Direction*)result = ((Event*)event)->position;
    return result;
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

static int NearlyEqual(float left, float right)
{
    return (float)fabs(left - right) < 0.00001f;
}

static int RunCase(
    Event& event, float expected_x, float expected_y)
{
    int ok = 1;
    Direction result;
    long input_marker = 0x24681357;
    void* returned;

    g_position_calls = 0;
    g_last_event = 0;
    g_last_result = 0;
    result.x = 99.0f;
    result.y = 99.0f;
    returned =
        CInputTypeXboxPadRightStickEvent_GetControlDirection_0066f840(
            &input_marker, 0, &result, &event);
    ok &= Check(returned == &result,
                "hidden result buffer must be returned");
    ok &= Check(g_position_calls == 1 && g_last_event == &event,
                "position helper must receive the event once");
    ok &= Check(g_last_result != &result,
                "position helper must populate a local temporary");
    ok &= Check(NearlyEqual(result.x, expected_x) &&
                NearlyEqual(result.y, expected_y),
                "returned control direction must match");
    ok &= Check(input_marker == 0x24681357,
                "input type object must be ignored");
    return ok;
}

int main()
{
    int ok = 1;
    Event event;

    event.position.x = 5.0f;
    event.position.y = -12.0f;
    ok &= RunCase(event, 5.0f / 13.0f, 12.0f / 13.0f);

    event.position.x = -0.25f;
    event.position.y = 0.5f;
    ok &= RunCase(event, -0.25f, -0.5f);

    event.position.x = 0.0f;
    event.position.y = 0.0f;
    ok &= RunCase(event, 0.0f, 0.0f);

    if (!ok)
        return 1;

    puts("PASS CInputTypeXboxPadRightStickEvent::GetControlDirection 0x0066F840");
    return 0;
}
