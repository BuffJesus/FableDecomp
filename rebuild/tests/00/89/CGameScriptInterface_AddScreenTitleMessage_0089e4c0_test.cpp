#include <cstdio>

// Behavior test for the linked reconstruction of
// CGameScriptInterface::AddScreenTitleMessage @ 0x0089e4c0.

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

void* __fastcall ASTM_Helper_A(void* p_ecx);
void* __fastcall ASTM_Helper_B(void* p_ecx);

void __fastcall CGameScriptInterface_AddScreenTitleMessage(
    void* self_ecx,
    unsigned long,
    void* areaName,
    void* fadeTimeBits,
    void* volatile rawMessageFlag);

void* __fastcall ASTM_Helper_A(void* p_ecx) { g_chain_seen = p_ecx; return g_A_ret; }
void* __fastcall ASTM_Helper_B(void* p_ecx) { g_a_seen = p_ecx; return g_B_ret; }

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
    void* areaName = (void*)0xA000;
    void* fadeTimeBits = (void*)0x3F400000;

    int fails = 0;

    // Case 1: no interface owner returns after both lookup helpers.
    reset();
    g_A_ret = (void*)0x22220000;
    g_B_ret = 0;
    CGameScriptInterface_AddScreenTitleMessage(
        self, 0, areaName, fadeTimeBits, (void*)1);
    if (g_chain_seen != chainval) { printf("c1 chain\n"); fails++; }
    if (g_a_seen != g_A_ret)      { printf("c1 a\n"); fails++; }
    if (g_d_calls != 0)           { printf("c1 no-call\n"); fails++; }

    // Case 2: owner byte 0x91 bit 0 disables the operation.
    reset();
    for (int i=0;i<0x100;i++) obj[i]=0;
    obj[OFF_BYTE91] = 0x01;
    g_A_ret = (void*)0x33330000;
    g_B_ret = obj;
    CGameScriptInterface_AddScreenTitleMessage(
        self, 0, areaName, fadeTimeBits, (void*)1);
    if (g_d_calls != 0) { printf("c2 no-call\n"); fails++; }
    if (g_lb_this != 0) { printf("c2 no-map\n"); fails++; }

    // Case 3: with no interface map, a false public bool is also a null
    // fallback target.  This locks the retail raw-slot aliasing safely.
    reset();
    for (int i=0;i<0x100;i++) obj[i]=0;
    *(unsigned int*)(obj + OFF_FLAGS30) = 0x0000;
    g_A_ret = (void*)0x44440000;
    g_B_ret = obj;
    CGameScriptInterface_AddScreenTitleMessage(
        self, 0, areaName, fadeTimeBits, (void*)0);
    if (g_lb_this != 0) { printf("c3 no-map\n"); fails++; }
    if (g_d_calls != 0) { printf("c3 null-fallback\n"); fails++; }

    // Case 4: a valid lower-bound entry supplies the display.  The public bool
    // remains the third forwarded argument, followed by false and true.
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
    CGameScriptInterface_AddScreenTitleMessage(
        self, 0, areaName, fadeTimeBits, (void*)1);
    if (g_lb_this != (void*)(obj + OFF_MAP)) { printf("c4 map-this\n"); fails++; }
    if (g_lb_key != 0x8e)                    { printf("c4 key\n"); fails++; }
    if (g_d_calls != 1)                      { printf("c4 call\n"); fails++; }
    if (g_d_this != &targetObj)              { printf("c4 this\n"); fails++; }
    if (g_d_a0 != areaName || g_d_a1 != fadeTimeBits || g_d_a2 != (void*)1) {
        printf("c4 args\n");
        fails++;
    }
    if (g_d_z != 0 || g_d_one != 1)          { printf("c4 constants\n"); fails++; }

    // Case 5: an entry above key 0x8e falls back to the end node.
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
    CGameScriptInterface_AddScreenTitleMessage(
        self, 0, areaName, fadeTimeBits, (void*)0);
    if (g_d_calls != 1)         { printf("c5 call\n"); fails++; }
    if (g_d_this != &targetObj) { printf("c5 this (picked node instead of end)\n"); fails++; }
    if (g_d_a2 != 0)            { printf("c5 false-forward\n"); fails++; }

    // Case 6: an end node with no display produces no call.
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
    CGameScriptInterface_AddScreenTitleMessage(
        self, 0, areaName, fadeTimeBits, (void*)1);
    if (g_d_calls != 0) { printf("c6 null-target no-call\n"); fails++; }

    if (fails == 0) {
        printf("FSE2_0089e4c0_TEST PASS\n");
        return 0;
    }
    printf("FAILS=%d\n", fails);
    return 1;
}
