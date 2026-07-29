#include <stdio.h>

extern "C" unsigned char __fastcall
CInputTypeMouseButtonEvent_IsEventOfThisType_0066eee0(
    void* input_type, void*, const void* event);

struct InputType
{
    void* vtable;
    long button;
};

struct Event
{
    long category;
    long type;
};

static int g_category_calls;
static int g_type_calls;
static void* g_last_event;

extern "C" long __fastcall
CInputTypeMouseButtonEvent_IsEventOfThisType_0066eee0_GetCategory(
    void* event, void*)
{
    ++g_category_calls;
    g_last_event = event;
    return ((Event*)event)->category;
}

extern "C" long __fastcall
CInputTypeMouseButtonEvent_IsEventOfThisType_0066eee0_GetType(
    void* event, void*)
{
    ++g_type_calls;
    g_last_event = event;
    return ((Event*)event)->type;
}

static void ResetCalls()
{
    g_category_calls = 0;
    g_type_calls = 0;
    g_last_event = 0;
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
    int expected_type_calls)
{
    int ok = 1;
    ResetCalls();
    ok &= Check(
        CInputTypeMouseButtonEvent_IsEventOfThisType_0066eee0(
            &input_type, 0, &event) == expected,
        "mouse event predicate return must match");
    ok &= Check(g_category_calls == 1,
                "category must be queried exactly once");
    ok &= Check(g_type_calls == expected_type_calls,
                "type query count must match retail short-circuit path");
    ok &= Check(g_last_event == &event,
                "event helper must receive the original event");
    return ok;
}

int main()
{
    int ok = 1;
    InputType input_type;
    Event event;

    input_type.vtable = 0;
    event.category = 2;
    event.type = 4;
    input_type.button = 1;
    ok &= RunCase(input_type, event, 0, 0);

    event.category = 3;
    event.type = 4;
    input_type.button = 1;
    ok &= RunCase(input_type, event, 1, 1);
    input_type.button = 2;
    ok &= RunCase(input_type, event, 0, 1);

    event.type = 8;
    input_type.button = 3;
    ok &= RunCase(input_type, event, 1, 5);

    event.type = 11;
    input_type.button = 2;
    ok &= RunCase(input_type, event, 1, 8);

    event.type = 22;
    input_type.button = 8;
    ok &= RunCase(input_type, event, 1, 12);
    event.type = 26;
    input_type.button = 12;
    ok &= RunCase(input_type, event, 1, 12);

    event.type = 27;
    input_type.button = 8;
    ok &= RunCase(input_type, event, 1, 14);
    event.type = 31;
    input_type.button = 12;
    ok &= RunCase(input_type, event, 1, 14);

    event.type = 32;
    input_type.button = 8;
    ok &= RunCase(input_type, event, 1, 16);
    event.type = 36;
    input_type.button = 12;
    ok &= RunCase(input_type, event, 1, 16);

    event.type = 21;
    input_type.button = 7;
    ok &= RunCase(input_type, event, 0, 12);

    event.type = 37;
    input_type.button = 13;
    ok &= RunCase(input_type, event, 0, 15);

    if (!ok)
        return 1;

    puts("PASS CInputTypeMouseButtonEvent::IsEventOfThisType");
    return 0;
}
