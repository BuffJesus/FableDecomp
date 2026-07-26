// Standalone behavioral test for CGameScriptInterface::GiveHeroItem (retail 0x00898bb0).
// Defines its own copy of the function plus recording stubs for every extern, then
// exercises each control-flow path. Prints exactly one pass line on success.
//
// Build (VC7.1):  cl /nologo /O2 /Oy /W3 test_00898bb0.cpp
#include <cstdio>

// ---- struct layout (mirrors the reconstruction) ----------------------------
struct HeroItemQuery;

struct HeroItemQueryVtbl {
    unsigned char pad_00[0x2c];
    void* (__fastcall* GetItemDef)(HeroItemQuery*);           // +0x2c
    unsigned char pad_30[0x12c - 0x2c - 4];
    unsigned char (__fastcall* ShouldGive)(HeroItemQuery*);   // +0x12c
};
struct HeroItemQuery {
    HeroItemQueryVtbl* vtbl;   // +0x0
};

struct HeroThing {
    unsigned char pad_00[0x38];
    unsigned long flags38;       // +0x38
    unsigned char pad_3c[0x44 - 0x38 - 4];
    void*         mapRoot;       // +0x44
    unsigned char pad_48[0x91 - 0x44 - 4];
    unsigned char flags91;       // +0x91
};

struct MapNode {
    long  key;       // +0x0
    void* value;     // +0x4
};

struct HeroMap {
    void*    pad_00;                               // +0x0
    void*    endNode;                              // +0x4
    MapNode* lower_bound(const long* key);         // __fastcall
};

struct GiveTarget {
    void Give(void* itemDef, int zero);            // __fastcall
};

// ---- recording state -------------------------------------------------------
static int   g_shouldGive_ret   = 1;
static void* g_getItemDef_ret    = 0;
static int   g_getItemDef_calls  = 0;

static void* g_helperA_arg = 0;
static void* g_helperA_ret = 0;
static void* g_helperB_arg = 0;
static HeroThing* g_helperB_ret = 0;

static const long* g_lb_keyPtr = 0;
static long        g_lb_keyVal = 0;
static void*       g_lb_this   = 0;
static MapNode*    g_lb_ret    = 0;

static void* g_give_this    = 0;
static void* g_give_itemDef = 0;
static int   g_give_zero    = -1;
static int   g_give_calls   = 0;

// ---- extern stubs ----------------------------------------------------------
void* __fastcall GiveHeroItem_HelperA(void* thisA) {
    g_helperA_arg = thisA;
    return g_helperA_ret;
}
HeroThing* __fastcall GiveHeroItem_HelperB(void* thisB) {
    g_helperB_arg = thisB;
    return g_helperB_ret;
}
MapNode* HeroMap::lower_bound(const long* key) {
    g_lb_this   = this;
    g_lb_keyPtr = key;
    g_lb_keyVal = *key;              // capture the 0xcb the caller wrote
    return g_lb_ret;
}
void GiveTarget::Give(void* itemDef, int zero) {
    g_give_this    = this;
    g_give_itemDef = itemDef;
    g_give_zero    = zero;
    ++g_give_calls;
}

static unsigned char __fastcall stub_ShouldGive(HeroItemQuery*) {
    return (unsigned char)g_shouldGive_ret;
}
static void* __fastcall stub_GetItemDef(HeroItemQuery*) {
    ++g_getItemDef_calls;
    return g_getItemDef_ret;
}

