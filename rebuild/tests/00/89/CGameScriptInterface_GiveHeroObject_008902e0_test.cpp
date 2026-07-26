/* Standalone behavioral test for CGameScriptInterface::GiveHeroObject @ 008902e0
 * Links ONLY this object. Defines its own copy of the function + recording stubs
 * for the externs, drives several control-flow paths, and prints exactly:
 *     FSE2_008902e0_TEST PASS
 * Returns 0 on success, nonzero on the first failed check.
 */
#include <cstdio>

/* ------- recording state for the stubs ------- */
static int   g_countRet;          /* value CountObjects returns */
static void* g_worldAccRet;       /* value GetWorldAccessor returns */
static void* g_heroRet;           /* value GetHero returns */
static unsigned char g_giveRet;   /* value Give returns */

static int   g_lastObjDefId;
static void* g_lastContainer;
static void* g_lastWorld;
static void* g_lastAccessor;
static int   g_ctorCalls, g_dtorCalls, g_giveCalls;
static int   g_lastGiveCount, g_lastGiveQty, g_lastGiveSuppress;
static void* g_lastGiveHero;
static const char* g_lastCtorStr;
static int   g_lastCtorLen;
static int   g_vfnCalls;
static void* g_lastVfnThis;

const char GHO_ItemName[] = "SILVER_KEY";
void* GHO_RefreshGate;

struct CCharString { void* rep; };

struct ObjContainer {
    int CountObjects(int objDefId) {
        g_lastContainer = this; g_lastObjDefId = objDefId; return g_countRet;
    }
};
struct World {
    void* GetWorldAccessor() { g_lastWorld = this; return g_worldAccRet; }
};
struct Accessor {
    void* GetHero() { g_lastAccessor = this; return g_heroRet; }
};
struct Hero {
    unsigned char Give(int count, int quantity, int suppressFlag, CCharString* name) {
        g_giveCalls++; g_lastGiveHero = this;
        g_lastGiveCount = count; g_lastGiveQty = quantity; g_lastGiveSuppress = suppressFlag;
        (void)name; return g_giveRet;
    }
};
struct Str {
    void ctor(const char* cstr, int len) { g_ctorCalls++; g_lastCtorStr = cstr; g_lastCtorLen = len; }
    void dtor() { g_dtorCalls++; }
};

/* ---- the function under test (identical to the byte-exact source) ---- */
void __fastcall CGameScriptInterface_GiveHeroObject(void* thisptr, unsigned long /*edx*/,
                                                    int objDefId, int quantity, int suppressFlag)
{
    void* self = thisptr;

    int count = ((ObjContainer*)(*(void**)((char*)self + 0x10)))->CountObjects(objDefId);
    if (count <= 0)
        return;

    void* accessor = ((World*)(*(void**)((char*)self + 0x14)))->GetWorldAccessor();
    void* hero = ((Accessor*)accessor)->GetHero();
    if (hero == 0)
        return;

    if (*(unsigned char*)((char*)hero + 0x91) & 1)
        return;

    CCharString name;
    ((Str*)&name)->ctor(GHO_ItemName, -1);

    ((Hero*)hero)->Give(count, quantity, suppressFlag, &name);

    ((Str*)&name)->dtor();

    if ((unsigned char)suppressFlag != 0)
        return;

    if (GHO_RefreshGate == 0)
        return;

    typedef void (__fastcall *GHO_VfnT)(void*);
    ((GHO_VfnT)(*(void***)self)[0x1c / 4])(self);
    ((GHO_VfnT)(*(void***)self)[0x1c / 4])(self);
}

/* ---- a fake "this" object with a vtable whose slot 0x1c records calls ---- */
static void __fastcall VfnRecord(void* thisp) { g_vfnCalls++; g_lastVfnThis = thisp; }

/* vtable large enough to hold slot 0x1c/4 = index 7 */
static void* g_vtbl[16];

struct FakeGSI {
    void*  vptr;      /* +0x00 : vtable pointer */
    char   pad[0x0c]; /* +0x04..0x0f */
    void*  container; /* +0x10 */
    void*  world;     /* +0x14 */
};

static ObjContainer g_container;
static World        g_world;
static Accessor     g_accessor;
static char         g_heroBuf[0x100];   /* hero object; +0x91 is the block flag */

