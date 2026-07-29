#include <stdio.h>

extern "C" unsigned char __fastcall
CInputTypeKeyboardKeyEvent_operator_eq_0066f210(
    void* event, void*, const void* other);

struct Event;
typedef long (__fastcall* GetTypeFunction)(Event*, void*);

struct EventVtable
{
    void* slot0;
    GetTypeFunction get_type;
};

struct Event
{
    EventVtable* vtable;
    long value;
    long type;
    long identifier;
};

static int g_call_count;
static long g_call_order[4];

static long __fastcall GetType(Event* event, void*)
{
    g_call_order[g_call_count++] = event->identifier;
    return event->type;
}

static void ResetCalls()
{
    g_call_count = 0;
    g_call_order[0] = 0;
    g_call_order[1] = 0;
    g_call_order[2] = 0;
    g_call_order[3] = 0;
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

int main()
{
    int ok = 1;
    EventVtable vtable;
    Event left;
    Event right;

    vtable.slot0 = 0;
    vtable.get_type = GetType;
    left.vtable = &vtable;
    left.value = 91;
    left.type = 5;
    left.identifier = 1;
    right.vtable = &vtable;
    right.value = 91;
    right.type = 5;
    right.identifier = 2;

    ResetCalls();
    ok &= Check(
        CInputTypeKeyboardKeyEvent_operator_eq_0066f210(
            &left, 0, &right) == 1,
        "matching type and value must compare equal");
    ok &= Check(g_call_count == 2 &&
                g_call_order[0] == 2 &&
                g_call_order[1] == 1,
                "right discriminator must run before left");

    right.value = 92;
    ResetCalls();
    ok &= Check(
        CInputTypeKeyboardKeyEvent_operator_eq_0066f210(
            &left, 0, &right) == 0,
        "different value must compare unequal");
    ok &= Check(g_call_count == 2,
                "value mismatch must query both types");

    right.value = 91;
    right.type = 6;
    ResetCalls();
    ok &= Check(
        CInputTypeKeyboardKeyEvent_operator_eq_0066f210(
            &left, 0, &right) == 0,
        "different type must compare unequal");
    ok &= Check(g_call_count == 2 &&
                g_call_order[0] == 2 &&
                g_call_order[1] == 1,
                "type mismatch must retain discriminator order");

    if (!ok)
        return 1;

    puts("PASS CInputTypeKeyboardKeyEvent::operator== 0x0066F210");
    return 0;
}