// ---- the reconstructed function (verbatim copy) ----------------------------
void __fastcall CGameScriptInterface_GiveHeroItem(void* thisPtr, unsigned long /*edx*/,
                                                  HeroItemQuery* query)
{
    if (!query->vtbl->ShouldGive(query))
        return;

    void* a = GiveHeroItem_HelperA(*(void**)((unsigned char*)thisPtr + 0x14));
    HeroThing* h = GiveHeroItem_HelperB(a);

    if (h == 0)
        return;

    if (h->flags91 & 0x1)
        return;

    void* target;

    if (h->flags38 & 0x0800) {
        long key = 0xcb;
        HeroMap* hdr = (HeroMap*)((unsigned char*)h + 0x44);
        MapNode* found = hdr->lower_bound(&key);

        MapNode* end = (MapNode*)hdr->endNode;

        if (found != end) {
            if (found->key > 0xcb)
                found = end;
        } else {
            found = end;
        }
        target = found->value;
    } else {
        target = *(HeroItemQuery* volatile*)&query;
    }

    void* itemDef = query->vtbl->GetItemDef(query);

    ((GiveTarget*)target)->Give(itemDef, 0);
}

// ---- harness ---------------------------------------------------------------
static void reset() {
    g_getItemDef_calls = 0; g_getItemDef_ret = 0;
    g_helperA_arg = g_helperA_ret = 0; g_helperB_arg = 0; g_helperB_ret = 0;
    g_lb_keyPtr = 0; g_lb_keyVal = 0; g_lb_this = 0; g_lb_ret = 0;
    g_give_this = 0; g_give_itemDef = 0; g_give_zero = -1; g_give_calls = 0;
}

