#include <stdio.h>

extern "C" unsigned char __fastcall
CInputTypeMouseButtonEvent_IsEventOfThisTypeInQueue_0066f6d0(
    void* input_type, void*, long general_type, long index, float* strength);

struct InputType
{
    void* vtable;
    long button;
};

static long g_manager_storage;
extern "C" void*
CInputTypeMouseButtonEvent_IsEventOfThisTypeInQueue_0066f6d0_Manager =
    &g_manager_storage;

static int g_sequence;
static int g_construct_order;
static int g_translate_order;
static int g_query_order;
static int g_destroy_order;
static void* g_temporary;
static void* g_translate_input;
static long g_general_type;
static long g_translated_type;
static void* g_query_manager;
static unsigned char g_query_result;

extern "C" void __fastcall
CInputTypeMouseButtonEvent_IsEventOfThisTypeInQueue_0066f6d0_Construct(
    void* event, void*)
{
    g_construct_order = ++g_sequence;
    g_temporary = event;
}

extern "C" long __fastcall
CInputTypeMouseButtonEvent_IsEventOfThisTypeInQueue_0066f6d0_Translate(
    void* input_type, void*, long general_type)
{
    g_translate_order = ++g_sequence;
    g_translate_input = input_type;
    g_general_type = general_type;
    return g_translated_type;
}

extern "C" unsigned char __fastcall
CInputTypeMouseButtonEvent_IsEventOfThisTypeInQueue_0066f6d0_Query(
    void* manager, void*, long event_type, void* event)
{
    g_query_order = ++g_sequence;
    g_query_manager = manager;
    if (event != g_temporary || event_type != g_translated_type)
        return 0;
    return g_query_result;
}

extern "C" void __fastcall
CInputTypeMouseButtonEvent_IsEventOfThisTypeInQueue_0066f6d0_Destroy(
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

static void Reset(long translated_type, unsigned char result)
{
    g_sequence = 0;
    g_construct_order = 0;
    g_translate_order = 0;
    g_query_order = 0;
    g_destroy_order = 0;
    g_temporary = 0;
    g_translate_input = 0;
    g_general_type = -1;
    g_translated_type = translated_type;
    g_query_manager = 0;
    g_query_result = result;
}

static int RunCase(
    InputType& input_type, long general_type, long translated_type,
    unsigned char result)
{
    int ok = 1;
    float strength = 4.25f;

    Reset(translated_type, result);
    ok &= Check(
        CInputTypeMouseButtonEvent_IsEventOfThisTypeInQueue_0066f6d0(
            &input_type, 0, general_type, 71, &strength) == result,
        "queue result must be returned");
    ok &= Check(g_construct_order == 1 &&
                g_translate_order == 2 &&
                g_query_order == 3 &&
                g_destroy_order == 4,
                "construct, translate, query, and destroy order must match");
    ok &= Check(g_translate_input == &input_type &&
                g_general_type == general_type,
                "translator must receive this and the general event type");
    ok &= Check(g_query_manager == &g_manager_storage,
                "query must use the global input manager");
    ok &= Check(strength == (result ? 1.0f : 4.25f),
                "only a successful query may write unit strength");
    return ok;
}

int main()
{
    int ok = 1;
    InputType input_type;

    input_type.vtable = 0;
    input_type.button = 2;

    ok &= RunCase(input_type, 1, 10, 1);
    ok &= RunCase(input_type, 3, 12, 0);
    ok &= RunCase(input_type, 0, 0, 1);

    if (!ok)
        return 1;

    puts("PASS CInputTypeMouseButtonEvent::IsEventOfThisTypeInQueue 0x0066F6D0");
    return 0;
}
