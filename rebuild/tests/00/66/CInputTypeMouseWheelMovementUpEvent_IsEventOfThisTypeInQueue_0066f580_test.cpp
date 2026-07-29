#include <stdio.h>

extern "C" unsigned char __fastcall
CInputTypeMouseWheelMovementUpEvent_IsEventOfThisTypeInQueue_0066f580(
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
    float strength = 7.5f;
    long input_type = 123;

    ok &= Check(
        CInputTypeMouseWheelMovementUpEvent_IsEventOfThisTypeInQueue_0066f580(
            &input_type, 0, 0x0e, 0, &strength) == 0,
        "wheel-up queue predicate must always return false");
    ok &= Check(strength == 7.5f && input_type == 123,
                "predicate must not modify input state");

    ok &= Check(
        CInputTypeMouseWheelMovementUpEvent_IsEventOfThisTypeInQueue_0066f580(
            0, 0, -1, 99, 0) == 0,
        "return must remain false for unrelated arguments");

    if (!ok)
        return 1;

    puts("PASS CInputTypeMouseWheelMovementUpEvent::IsEventOfThisTypeInQueue 0x0066F580");
    return 0;
}