int main() {
    int fails = 0;

    static HeroItemQueryVtbl vtbl;
    for (int i = 0; i < (int)sizeof(vtbl); ++i) ((unsigned char*)&vtbl)[i] = 0;
    vtbl.ShouldGive = stub_ShouldGive;
    vtbl.GetItemDef = stub_GetItemDef;

    HeroItemQuery query; query.vtbl = &vtbl;

    unsigned char thisObj[0x20];
    for (int i = 0; i < 0x20; ++i) thisObj[i] = 0;
    void* thisPlus14 = (void*)0xDEADBEEF;
    *(void**)(thisObj + 0x14) = thisPlus14;

    // Case 1: ShouldGive returns 0 -> early return, no Give
    reset();
    g_shouldGive_ret = 0;
    CGameScriptInterface_GiveHeroItem(thisObj, 0, &query);
    if (g_give_calls != 0)            { printf("C1 give called\n");        ++fails; }
    if (g_getItemDef_calls != 0)      { printf("C1 getItemDef called\n");  ++fails; }

    // Case 2: HelperB returns null -> early return
    reset();
    g_shouldGive_ret = 1;
    g_helperA_ret = (void*)0x1111;
    g_helperB_ret = 0;
    CGameScriptInterface_GiveHeroItem(thisObj, 0, &query);
    if (g_helperA_arg != thisPlus14)  { printf("C2 helperA arg wrong\n");  ++fails; }
    if (g_helperB_arg != (void*)0x1111){ printf("C2 helperB arg wrong\n"); ++fails; }
    if (g_give_calls != 0)            { printf("C2 give called\n");        ++fails; }

    // Case 3: flags91 bit0 set -> early return
    reset();
    HeroThing ht3;
    for (int i = 0; i < (int)sizeof(ht3); ++i) ((unsigned char*)&ht3)[i] = 0;
    ht3.flags91 = 0x1;
    g_helperA_ret = (void*)0x2222;
    g_helperB_ret = &ht3;
    CGameScriptInterface_GiveHeroItem(thisObj, 0, &query);
    if (g_give_calls != 0)            { printf("C3 give called\n");        ++fails; }

    // Case 4: no-map path (flags38 bit0x800 clear) -> target == query
    reset();
    HeroThing ht4;
    for (int i = 0; i < (int)sizeof(ht4); ++i) ((unsigned char*)&ht4)[i] = 0;
    ht4.flags38 = 0x0000;
    ht4.flags91 = 0x0;
    g_helperA_ret = (void*)0x3333;
    g_helperB_ret = &ht4;
    g_getItemDef_ret = (void*)0xABCD;
    CGameScriptInterface_GiveHeroItem(thisObj, 0, &query);
    if (g_give_calls != 1)            { printf("C4 give not called once\n"); ++fails; }
    if (g_give_this != (void*)&query) { printf("C4 target != query\n");      ++fails; }
    if (g_give_itemDef != (void*)0xABCD){ printf("C4 itemDef wrong\n");      ++fails; }
    if (g_give_zero != 0)             { printf("C4 zero arg wrong\n");       ++fails; }

    // Case 5: map path, found valid, key == 0xcb -> target == found->value
    reset();
    HeroThing ht5;
    for (int i = 0; i < (int)sizeof(ht5); ++i) ((unsigned char*)&ht5)[i] = 0;
    ht5.flags38 = 0x0800;
    ht5.flags91 = 0x0;
    void* endNodeSentinel = (void*)0x9999;
    *(void**)((unsigned char*)&ht5 + 0x44 + 4) = endNodeSentinel;
    MapNode found5; found5.key = 0xcb; found5.value = (void*)0x5555;
    g_lb_ret = &found5;
    g_helperA_ret = (void*)0x4444;
    g_helperB_ret = &ht5;
    g_getItemDef_ret = (void*)0xBEEF;
    CGameScriptInterface_GiveHeroItem(thisObj, 0, &query);
    if (g_lb_keyVal != 0xcb)          { printf("C5 key not 0xcb\n");         ++fails; }
    if (g_lb_this != (void*)((unsigned char*)&ht5 + 0x44)) { printf("C5 lb this wrong\n"); ++fails; }
    if (g_give_calls != 1)            { printf("C5 give not called once\n"); ++fails; }
    if (g_give_this != (void*)0x5555) { printf("C5 target != found value\n");++fails; }
    if (g_give_itemDef != (void*)0xBEEF){ printf("C5 itemDef wrong\n");      ++fails; }

    // Case 6: map path, found->key > 0xcb -> fall back to end node value
    reset();
    HeroThing ht6;
    for (int i = 0; i < (int)sizeof(ht6); ++i) ((unsigned char*)&ht6)[i] = 0;
    ht6.flags38 = 0x0800;
    ht6.flags91 = 0x0;
    MapNode endNode6; endNode6.key = 0x0; endNode6.value = (void*)0x7777;
    *(void**)((unsigned char*)&ht6 + 0x44 + 4) = &endNode6;
    MapNode found6; found6.key = 0x100; found6.value = (void*)0x6666;
    g_lb_ret = &found6;
    g_helperA_ret = (void*)0x4445;
    g_helperB_ret = &ht6;
    g_getItemDef_ret = (void*)0xCAFE;
    CGameScriptInterface_GiveHeroItem(thisObj, 0, &query);
    if (g_give_calls != 1)            { printf("C6 give not called once\n"); ++fails; }
    if (g_give_this != (void*)0x7777) { printf("C6 target != endnode value\n");++fails; }

    // Case 7: map path, found == end -> use end node value
    reset();
    HeroThing ht7;
    for (int i = 0; i < (int)sizeof(ht7); ++i) ((unsigned char*)&ht7)[i] = 0;
    ht7.flags38 = 0x0800;
    ht7.flags91 = 0x0;
    MapNode endNode7; endNode7.key = 0x0; endNode7.value = (void*)0x8888;
    *(void**)((unsigned char*)&ht7 + 0x44 + 4) = &endNode7;
    g_lb_ret = &endNode7;
    g_helperA_ret = (void*)0x4446;
    g_helperB_ret = &ht7;
    g_getItemDef_ret = (void*)0xF00D;
    CGameScriptInterface_GiveHeroItem(thisObj, 0, &query);
    if (g_give_calls != 1)            { printf("C7 give not called once\n"); ++fails; }
    if (g_give_this != (void*)0x8888) { printf("C7 target != endnode value\n");++fails; }

    if (fails == 0) {
        printf("FSE2_00898bb0_TEST PASS\n");
        return 0;
    }
    printf("FAILED: %d checks\n", fails);
    return 1;
}