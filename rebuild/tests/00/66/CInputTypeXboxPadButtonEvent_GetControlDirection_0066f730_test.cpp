#include <stdio.h>

extern "C" void* __fastcall
CInputTypeXboxPadButtonEvent_GetControlDirection_0066f730(
    void* input_type, void*, void* result, const void* event);

struct Direction
{
    unsigned long x_bits;
    unsigned long y_bits;
};

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
    Direction result;
    unsigned long input_marker = 0x11223344;
    unsigned long event_marker = 0x55667788;
    void* returned;

    result.x_bits = 0xffffffff;
    result.y_bits = 0x7fc00001;
    returned = CInputTypeXboxPadButtonEvent_GetControlDirection_0066f730(
        &input_marker, 0, &result, &event_marker);
    ok &= Check(returned == &result,
                "hidden result buffer must be returned");
    ok &= Check(result.x_bits == 0 && result.y_bits == 0,
                "both direction components must be zeroed");
    ok &= Check(input_marker == 0x11223344 &&
                event_marker == 0x55667788,
                "input type and event must remain untouched");

    result.x_bits = 1;
    result.y_bits = 2;
    returned = CInputTypeXboxPadButtonEvent_GetControlDirection_0066f730(
        0, 0, &result, 0);
    ok &= Check(returned == &result &&
                result.x_bits == 0 &&
                result.y_bits == 0,
                "zero vector must not depend on object or event");

    if (!ok)
        return 1;

    puts("PASS CInputTypeXboxPadButtonEvent::GetControlDirection 0x0066F730");
    return 0;
}