static void resetHero(unsigned char flag91) {
    for (int i = 0; i < (int)sizeof(g_heroBuf); ++i) g_heroBuf[i] = 0;
    g_heroBuf[0x91] = (char)flag91;
}
static void resetCounters() {
    g_ctorCalls = g_dtorCalls = g_giveCalls = g_vfnCalls = 0;
    g_lastObjDefId = g_lastGiveCount = g_lastGiveQty = g_lastGiveSuppress = 0;
}

#define CHECK(c) do { if(!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); return 1; } } while(0)

int main(void)
{
    for (int i = 0; i < 16; ++i) g_vtbl[i] = (void*)&VfnRecord;

    FakeGSI gsi;
    gsi.vptr = g_vtbl;
    gsi.container = &g_container;
    gsi.world = &g_world;

    g_worldAccRet = &g_accessor;

    /* Path 1: count <= 0 -> early return, nothing else happens. */
    resetCounters(); resetHero(0);
    g_countRet = 0; g_heroRet = &g_heroBuf; g_giveRet = 0; GHO_RefreshGate = (void*)1;
    CGameScriptInterface_GiveHeroObject(&gsi, 0, 4242, 3, 0);
    CHECK(g_lastObjDefId == 4242);
    CHECK(g_lastContainer == &g_container);
    CHECK(g_giveCalls == 0 && g_ctorCalls == 0 && g_vfnCalls == 0);

    /* Path 2: hero == 0 -> return after world lookup. */
    resetCounters(); resetHero(0);
    g_countRet = 5; g_heroRet = 0;
    CGameScriptInterface_GiveHeroObject(&gsi, 0, 7, 2, 0);
    CHECK(g_lastWorld == &g_world && g_lastAccessor == &g_accessor);
    CHECK(g_giveCalls == 0 && g_vfnCalls == 0);

    /* Path 3: hero blocked (flag +0x91 bit0 set) -> return before give. */
    resetCounters(); resetHero(1);
    g_countRet = 5; g_heroRet = &g_heroBuf;
    CGameScriptInterface_GiveHeroObject(&gsi, 0, 7, 2, 0);
    CHECK(g_giveCalls == 0 && g_ctorCalls == 0 && g_vfnCalls == 0);

    /* Path 4: full give, suppressFlag != 0 -> give happens, NO ui refresh. */
    resetCounters(); resetHero(0);
    g_countRet = 9; g_heroRet = &g_heroBuf; g_giveRet = 0; GHO_RefreshGate = (void*)1;
    CGameScriptInterface_GiveHeroObject(&gsi, 0, 111, 22, 1 /*suppress*/);
    CHECK(g_ctorCalls == 1 && g_dtorCalls == 1 && g_giveCalls == 1);
    CHECK(g_lastGiveCount == 9 && g_lastGiveQty == 22 && g_lastGiveSuppress == 1);
    CHECK(g_lastGiveHero == (void*)&g_heroBuf);
    CHECK(g_lastCtorLen == -1 && g_lastCtorStr == GHO_ItemName);
    CHECK(g_vfnCalls == 0);            /* suppressed */

    /* Path 5: full give, suppress == 0, RefreshGate != 0 -> two vtable calls. */
    resetCounters(); resetHero(0);
    g_countRet = 3; g_heroRet = &g_heroBuf; g_giveRet = 0; GHO_RefreshGate = (void*)1;
    CGameScriptInterface_GiveHeroObject(&gsi, 0, 5, 6, 0);
    CHECK(g_giveCalls == 1 && g_lastGiveSuppress == 0);
    CHECK(g_vfnCalls == 2);
    CHECK(g_lastVfnThis == &gsi);

    /* Path 6: suppress == 0 but RefreshGate == 0 -> give but NO ui refresh. */
    resetCounters(); resetHero(0);
    g_countRet = 3; g_heroRet = &g_heroBuf; g_giveRet = 0; GHO_RefreshGate = 0;
    CGameScriptInterface_GiveHeroObject(&gsi, 0, 5, 6, 0);
    CHECK(g_giveCalls == 1);
    CHECK(g_vfnCalls == 0);

    printf("FSE2_008902e0_TEST PASS\n");
    return 0;
}