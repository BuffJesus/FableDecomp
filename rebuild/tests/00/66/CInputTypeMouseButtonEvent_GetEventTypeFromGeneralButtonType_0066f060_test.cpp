#include <stdio.h>

extern "C" long __fastcall
CInputTypeMouseButtonEvent_GetEventTypeFromGeneralButtonType_0066f060(
    void* input_type, void*, long general_type);

extern "C" void* CInputTypeMouseButtonEvent_0066f060_Table3[12];
extern "C" void* CInputTypeMouseButtonEvent_0066f060_Table2[12];
extern "C" void* CInputTypeMouseButtonEvent_0066f060_Table1[12];

void* CInputTypeMouseButtonEvent_0066f060_Table3[12];
void* CInputTypeMouseButtonEvent_0066f060_Table2[12];
void* CInputTypeMouseButtonEvent_0066f060_Table1[12];

struct InputType
{
    void* vtable;
    long button;
};

static void InitialiseTables()
{
    unsigned char* base =
        (unsigned char*)CInputTypeMouseButtonEvent_GetEventTypeFromGeneralButtonType_0066f060;
    int i;
    for (i = 0; i < 12; ++i)
    {
        CInputTypeMouseButtonEvent_0066f060_Table1[i] = base + 0x113;
        CInputTypeMouseButtonEvent_0066f060_Table2[i] = base + 0x113;
        CInputTypeMouseButtonEvent_0066f060_Table3[i] = base + 0x113;
    }

    CInputTypeMouseButtonEvent_0066f060_Table3[0] = base + 0x2B;
    CInputTypeMouseButtonEvent_0066f060_Table3[1] = base + 0x3B;
    CInputTypeMouseButtonEvent_0066f060_Table3[2] = base + 0x33;
    CInputTypeMouseButtonEvent_0066f060_Table3[7] = base + 0x43;
    CInputTypeMouseButtonEvent_0066f060_Table3[8] = base + 0x4B;
    CInputTypeMouseButtonEvent_0066f060_Table3[9] = base + 0x53;
    CInputTypeMouseButtonEvent_0066f060_Table3[10] = base + 0x5B;
    CInputTypeMouseButtonEvent_0066f060_Table3[11] = base + 0x63;

    CInputTypeMouseButtonEvent_0066f060_Table2[0] = base + 0x81;
    CInputTypeMouseButtonEvent_0066f060_Table2[1] = base + 0x91;
    CInputTypeMouseButtonEvent_0066f060_Table2[2] = base + 0x89;
    CInputTypeMouseButtonEvent_0066f060_Table2[7] = base + 0x99;
    CInputTypeMouseButtonEvent_0066f060_Table2[8] = base + 0xA1;
    CInputTypeMouseButtonEvent_0066f060_Table2[9] = base + 0xA9;
    CInputTypeMouseButtonEvent_0066f060_Table2[10] = base + 0xB1;
    CInputTypeMouseButtonEvent_0066f060_Table2[11] = base + 0xB9;

    CInputTypeMouseButtonEvent_0066f060_Table1[0] = base + 0xD3;
    CInputTypeMouseButtonEvent_0066f060_Table1[1] = base + 0xE3;
    CInputTypeMouseButtonEvent_0066f060_Table1[2] = base + 0xDB;
    CInputTypeMouseButtonEvent_0066f060_Table1[7] = base + 0xEB;
    CInputTypeMouseButtonEvent_0066f060_Table1[8] = base + 0xF3;
    CInputTypeMouseButtonEvent_0066f060_Table1[9] = base + 0xFB;
    CInputTypeMouseButtonEvent_0066f060_Table1[10] = base + 0x103;
    CInputTypeMouseButtonEvent_0066f060_Table1[11] = base + 0x10B;
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

static int Expected(long general_type, long button)
{
    if (button == 1)
        return general_type == 1 ? 4 : general_type == 2 ? 5 : 6;
    if (button == 2)
        return general_type == 1 ? 10 : general_type == 2 ? 11 : 12;
    if (button == 3)
        return general_type == 1 ? 7 : general_type == 2 ? 8 : 9;
    if (button >= 8 && button <= 12)
    {
        if (general_type == 1)
            return button + 14;
        if (general_type == 2)
            return button + 19;
        return button + 24;
    }
    return 0;
}

int main()
{
    int ok = 1;
    InputType input_type;
    const long buttons[] = {1, 2, 3, 8, 9, 10, 11, 12};
    int general_type;
    int i;

    InitialiseTables();
    input_type.vtable = 0;
    for (general_type = 1; general_type <= 3; ++general_type)
    {
        for (i = 0; i < (int)(sizeof(buttons) / sizeof(buttons[0])); ++i)
        {
            input_type.button = buttons[i];
            ok &= Check(
                CInputTypeMouseButtonEvent_GetEventTypeFromGeneralButtonType_0066f060(
                    &input_type, 0, general_type) ==
                    Expected(general_type, buttons[i]),
                "valid mapping must match recovered switch table");
        }
    }

    input_type.button = 4;
    ok &= Check(
        CInputTypeMouseButtonEvent_GetEventTypeFromGeneralButtonType_0066f060(
            &input_type, 0, 1) == 0,
        "sparse invalid button inside table range must return zero");
    input_type.button = 0;
    ok &= Check(
        CInputTypeMouseButtonEvent_GetEventTypeFromGeneralButtonType_0066f060(
            &input_type, 0, 2) == 0,
        "button below table range must return zero");
    input_type.button = 13;
    ok &= Check(
        CInputTypeMouseButtonEvent_GetEventTypeFromGeneralButtonType_0066f060(
            &input_type, 0, 3) == 0,
        "button above table range must return zero");
    input_type.button = 1;
    ok &= Check(
        CInputTypeMouseButtonEvent_GetEventTypeFromGeneralButtonType_0066f060(
            &input_type, 0, 0) == 0 &&
        CInputTypeMouseButtonEvent_GetEventTypeFromGeneralButtonType_0066f060(
            &input_type, 0, 4) == 0,
        "general type outside one through three must return zero");

    if (!ok)
        return 1;

    puts("PASS CInputTypeMouseButtonEvent::GetEventTypeFromGeneralButtonType");
    return 0;
}
