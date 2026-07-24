// Standalone test for CGameScriptInterface::EntitySetAlwaysBlockAttacksFromThing @ 0x0089dff0
// Defines its own copy of the target function + recording stubs, exercises it,
// prints exactly one pass line on success.
#include <cstdio>

typedef unsigned char  u8;
typedef unsigned long  u32;

struct BlockSet { void* _node; void* _end; };
struct CThing {
    char _p0[0x24]; u32 flags; char _p1[0x44-0x28];
    BlockSet blockSet; char _p2[0x91-0x4C]; u8 guardFlag;
};
struct CEntityWrap;
typedef CThing* (__fastcall *ResolveFn)(CEntityWrap*);

struct BlockSet2 { void* _node; void* _end; void* LowerBound(const int* key); };
struct Mapped    { void Add(CThing* thing); void Remove(CThing* thing); };

// --- recording state for the externs -----------------------------------------
static const int* g_lb_key;
static void*      g_lb_this;
static void*      g_lb_ret;
static void*      g_add_this;  static CThing* g_add_arg;  static int g_add_calls;
static void*      g_rem_this;  static CThing* g_rem_arg;  static int g_rem_calls;

void* BlockSet2::LowerBound(const int* key)
{ g_lb_this = this; g_lb_key = key; return g_lb_ret; }
void Mapped::Add(CThing* t)    { g_add_this = this; g_add_arg = t; ++g_add_calls; }
void Mapped::Remove(CThing* t) { g_rem_this = this; g_rem_arg = t; ++g_rem_calls; }

// --- function under test (byte-exact source) ---------------------------------
void __fastcall F(void* self, void* edx, CEntityWrap* entityA, CEntityWrap* entityB, u8 alwaysBlock)
{
    CThing* a = (*(ResolveFn**)entityA)[0x2c/4](entityA);
    CThing* b = (*(ResolveFn**)entityB)[0x2c/4](entityB);
    if (!a || (1 & a->guardFlag) || !b || (b->guardFlag & 1) || !(a->flags & 0x10000)) return;
    int key = 0x30;
    BlockSet2* bs = (BlockSet2*)((char*)a + 0x44);
    void* entry = bs->LowerBound(&key);
    void* end   = bs->_end;
    void* sel;
    if (entry == end) sel = end;
    else if (*(int*)entry <= 0x30) sel = entry;
    else sel = end;
    Mapped* mapped = *(Mapped**)((char*)sel + 4);
    if (alwaysBlock) mapped->Add(b); else mapped->Remove(b);
}

// --- fake entity: first dword is a vtable ptr; slot 0x2c returns per-entity target
struct Wrapper { void* vptr; CThing* target; };
static void* g_shared_vtbl[16];
static CThing* __fastcall WResolve(CEntityWrap* e) { return ((Wrapper*)e)->target; }
static void makeWrapper(Wrapper* w, CThing* target)
{ w->vptr = (void*)g_shared_vtbl; w->target = target; }

static void resetRec()
{
    g_lb_this=0; g_lb_key=0; g_lb_ret=0;
    g_add_this=0; g_add_arg=0; g_add_calls=0;
    g_rem_this=0; g_rem_arg=0; g_rem_calls=0;
}
static int fail(const char* m){ std::printf("FAIL: %s\n", m); return 1; }

