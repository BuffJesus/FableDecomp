#include <stdio.h>

extern "C" unsigned char __fastcall
CInputTypeXboxPadButtonEvent_IsEventOfThisTypeInQueue_0066f750(
    void* input_type, void*, long general_type, long index, float* strength);

struct InputType
{
    void* vtable;
    long button;
};

static long g_manager_storage;
extern "C" void*
CInputTypeXboxPadButtonEvent_IsEventOfThisTypeInQueue_0066f750_Manager =
    &g_manager_storage;

static int g_sequence;
static int g_construct_order;
static int g_query_order;
static int g_strength_order;
static int g_destroy_order;
static void* g_temporary;
static void* g_query_manager;
static long g_query_kind;
static long g_query_button;
static long g_query_index;
static unsigned char g_query_result;
static float g_return_strength;

extern "C" void __fastcall
CInputTypeXboxPadButtonEvent_IsEventOfThisTypeInQueue_0066f750_Construct(
    void* event, void*)
{
    g_construct_order = ++g_sequence;
    g_temporary = event;
}

extern "C" unsigned char __fastcall
CInputTypeXboxPadButtonEvent_IsEventOfThisTypeInQueue_0066f750_Query(
    void* manager, void*, long kind, long button, void* event, long index)
{
    g_query_order = ++g_sequence;
    g_query_manager = manager;
    g_query_kind = kind;
    g_query_button = button;
    g_query_index = index;
    if (event != g_temporary)
        return 0;
    return g_query_result;
}

extern "C" float __fastcall
CInputTypeXboxPadButtonEvent_IsEventOfThisTypeInQueue_0066f750_GetStrength(
    void* event, void*)
{
    g_strength_order = ++g_sequence;
    if (event != g_temporary)
        return -99.0f;
    return g_return_strength;
}

extern "C" void __fastcall
CInputTypeXboxPadButtonEvent_IsEventOfThisTypeInQueue_0066f750_Destroy(
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

static void Reset(unsigned char result, float strength)
{
    g_sequence = 0;
    g_construct_order = 0;
    g_query_order = 0;
    g_strength_order = 0;
    g_destroy_order = 0;
    g_temporary = 0;
    g_query_manager = 0;
    g_query_kind = -1;
    g_query_button = -1;
    g_query_index = -1;
    g_query_result = result;
    g_return_strength = strength;
}

static int RunCase(
    InputType& input_type, long general_type, long expected_kind,
    long index, unsigned char result)
{
    int ok = 1;
    float strength = 6.75f;
    float returned_strength = 0.375f;

    Reset(result, returned_strength);
    ok &= Check(
        CInputTypeXboxPadButtonEvent_IsEventOfThisTypeInQueue_0066f750(
            &input_type, 0, general_type, index, &strength) == result,
        "queue result must be returned");
    ok &= Check(g_construct_order == 1 && g_query_order == 2,
                "temporary construction must precede the query");
    ok &= Check(g_query_manager == &g_manager_storage,
                "query must use the global input manager");
    ok &= Check(g_query_kind == expected_kind &&
                g_query_button == input_type.button &&
                g_query_index == index,
                "query arguments must match the mapped request");
    if (result)
    {
        ok &= Check(g_strength_order == 3 && g_destroy_order == 4,
                    "successful query must read strength before destruction");
        ok &= Check(strength == returned_strength,
                    "successful query must copy analogue strength");
    }
    else
    {
        ok &= Check(g_strength_order == 0 && g_destroy_order == 3,
                    "failed query must skip strength and destroy the temporary");
        ok &= Check(strength == 6.75f,
                    "failed query must preserve caller strength");
    }
    return ok;
}

int main()
{
    int ok = 1;
    InputType input_type;

    input_type.vtable = 0;
    input_type.button = 9;

    ok &= RunCase(input_type, 1, 0x13, 2, 1);
    ok &= RunCase(input_type, 2, 0x14, 4, 1);
    ok &= RunCase(input_type, 3, 0x15, 6, 0);
    ok &= RunCase(input_type, 7, 0, 8, 0);

    if (!ok)
        return 1;

    puts("PASS CInputTypeXboxPadButtonEvent::IsEventOfThisTypeInQueue 0x0066F750");
    return 0;
}
