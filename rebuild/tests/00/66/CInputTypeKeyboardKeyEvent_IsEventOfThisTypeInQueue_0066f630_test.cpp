#include <stdio.h>

extern "C" unsigned char __fastcall
CInputTypeKeyboardKeyEvent_IsEventOfThisTypeInQueue_0066f630(
    void* input_type, void*, long event_type, long index, float* strength);

struct InputType
{
    void* vtable;
    long key;
};

static long g_manager_storage;
extern "C" void*
CInputTypeKeyboardKeyEvent_IsEventOfThisTypeInQueue_0066f630_Manager =
    &g_manager_storage;

static int g_sequence;
static int g_construct_order;
static int g_query_order;
static int g_destroy_order;
static void* g_temporary;
static void* g_query_manager;
static long g_query_state;
static long g_query_key;
static unsigned char g_query_result;

extern "C" void __fastcall
CInputTypeKeyboardKeyEvent_IsEventOfThisTypeInQueue_0066f630_Construct(
    void* event, void*)
{
    g_construct_order = ++g_sequence;
    g_temporary = event;
}

extern "C" unsigned char __fastcall
CInputTypeKeyboardKeyEvent_IsEventOfThisTypeInQueue_0066f630_Query(
    void* manager, void*, long state, long key, void* event)
{
    g_query_order = ++g_sequence;
    g_query_manager = manager;
    g_query_state = state;
    g_query_key = key;
    if (event != g_temporary)
        return 0;
    return g_query_result;
}

extern "C" void __fastcall
CInputTypeKeyboardKeyEvent_IsEventOfThisTypeInQueue_0066f630_Destroy(
    void* event, void*)
{
    g_destroy_order = ++g_sequence;
    if (event != g_temporary)
        g_destroy_order = -1;
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

static void Reset(unsigned char result)
{
    g_sequence = 0;
    g_construct_order = 0;
    g_query_order = 0;
    g_destroy_order = 0;
    g_temporary = 0;
    g_query_manager = 0;
    g_query_state = -1;
    g_query_key = -1;
    g_query_result = result;
}

static int RunCase(
    InputType& input_type, long event_type, long expected_state,
    unsigned char result)
{
    int ok = 1;
    float strength = 6.5f;

    Reset(result);
    ok &= Check(
        CInputTypeKeyboardKeyEvent_IsEventOfThisTypeInQueue_0066f630(
            &input_type, 0, event_type, 99, &strength) == result,
        "queue result must be returned");
    ok &= Check(g_construct_order == 1 &&
                g_query_order == 2 &&
                g_destroy_order == 3,
                "temporary must be constructed, queried, then destroyed");
    ok &= Check(g_query_manager == &g_manager_storage,
                "query must use the global input manager");
    ok &= Check(g_query_state == expected_state,
                "button event type must map to the expected query state");
    ok &= Check(g_query_key == input_type.key,
                "query must use the configured keyboard key");
    ok &= Check(strength == (result ? 1.0f : 6.5f),
                "only a successful query may write unit strength");
    return ok;
}

int main()
{
    int ok = 1;
    InputType input_type;

    input_type.vtable = 0;
    input_type.key = 0x2a;

    ok &= RunCase(input_type, 1, 1, 1);
    ok &= RunCase(input_type, 2, 2, 1);
    ok &= RunCase(input_type, 3, 3, 1);
    ok &= RunCase(input_type, 0, 0, 1);
    ok &= RunCase(input_type, 4, 0, 0);

    if (!ok)
        return 1;

    puts("PASS CInputTypeKeyboardKeyEvent::IsEventOfThisTypeInQueue 0x0066F630");
    return 0;
}
