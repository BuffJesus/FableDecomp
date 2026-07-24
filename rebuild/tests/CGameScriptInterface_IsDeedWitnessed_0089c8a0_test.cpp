// Standalone test for CGameScriptInterface::IsDeedWitnessed (retail 0x0089c8a0)
// Self-contained: defines its own copy of the function plus recording stubs (as
// out-of-line member methods matching the __fastcall model), exercises every branch,
// prints exactly the pass_pattern on success.
#include <cstdio>

struct GSI_Self
{
    unsigned char pad00[0x14];
    void*         field14;   // [this+0x14]
};

struct TCEntry
{
    int   key;      // +0x00
    void* iface;    // +0x04
};

struct IDeedIface
{
    bool Final(void* arg0, int one);
};

struct TCMap
{
    void*     begin;   // +0x00
    TCEntry*  end;     // +0x04
    TCEntry*  LowerBound(const int* pKey);
};

struct GSI_Target;

struct GSI_HelperAObj { void* HelperA(); };
struct GSI_HelperBObj { GSI_Target* HelperB(); };

struct GSI_Target
{
    unsigned char pad00[0x28];
    unsigned int  field28;   // +0x28
    unsigned char pad2c[0x18];
    TCMap         map;       // +0x44
    unsigned char pad4c[0x45];
    unsigned char flags91;   // +0x91
};

// ---- recording state ----
static void*       g_helperA_this = 0;
static void*       g_helperA_ret  = 0;
static void*       g_helperB_this = 0;
static GSI_Target* g_helperB_ret  = 0;

static TCMap*      g_lb_this      = 0;
static int         g_lb_key       = -1;
static TCEntry*    g_lb_ret       = 0;

static void*       g_final_this   = 0;
static void*       g_final_arg0   = 0;
static int         g_final_one    = -1;
static bool        g_final_ret    = false;
static int         g_final_calls  = 0;

void* GSI_HelperAObj::HelperA()          { g_helperA_this = this; return g_helperA_ret; }
GSI_Target* GSI_HelperBObj::HelperB()    { g_helperB_this = this; return g_helperB_ret; }
TCEntry* TCMap::LowerBound(const int* p) { g_lb_this = this; g_lb_key = *p; return g_lb_ret; }
bool IDeedIface::Final(void* arg0, int one)
{
    g_final_this = this; g_final_arg0 = arg0; g_final_one = one;
    ++g_final_calls; return g_final_ret;
}

// ---- function under test (copy of the reconstruction) ----
bool __fastcall CGameScriptInterface_IsDeedWitnessed(const void* thisPtr,
                                                     unsigned long /*edx*/,
                                                     void* arg0)
{
    const GSI_Self* self = reinterpret_cast<const GSI_Self*>(thisPtr);

    GSI_HelperAObj* a = reinterpret_cast<GSI_HelperAObj*>(self->field14);
    GSI_HelperBObj* b = reinterpret_cast<GSI_HelperBObj*>(a->HelperA());
    GSI_Target*     target = b->HelperB();

    if (target != 0 &&
        (target->flags91 & 0x01) == 0 &&
        (target->field28 & 0x20000) != 0)
    {
        int key = 0x51;
        TCMap* container = &target->map;

        TCEntry* node = container->LowerBound(&key);
        TCEntry* end  = container->end;

        if (node != end)
        {
            if (node->key > 0x51)
                node = end;
        }
        else
        {
            node = end;
        }

        IDeedIface* iface = reinterpret_cast<IDeedIface*>(node->iface);
        return iface->Final(arg0, 1);
    }

    return false;
}

static void reset_final() { g_final_calls = 0; g_final_this = 0; g_final_arg0 = 0; g_final_one = -1; }

