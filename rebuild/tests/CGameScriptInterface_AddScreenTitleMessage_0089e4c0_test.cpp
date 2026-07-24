#include <cstdio>

// Standalone reproduction + behavior test for
//   CGameScriptInterface::AddScreenTitleMessage @ 0x0089e4c0

static void*        g_chain_seen;
static void*        g_a_seen;
static void*        g_A_ret;
static void*        g_B_ret;

static void*        g_lb_this;
static int          g_lb_key;
static void*        g_lb_ret;

static void*        g_d_this;
static void*        g_d_a0, *g_d_a1, *g_d_a2;
static int          g_d_z, g_d_one;
static int          g_d_calls;

struct AstmMap { void* lb(int* pKey); };
struct AstmTarget { void d(void* a0, void* a1, void* a2, int z, int one); };

extern "C" void* __fastcall ASTM_Helper_A(void* p_ecx);
extern "C" void* __fastcall ASTM_Helper_B(void* p_ecx);

void __fastcall CGameScriptInterface_AddScreenTitleMessage(
    void* self_ecx, unsigned long, void* a0, void* a1, void* volatile a2)
{
    void* chain = *(void**)((char*)self_ecx + 0x14);
    void* a = ASTM_Helper_A(chain);
    unsigned char* obj = (unsigned char*)ASTM_Helper_B(a);
    if (obj == 0) return;
    if (*(unsigned char*)(obj + 0x91) & 0x01) return;
    AstmTarget* target;
    if (*(unsigned int*)(obj + 0x30) & 0x4000) {
        int key = 0x8e;
        AstmMap* map = (AstmMap*)(obj + 0x44);
        void* node = map->lb(&key);
        void* end = *(void**)((char*)map + 0x4);
        void* pick;
        if (node == end) pick = end;
        else if (*(int*)node > 0x8e) pick = end;
        else pick = node;
        target = *(AstmTarget**)((char*)pick + 0x4);
    } else target = (AstmTarget*)a2;
    if (target == 0) return;
    target->d(a0, a1, a2, 0, 1);
}

extern "C" void* __fastcall ASTM_Helper_A(void* p_ecx) { g_chain_seen = p_ecx; return g_A_ret; }
extern "C" void* __fastcall ASTM_Helper_B(void* p_ecx) { g_a_seen = p_ecx; return g_B_ret; }

void* AstmMap::lb(int* pKey) { g_lb_this = this; g_lb_key = *pKey; return g_lb_ret; }
void  AstmTarget::d(void* a0, void* a1, void* a2, int z, int one) {
    g_d_this = this; g_d_a0 = a0; g_d_a1 = a1; g_d_a2 = a2; g_d_z = z; g_d_one = one; ++g_d_calls;
}

static const int OFF_FLAGS30 = 0x30;
static const int OFF_MAP     = 0x44;
static const int OFF_MAPEND  = OFF_MAP + 0x04;
static const int OFF_BYTE91  = 0x91;

static void reset() {
    g_chain_seen=g_a_seen=g_A_ret=g_B_ret=0;
    g_lb_this=0; g_lb_key=0; g_lb_ret=0;
    g_d_this=0; g_d_a0=g_d_a1=g_d_a2=0; g_d_z=g_d_one=0; g_d_calls=0;
}

