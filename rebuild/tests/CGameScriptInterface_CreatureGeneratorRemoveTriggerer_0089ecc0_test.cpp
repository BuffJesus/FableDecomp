#include <cstdio>

struct Node { int key; Node* left; };

static int  g_consumed;
static void* g_consumedSelf;

struct BObj {
    unsigned char pad[0x6c];
    unsigned char flag6c;
    void consume(BObj* b);
};
struct MapT {
    Node* dummy0;
    Node* end;
    Node* find(int* key);
};
struct AObj {
    unsigned char pad0[0x24];
    unsigned int field24;
    unsigned char pad2[0x1c];
    MapT map44;
};
struct Host { void** vt; };

static Node g_end;
static Node g_hit;
static Node g_valNode;

Node* MapT::find(int* key) { (void)key; return &g_hit; }
void  BObj::consume(BObj* b) { (void)b; g_consumed = 1; g_consumedSelf = this; }

typedef AObj* (__fastcall *GetA_t)(void*);
typedef BObj* (__fastcall *GetB_t)(void*);

static AObj g_a;
static BObj g_b;
static AObj* __fastcall getA(void* self) { (void)self; return &g_a; }
static BObj* __fastcall getB(void* self) { (void)self; return &g_b; }

void __stdcall CGameScriptInterface_CreatureGeneratorRemoveTriggerer(Host* p0, Host* p1)
{
    AObj* a = ((GetA_t)(p0->vt[0x0b]))(p0);
    BObj* b = ((GetB_t)(p1->vt[0x0b]))(p1);

    if (a->field24 & 0x20000000)
    {
        int local = 0x3d;
        MapT* m = &a->map44;
        Node* it = m->find(&local);
        Node* end = m->end;
        Node* node = (it == end || it->key > 0x3d) ? end : it;
        BObj* volatile* pv = (BObj* volatile*)((char*)node + 4);
        BObj* val = *pv;
        if (b->flag6c & 8)
            val->consume(b);
    }
}

int main()
{
    void* vtA[16]; void* vtB[16];
    vtA[0x0b] = (void*)(GetA_t)getA;
    vtB[0x0b] = (void*)(GetB_t)getB;
    Host h0; h0.vt = vtA;
    Host h1; h1.vt = vtB;

    g_a.field24 = 0x20000000;
    g_a.map44.end = &g_end;
    g_hit.key = 0x10;
    g_hit.left = &g_valNode; // it->[4] == left => value pointer
    g_b.flag6c = 0x08;

    g_consumed = 0; g_consumedSelf = 0;
    CGameScriptInterface_CreatureGeneratorRemoveTriggerer(&h0, &h1);
    if (g_consumed != 1) { std::printf("FAIL: not consumed\n"); return 1; }
    if (g_consumedSelf != (void*)&g_valNode) { std::printf("FAIL: wrong self\n"); return 1; }

    // key > 0x3d -> forced to end; end->left used as val
    g_end.left = 0;
    g_hit.key = 0x50;
    g_consumed = 0; g_consumedSelf = 0;
    CGameScriptInterface_CreatureGeneratorRemoveTriggerer(&h0, &h1);
    if (g_consumed != 1) { std::printf("FAIL: case2 not consumed\n"); return 1; }
    if (g_consumedSelf != (void*)0) { std::printf("FAIL: case2 wrong self\n"); return 1; }

    // flag24 clear -> no-op
    g_a.field24 = 0;
    g_consumed = 0;
    CGameScriptInterface_CreatureGeneratorRemoveTriggerer(&h0, &h1);
    if (g_consumed != 0) { std::printf("FAIL: case3 consumed\n"); return 1; }

    std::printf("CGameScriptInterface_0089ecc0_TEST PASS\n");
    return 0;
}