int main()
{
    int fails = 0;

    GSI_Self self;
    self.field14 = (void*)0xAAAA1414;

    void* ARG0 = (void*)0xDEAD0000;

    // Case 1: target == 0 -> false, no Final call
    g_helperA_ret = (void*)0xB0;
    g_helperB_ret = 0;
    reset_final();
    if (CGameScriptInterface_IsDeedWitnessed(&self, 0, ARG0) != false) { printf("c1 ret\n"); ++fails; }
    if (g_final_calls != 0) { printf("c1 final\n"); ++fails; }
    if (g_helperA_this != self.field14) { printf("c1 A this\n"); ++fails; }
    if (g_helperB_this != g_helperA_ret) { printf("c1 B this\n"); ++fails; }

    GSI_Target tgt;
    for (unsigned i = 0; i < sizeof(tgt); ++i) ((unsigned char*)&tgt)[i] = 0;
    tgt.flags91 = 0x00;
    tgt.field28 = 0x20000;

    // Case 2: flags91 bit0 set -> false
    g_helperB_ret = &tgt;
    tgt.flags91 = 0x01;
    reset_final();
    if (CGameScriptInterface_IsDeedWitnessed(&self, 0, ARG0) != false) { printf("c2 ret\n"); ++fails; }
    if (g_final_calls != 0) { printf("c2 final\n"); ++fails; }
    tgt.flags91 = 0x00;

    // Case 3: field28 bit clear -> false
    tgt.field28 = 0x00;
    reset_final();
    if (CGameScriptInterface_IsDeedWitnessed(&self, 0, ARG0) != false) { printf("c3 ret\n"); ++fails; }
    if (g_final_calls != 0) { printf("c3 final\n"); ++fails; }
    tgt.field28 = 0x20000;

    TCEntry endEntry; endEntry.key = 0; endEntry.iface = (void*)0xE0DE0000;
    tgt.map.end = &endEntry;

    // Case 4: node != end and node->key <= 0x51 (=0x50) -> keep node
    TCEntry n4; n4.key = 0x50; n4.iface = (void*)0x4444;
    g_lb_ret = &n4;
    reset_final();
    g_final_ret = true;
    if (CGameScriptInterface_IsDeedWitnessed(&self, 0, ARG0) != true) { printf("c4 ret\n"); ++fails; }
    if (g_final_calls != 1) { printf("c4 calls\n"); ++fails; }
    if (g_final_this != n4.iface) { printf("c4 this\n"); ++fails; }
    if (g_final_arg0 != ARG0) { printf("c4 arg0\n"); ++fails; }
    if (g_final_one != 1) { printf("c4 one\n"); ++fails; }
    if (g_lb_key != 0x51) { printf("c4 key\n"); ++fails; }
    if (g_lb_this != &tgt.map) { printf("c4 this-lb\n"); ++fails; }

    // Case 5: node != end and node->key > 0x51 (=0x52) -> use end
    TCEntry n5; n5.key = 0x52; n5.iface = (void*)0x5555;
    g_lb_ret = &n5;
    reset_final();
    g_final_ret = false;
    if (CGameScriptInterface_IsDeedWitnessed(&self, 0, ARG0) != false) { printf("c5 ret\n"); ++fails; }
    if (g_final_calls != 1) { printf("c5 calls\n"); ++fails; }
    if (g_final_this != endEntry.iface) { printf("c5 this(end)\n"); ++fails; }

    // Case 6: node == end -> use end
    g_lb_ret = &endEntry;
    reset_final();
    g_final_ret = true;
    if (CGameScriptInterface_IsDeedWitnessed(&self, 0, ARG0) != true) { printf("c6 ret\n"); ++fails; }
    if (g_final_calls != 1) { printf("c6 calls\n"); ++fails; }
    if (g_final_this != endEntry.iface) { printf("c6 this(end)\n"); ++fails; }

    // Case 7: boundary node->key == 0x51 -> keep node (<=)
    TCEntry n7; n7.key = 0x51; n7.iface = (void*)0x7777;
    g_lb_ret = &n7;
    reset_final();
    g_final_ret = true;
    if (CGameScriptInterface_IsDeedWitnessed(&self, 0, ARG0) != true) { printf("c7 ret\n"); ++fails; }
    if (g_final_this != n7.iface) { printf("c7 this\n"); ++fails; }

    if (fails == 0)
        printf("FSE2_0089c8a0_TEST PASS\n");
    else
        printf("FAILED %d checks\n", fails);

    return fails == 0 ? 0 : 1;
}