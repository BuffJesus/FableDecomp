#include <stdio.h>

extern "C" unsigned char __fastcall
CInputTypeMouseMovementEvent_IsEventOfThisTypeInQueue_0066f440(
    void* input_type, void*, long event_type, long index, float* strength);

static long g_manager_storage;
extern "C" void*
CInputTypeMouseMovementEvent_IsEventOfThisTypeInQueue_0066f440_Manager =
    &g_manager_storage;

static int g_sequence;
static int g_construct_order;
static int g_query_order;
static int g_destroy_order;
static void* g_temporary;
static void* g_query_manager;
static long g_query_kind;
static long g_query_state;
static long g_query_index;
static unsigned char g_query_result;

extern "C" void __fastcall
CInputTypeMouseMovementEvent_IsEventOfThisTypeInQueue_0066f440_Construct(
    void* event, void*)
{
    g_construct_order = ++g_sequence;
    g_temporary = event;
}

extern "C" unsigned char __fastcall
CInputTypeMouseMovementEvent_IsEventOfThisTypeInQueue_0066f440_Query(
    void* manager, void*, long kind, long state, void* event, long index)
{
    g_query_order = ++g_sequence;
    g_query_manager = manager;
    g_query_kind = kind;
    g_query_state = state;
    g_query_index = index;
    if (event != g_temporary)
        return 0;
    return g_query_result;
}

extern "C" void __fastcall
CInputTypeMouseMovementEvent_IsEventOfThisTypeInQueue_0066f440_Destroy(
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
    g_query_kind = 0;
    g_query_state = -1;
    g_query_index = -1;
    g_query_result = result;
}

static int RunCase(long index, unsigned char result)
{
    int ok = 1;
    float strength = 8.0f;

    Reset(result);
    ok &= Check(
        CInputTypeMouseMovementEvent_IsEventOfThisTypeInQueue_0066f440(
            0, 0, 0x44, index, &strength) == result,
        "query result must be returned after cleanup");
    ok &= Check(g_construct_order == 1 &&
                g_query_order == 2 &&
                g_destroy_order == 3,
                "temporary must be constructed, queried, then destroyed");
    ok &= Check(g_query_manager == &g_manager_storage,
                "query must use the global input manager");
    ok &= Check(g_query_kind == 0x0d && g_query_state == 0,
                "query must request mouse movement and state zero");
    ok &= Check(g_query_index == index,
                "query must forward the queue index");
    return ok;
}

int main()
{
    int ok = 1;

    ok &= RunCase(1, 1);
    ok &= RunCase(6, 0);

    if (!ok)
        return 1;

    puts("PASS CInputTypeMouseMovementEvent::IsEventOfThisTypeInQueue 0x0066F440");
    return 0;
}
