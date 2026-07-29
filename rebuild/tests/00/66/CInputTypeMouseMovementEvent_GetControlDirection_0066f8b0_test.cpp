#include <math.h>
#include <stdio.h>

extern "C" void* __fastcall
CInputTypeMouseMovementEvent_GetControlDirection_0066f8b0(
    void* input_type, void*, void* result, const void* event);

struct Direction
{
    float x;
    float y;
};

struct Event
{
    Direction movement;
};

extern "C" float
CInputTypeMouseMovementEvent_GetControlDirection_0066f8b0_Scale =
    0.015625f;
extern "C" float
CInputTypeMouseMovementEvent_GetControlDirection_0066f8b0_FlipY =
    -1.0f;
extern "C" float
CInputTypeMouseMovementEvent_GetControlDirection_0066f8b0_Unit =
    1.0f;

static int g_movement_calls;
static void* g_last_event;
static void* g_last_result;

extern "C" void* __fastcall
CInputTypeMouseMovementEvent_GetControlDirection_0066f8b0_GetMovement(
    void* event, void*, void* result)
{
    ++g_movement_calls;
    g_last_event = event;
    g_last_result = result;
    *(Direction*)result = ((Event*)event)->movement;
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
    long input_marker = 0x10203040;
    void* returned;

    g_movement_calls = 0;
    g_last_event = 0;
    g_last_result = 0;
    result.x = 99.0f;
    result.y = 99.0f;
    returned = CInputTypeMouseMovementEvent_GetControlDirection_0066f8b0(
        &input_marker, 0, &result, &event);
    ok &= Check(returned == &result,
                "hidden result buffer must be returned");
    ok &= Check(g_movement_calls == 1 && g_last_event == &event,
                "movement helper must receive the event once");
    ok &= Check(g_last_result != &result,
                "movement helper must populate a local temporary");
    ok &= Check(NearlyEqual(result.x, expected_x) &&
                NearlyEqual(result.y, expected_y),
                "returned control direction must match");
    ok &= Check(input_marker == 0x10203040,
                "input type object must be ignored");
    return ok;
}

int main()
{
    int ok = 1;
    Event event;

    event.movement.x = 192.0f;
    event.movement.y = 256.0f;
    ok &= RunCase(event, 0.6f, -0.8f);

    event.movement.x = -16.0f;
    event.movement.y = -32.0f;
    ok &= RunCase(event, -0.25f, 0.5f);

    event.movement.x = 0.0f;
    event.movement.y = 0.0f;
    ok &= RunCase(event, 0.0f, 0.0f);

    if (!ok)
        return 1;

    puts("PASS CInputTypeMouseMovementEvent::GetControlDirection 0x0066F8B0");
    return 0;
}
