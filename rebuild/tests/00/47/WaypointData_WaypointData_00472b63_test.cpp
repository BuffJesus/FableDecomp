#include <cstdio>

extern void* const WaypointApplyVtbl_0123543c[4];
extern void* const WaypointDestroyVtbl_01230ba0[4];

void* const WaypointApplyVtbl_0123543c[4]   = { 0, 0, 0, 0 };
void* const WaypointDestroyVtbl_01230ba0[4] = { 0, 0, 0, 0 };

struct WaypointTemp {
    void* head;
    char  pad[0x40 - 4];
    void Construct();
    void Destroy();
};

struct WaypointData {
    void Apply(void* arg, WaypointTemp* temp);
    void Dtor(void* arg);
};

static int g_order = 0;
static int g_construct_seq = 0;
static int g_apply_seq = 0;
static int g_destroy_seq = 0;
static WaypointTemp* g_construct_temp = 0;
static WaypointTemp* g_apply_temp = 0;
static WaypointTemp* g_destroy_temp = 0;
static void* g_apply_self = 0;
static void* g_apply_arg = 0;
static void* g_apply_head = 0;
static void* g_destroy_head = 0;

void WaypointTemp::Construct()
{
    g_construct_temp = this;
    g_construct_seq = ++g_order;
}

void WaypointTemp::Destroy()
{
    g_destroy_temp = this;
    g_destroy_head = this->head;
    g_destroy_seq = ++g_order;
}

void WaypointData::Apply(void* arg, WaypointTemp* temp)
{
    g_apply_self = this;
    g_apply_arg = arg;
    g_apply_temp = temp;
    g_apply_head = temp->head;
    g_apply_seq = ++g_order;
}

void WaypointData::Dtor(void* arg)
{
    WaypointTemp temp;

    temp.Construct();
    temp.head = (void*)WaypointApplyVtbl_0123543c;
    this->Apply(arg, &temp);
    temp.head = (void*)WaypointDestroyVtbl_01230ba0;
    temp.Destroy();
}

int main()
{
    WaypointData self;
    int arg = 0x1234;

    self.Dtor(&arg);

    int ok = 1;
    ok &= (g_construct_seq == 1);
    ok &= (g_apply_seq == 2);
    ok &= (g_destroy_seq == 3);
    ok &= (g_apply_self == (void*)&self);
    ok &= (g_apply_arg == (void*)&arg);
    ok &= (g_construct_temp == g_apply_temp);
    ok &= (g_apply_temp == g_destroy_temp);
    ok &= (g_apply_head == (void*)WaypointApplyVtbl_0123543c);
    ok &= (g_destroy_head == (void*)WaypointDestroyVtbl_01230ba0);

    if (ok)
        std::printf("WAYPOINTDATA_DTOR_TEMP_APPLY_00472B63_TEST PASS\n");
    else
        std::printf("WAYPOINTDATA_DTOR_TEMP_APPLY_00472B63_TEST FAIL "
                    "cseq=%d aseq=%d dseq=%d\n",
                    g_construct_seq, g_apply_seq, g_destroy_seq);
    return 0;
}