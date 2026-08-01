#include <cstdio>

extern "C" void __fastcall
FableWaypointDataDestroy_00472B63(void* self, void* edx, void* arg);

static int g_order = 0;
static int g_construct_seq = 0;
static int g_apply_seq = 0;
static int g_destroy_seq = 0;
static void* g_apply_self = 0;
static void* g_apply_temp = 0;
static void* g_apply_arg = 0;
static void* g_construct_temp = 0;
static void* g_destroy_temp = 0;
static unsigned g_apply_head = 0;
static unsigned g_destroy_head = 0;

extern "C" void* const FableWaypointDataApplyVtable_00472B63[4] = {0,0,0,0};
extern "C" void* const FableWaypointDataDestroyVtable_00472B63[4] = {0,0,0,0};

extern "C" void __fastcall
FableWaypointDataTempConstruct_00472B63(void* temp, void* edx)
{
    (void)edx;
    g_construct_temp = temp;
    g_construct_seq = ++g_order;
}

extern "C" void __fastcall
FableWaypointDataApply_00472B63(void* self, void* edx, void* stackArg, void* temp)
{
    (void)edx;
    g_apply_self = self;
    g_apply_temp = temp;
    g_apply_arg = stackArg;
    g_apply_head = *(unsigned*)temp;
    g_apply_seq = ++g_order;
}

extern "C" void __fastcall
FableWaypointDataTempDestroy_00472B63(void* temp, void* edx)
{
    (void)edx;
    g_destroy_temp = temp;
    g_destroy_head = *(unsigned*)temp;
    g_destroy_seq = ++g_order;
}

int main()
{
    int self = 0xABCD;
    int arg  = 0x1234;

    FableWaypointDataDestroy_00472B63(&self, 0, &arg);

    int ok = 1;
    ok &= (g_construct_seq == 1);
    ok &= (g_apply_seq == 2);
    ok &= (g_destroy_seq == 3);
    ok &= (g_apply_self == (void*)&self);
    ok &= (g_apply_arg == (void*)&arg);
    ok &= (g_construct_temp == g_apply_temp);
    ok &= (g_apply_temp == g_destroy_temp);
    ok &= (g_apply_head == (unsigned)(unsigned long)&FableWaypointDataApplyVtable_00472B63[0]);
    ok &= (g_destroy_head == (unsigned)(unsigned long)&FableWaypointDataDestroyVtable_00472B63[0]);

    if (ok)
        std::printf("WAYPOINTDATA_DTOR_TEMP_APPLY_00472B63_TEST PASS\n");
    else
        std::printf("WAYPOINTDATA_DTOR_TEMP_APPLY_00472B63_TEST FAIL "
                    "cseq=%d aseq=%d dseq=%d\n",
                    g_construct_seq, g_apply_seq, g_destroy_seq);
    return 0;
}