#include <stdio.h>

extern "C" void* __fastcall
CInputTypeKeyboardKeyEvent_GetControlDirection_0066f610(
    void* input_type, void*, void* result, const void* event);

struct InputType
{
    void* vtable;
    long key;
    unsigned long direction_x_bits;
    unsigned long direction_y_bits;
};

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
    InputType input_type;
    Direction result;
    unsigned long event_marker = 0xa5a5a5a5;
    void* returned;

    input_type.vtable = 0;
    input_type.key = 77;
    input_type.direction_x_bits = 0xbf800000;
    input_type.direction_y_bits = 0x3f000000;
    result.x_bits = 0;
    result.y_bits = 0;

    returned = CInputTypeKeyboardKeyEvent_GetControlDirection_0066f610(
        &input_type, 0, &result, &event_marker);
    ok &= Check(returned == &result,
                "hidden result buffer must be returned");
    ok &= Check(result.x_bits == input_type.direction_x_bits &&
                result.y_bits == input_type.direction_y_bits,
                "both direction components must be copied exactly");
    ok &= Check(event_marker == 0xa5a5a5a5,
                "input event must remain untouched");

    input_type.direction_x_bits = 0x7fc00001;
    input_type.direction_y_bits = 0x80000000;
    returned = CInputTypeKeyboardKeyEvent_GetControlDirection_0066f610(
        &input_type, 0, &result, 0);
    ok &= Check(returned == &result &&
                result.x_bits == 0x7fc00001 &&
                result.y_bits == 0x80000000,
                "copy must preserve component bit patterns");

    if (!ok)
        return 1;

    puts("PASS CInputTypeKeyboardKeyEvent::GetControlDirection 0x0066F610");
    return 0;
}
