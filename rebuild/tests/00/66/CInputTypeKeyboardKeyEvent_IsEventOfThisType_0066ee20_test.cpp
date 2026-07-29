#include <stdio.h>

extern "C" unsigned char __fastcall
CInputTypeKeyboardKeyEvent_IsEventOfThisType_0066ee20(
    void* input_type, void*, const void* event);

struct InputType
{
    void* vtable;
    long key;
};

struct Event
{
    long kind;
    long key;
};

static int g_kind_calls;
static int g_key_calls;
static void* g_last_kind_event;
static void* g_last_key_event;

extern "C" long __fastcall
CInputTypeKeyboardKeyEvent_IsEventOfThisType_0066ee20_GetKind(
    void* event, void*)
{
    ++g_kind_calls;
    g_last_kind_event = event;
    return ((Event*)event)->kind;
}

extern "C" long __fastcall
CInputTypeKeyboardKeyEvent_IsEventOfThisType_0066ee20_GetKey(
    void* event, void*)
{
    ++g_key_calls;
    g_last_key_event = event;
    return ((Event*)event)->key;
}

static void ResetCalls()
{
    g_kind_calls = 0;
    g_key_calls = 0;
    g_last_kind_event = 0;
    g_last_key_event = 0;
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
    InputType& input_type, Event& event, unsigned char expected,
    int expected_kind_calls, int expected_key_calls)
{
    int ok = 1;
    ResetCalls();
    ok &= Check(
        CInputTypeKeyboardKeyEvent_IsEventOfThisType_0066ee20(
            &input_type, 0, &event) == expected,
        "predicate return must match");
    ok &= Check(g_kind_calls == expected_kind_calls,
                "kind query count must match short-circuit path");
    ok &= Check(g_key_calls == expected_key_calls,
                "key query count must match accepted-kind path");
    ok &= Check(g_last_kind_event == &event,
                "kind helper must receive the event");
    if (expected_key_calls)
    {
        ok &= Check(g_last_key_event == &event,
                    "key helper must receive the event");
    }
    return ok;
}

int main()
{
    int ok = 1;
    InputType input_type;
    Event event;

    input_type.vtable = 0;
    input_type.key = 42;
    event.key = 42;

    event.kind = 1;
    ok &= RunCase(input_type, event, 1, 1, 1);

    event.kind = 2;
    ok &= RunCase(input_type, event, 1, 2, 1);

    event.kind = 3;
    ok &= RunCase(input_type, event, 1, 3, 1);

    event.kind = 4;
    ok &= RunCase(input_type, event, 0, 3, 0);

    event.kind = 1;
    event.key = 99;
    ok &= RunCase(input_type, event, 0, 1, 1);

    if (!ok)
        return 1;

    puts("PASS CInputTypeKeyboardKeyEvent::IsEventOfThisType");
    return 0;
}