int main()
{
    for (int i = 0; i < 16; ++i) g_shared_vtbl[i] = 0;
    g_shared_vtbl[0x2c/4] = (void*)&WResolve;

    static CThing thA, thB;
    struct Node { int keyval; Mapped* mapped; };
    static Node node;
    static Mapped theMapped;
    Wrapper wA, wB;

    // S1: guards pass, alwaysBlock=1, entry!=end, key<=0x30 -> Add on node.mapped
    {
        resetRec();
        thA.flags = 0x10000; thA.guardFlag = 0; thB.guardFlag = 0;
        node.keyval = 0x30; node.mapped = &theMapped;
        g_lb_ret = &node;
        thA.blockSet._end = (void*)0xDEAD0004;
        makeWrapper(&wA, &thA); makeWrapper(&wB, &thB);
        F(0, 0, (CEntityWrap*)&wA, (CEntityWrap*)&wB, 1);
        if (g_add_calls != 1) return fail("S1: Add not called once");
        if (g_rem_calls != 0) return fail("S1: Remove wrongly called");
        if (g_add_arg != &thB) return fail("S1: Add arg != b");
        if (g_add_this != &theMapped) return fail("S1: Add this != node.mapped");
        if (g_lb_this != (void*)((char*)&thA + 0x44)) return fail("S1: lb this");
        if (*g_lb_key != 0x30) return fail("S1: lb key != 0x30");
    }
    // S2: alwaysBlock=0 -> Remove
    {
        resetRec();
        node.keyval = 0x10; node.mapped = &theMapped;
        g_lb_ret = &node;
        thA.blockSet._end = (void*)0xBEEF0008;
        makeWrapper(&wA, &thA); makeWrapper(&wB, &thB);
        F(0, 0, (CEntityWrap*)&wA, (CEntityWrap*)&wB, 0);
        if (g_rem_calls != 1) return fail("S2: Remove not called once");
        if (g_add_calls != 0) return fail("S2: Add wrongly called");
        if (g_rem_arg != &thB) return fail("S2: Remove arg != b");
        if (g_rem_this != &theMapped) return fail("S2: Remove this");
    }
    // S3: entry == end -> sel=end; mapped from end+4
    {
        resetRec();
        static Node endNode; endNode.keyval = 0x99; endNode.mapped = &theMapped;
        g_lb_ret = &endNode;
        thA.blockSet._end = &endNode;
        thA.flags = 0x10000; thA.guardFlag = 0; thB.guardFlag = 0;
        makeWrapper(&wA, &thA); makeWrapper(&wB, &thB);
        F(0, 0, (CEntityWrap*)&wA, (CEntityWrap*)&wB, 1);
        if (g_add_calls != 1) return fail("S3: Add not called");
        if (g_add_this != &theMapped) return fail("S3: mapped from end+4");
    }
    // S4: entry key > 0x30 -> sel = end
    {
        resetRec();
        static Node hiNode; hiNode.keyval = 0x31; hiNode.mapped = (Mapped*)0x1111;
        static Node endN;   endN.keyval  = 0x00; endN.mapped   = &theMapped;
        g_lb_ret = &hiNode;
        thA.blockSet._end = &endN;
        makeWrapper(&wA, &thA); makeWrapper(&wB, &thB);
        F(0, 0, (CEntityWrap*)&wA, (CEntityWrap*)&wB, 1);
        if (g_add_calls != 1) return fail("S4: Add not called");
        if (g_add_this != &theMapped) return fail("S4: sel should be end");
    }
    // S5: guard rejections -> no calls
    {
        resetRec();
        g_lb_ret = &node;
        Wrapper wNull; makeWrapper(&wNull, 0);
        makeWrapper(&wB, &thB);
        F(0, 0, (CEntityWrap*)&wNull, (CEntityWrap*)&wB, 1);
        if (g_add_calls || g_rem_calls) return fail("S5a: a==0");
        resetRec();
        thA.flags = 0x10000; thA.guardFlag = 1; thB.guardFlag = 0;
        makeWrapper(&wA, &thA);
        F(0, 0, (CEntityWrap*)&wA, (CEntityWrap*)&wB, 1);
        if (g_add_calls || g_rem_calls) return fail("S5b: a guard");
        resetRec();
        thA.guardFlag = 0; thB.guardFlag = 1; makeWrapper(&wB, &thB);
        F(0, 0, (CEntityWrap*)&wA, (CEntityWrap*)&wB, 1);
        if (g_add_calls || g_rem_calls) return fail("S5c: b guard");
        resetRec();
        thB.guardFlag = 0; thA.flags = 0;
        F(0, 0, (CEntityWrap*)&wA, (CEntityWrap*)&wB, 1);
        if (g_add_calls || g_rem_calls) return fail("S5d: flags clear");
    }

    std::printf("FSE2_0089dff0_TEST PASS\n");
    return 0;
}