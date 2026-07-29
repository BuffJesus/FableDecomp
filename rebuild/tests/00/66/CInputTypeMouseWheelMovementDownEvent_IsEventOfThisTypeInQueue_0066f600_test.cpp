#include <stdio.h>

extern "C" unsigned char __fastcall
CInputTypeMouseWheelMovementDownEvent_IsEventOfThisTypeInQueue_0066f600(
    void* input_type, void*, long event_type, long index, float* strength);

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
    float strength = -4.5f;
    long input_type = 456;

    ok &= Check(
        CInputTypeMouseWheelMovementDownEvent_IsEventOfThisTypeInQueue_0066f600(
            &input_type, 0, 0x0e, 0, &strength) == 0,
        "wheel-down queue predicate must always return false");
    ok &= Check(strength == -4.5f && input_type == 456,
                "predicate must not modify input state");

    ok &= Check(
        CInputTypeMouseWheelMovementDownEvent_IsEventOfThisTypeInQueue_0066f600(
            0, 0, -1, 99, 0) == 0,
        "return must remain false for unrelated arguments");

    if (!ok)
        return 1;

    puts("PASS CInputTypeMouseWheelMovementDownEvent::IsEventOfThisTypeInQueue 0x0066F600");
    return 0;
}