int main() {
    unsigned char self[0x40] = {0};
    void* chainval = (void*)0x11110000;
    *(void**)(self + 0x14) = chainval;

    static unsigned char obj[0x100];

    AstmTarget targetObj;
    void* a0 = (void*)0xA000;
    void* a1 = (void*)0xA100;

    int fails = 0;

    // Case 1: obj == 0 -> no d() call
    reset();
    g_A_ret = (void*)0x22220000;
    g_B_ret = 0;
    CGameScriptInterface_AddScreenTitleMessage(self, 0, a0, a1, (void*)0xDEAD);
    if (g_chain_seen != chainval) { printf("c1 chain\n"); fails++; }
    if (g_a_seen != g_A_ret)      { printf("c1 a\n"); fails++; }
    if (g_d_calls != 0)           { printf("c1 no-call\n"); fails++; }

    // Case 2: byte[obj+0x91] & 1 -> early return
    reset();
    for (int i=0;i<0x100;i++) obj[i]=0;
    obj[OFF_BYTE91] = 0x01;
    g_A_ret = (void*)0x33330000;
    g_B_ret = obj;
    CGameScriptInterface_AddScreenTitleMessage(self, 0, a0, a1, (void*)&targetObj);
    if (g_d_calls != 0) { printf("c2 no-call\n"); fails++; }

    // Case 3: flag 0x4000 clear -> target = a2
    reset();
    for (int i=0;i<0x100;i++) obj[i]=0;
    *(unsigned int*)(obj + OFF_FLAGS30) = 0x0000;
    g_A_ret = (void*)0x44440000;
    g_B_ret = obj;
    CGameScriptInterface_AddScreenTitleMessage(self, 0, a0, a1, (void*)&targetObj);
    if (g_d_calls != 1)          { printf("c3 call\n"); fails++; }
    if (g_d_this != &targetObj)  { printf("c3 this\n"); fails++; }
    if (g_d_a0 != a0 || g_d_a1 != a1 || g_d_a2 != (void*)&targetObj) { printf("c3 args\n"); fails++; }
    if (g_d_z != 0 || g_d_one != 1) { printf("c3 zc\n"); fails++; }

    // Case 4: flag set, node!=end and *node<=0x8e -> pick=node
    reset();
    for (int i=0;i<0x100;i++) obj[i]=0;
    *(unsigned int*)(obj + OFF_FLAGS30) = 0x4000;
    static unsigned char endNode[8];
    *(void**)(obj + OFF_MAPEND) = endNode;
    static unsigned char nodeBuf[8];
    *(int*)(nodeBuf) = 0x10;
    *(void**)(nodeBuf + 4) = (void*)&targetObj;
    g_lb_ret = nodeBuf;
    g_A_ret = (void*)0x55550000;
    g_B_ret = obj;
    CGameScriptInterface_AddScreenTitleMessage(self, 0, a0, a1, (void*)0xBEEF);
    if (g_lb_this != (void*)(obj + OFF_MAP)) { printf("c4 map-this\n"); fails++; }
    if (g_lb_key != 0x8e)                    { printf("c4 key\n"); fails++; }
    if (g_d_calls != 1)                      { printf("c4 call\n"); fails++; }
    if (g_d_this != &targetObj)              { printf("c4 this\n"); fails++; }
    if (g_d_a2 != (void*)0xBEEF)             { printf("c4 a2\n"); fails++; }

    // Case 5: flag set, node!=end but *node>0x8e -> pick=end
    reset();
    for (int i=0;i<0x100;i++) obj[i]=0;
    *(unsigned int*)(obj + OFF_FLAGS30) = 0x4000;
    static unsigned char endBuf[8];
    *(int*)(endBuf) = 0;
    *(void**)(endBuf + 4) = (void*)&targetObj;
    *(void**)(obj + OFF_MAPEND) = endBuf;
    static unsigned char node5[8];
    *(int*)(node5) = 0x100;
    *(void**)(node5 + 4) = 0;
    g_lb_ret = node5;
    g_A_ret = (void*)0x66660000;
    g_B_ret = obj;
    CGameScriptInterface_AddScreenTitleMessage(self, 0, a0, a1, (void*)0xC0DE);
    if (g_d_calls != 1)         { printf("c5 call\n"); fails++; }
    if (g_d_this != &targetObj) { printf("c5 this (picked node instead of end)\n"); fails++; }

    // Case 6: flag set, node==end -> pick=end, target(end+4)==0 -> no call
    reset();
    for (int i=0;i<0x100;i++) obj[i]=0;
    *(unsigned int*)(obj + OFF_FLAGS30) = 0x4000;
    static unsigned char endBuf6[8];
    *(int*)(endBuf6) = 0x5;
    *(void**)(endBuf6 + 4) = 0;
    *(void**)(obj + OFF_MAPEND) = endBuf6;
    g_lb_ret = endBuf6;
    g_A_ret = (void*)0x77770000;
    g_B_ret = obj;
    CGameScriptInterface_AddScreenTitleMessage(self, 0, a0, a1, (void*)0x1234);
    if (g_d_calls != 0) { printf("c6 null-target no-call\n"); fails++; }

    if (fails == 0) {
        printf("FSE2_0089e4c0_TEST PASS\n");
        return 0;
    }
    printf("FAILS=%d\n", fails);
    return 1;
}