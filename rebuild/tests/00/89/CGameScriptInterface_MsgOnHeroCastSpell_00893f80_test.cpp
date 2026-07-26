#include <cstdio>

/* Standalone byte-exact-behaviour test for
 * CGameScriptInterface::MsgOnHeroCastSpell @ 0x00893f80.
 * Defines its own copy of the function + recording stubs for the externs. */

struct GSI_MsgKind { unsigned long kind; void* a; void* b; };
struct GSI_MsgArg  { GSI_MsgKind* pInner; void** pB; };

struct MsgFrame { GSI_MsgKind inner; GSI_MsgArg arg; };

struct Router {
    void* Post(GSI_MsgArg* arg);
};

/* ---- recording stubs for the relocation-masked externs ---- */
static const void* g_getA_this;
static const void* g_getB_this;
static void*       g_getA_ret;
static void*       g_getB_ret;

static Router*      g_post_this;
static GSI_MsgArg*  g_post_arg;
static void*        g_post_ret;

void* __fastcall GetA(const void* thisptr) { g_getA_this = thisptr; return g_getA_ret; }
void* __fastcall GetB(const void* thisptr) { g_getB_this = thisptr; return g_getB_ret; }

void* Router::Post(GSI_MsgArg* arg)
{
    g_post_this = this;
    g_post_arg  = arg;
    return g_post_ret;
}

/* ---- the function under test (exact source that assembles byte-identical) ---- */
unsigned char __fastcall Func(void* thisptr, unsigned long edx, void** out)
{
    void* a = GetA(thisptr);
    void* b = GetB(thisptr);

    Router* router = *(Router**)((char*)(*(void**)((char*)thisptr + 0x4)) + 0x60);

    MsgFrame f;
    f.arg.pInner = &f.inner;
    f.inner.b = b;
    f.inner.a = a;
    f.inner.kind = 0x44;
    f.arg.pB = &f.inner.a;

    void* result = router->Post(&f.arg);
    if (result)
    {
        *out = *(void**)((char*)result + 0x3c);
        return 1;
    }
    return 0;
}

int main()
{
    /* Build a fake 'this': this+0x4 -> field4; field4+0x60 -> router. */
    Router routerObj;
    void* field4_block[0x20];
    field4_block[0x60/4] = &routerObj;   /* [+0x60] = router */

    void* this_block[8];
    this_block[0x4/4] = field4_block;     /* [+0x4] = field4 */

    int aMarker = 0xAA;
    int bMarker = 0xBB;
    g_getA_ret = &aMarker;
    g_getB_ret = &bMarker;

    /* --- Case 1: Post returns non-null -> return 1, *out = *(result+0x3c). --- */
    unsigned char resultBlock[0x40 + sizeof(void*)];
    void* payload = (void*)0xDEADBEEF;
    *(void**)(resultBlock + 0x3c) = payload;
    g_post_ret = resultBlock;

    void* outVal = 0;
    unsigned char rv = Func(this_block, 0, &outVal);

    if (rv != 1)                   { printf("FAIL rv!=1 (%d)\n", rv); return 1; }
    if (outVal != payload)         { printf("FAIL out mismatch\n");   return 2; }

    if (g_post_this != &routerObj) { printf("FAIL router this\n");    return 3; }
    GSI_MsgArg* pa = g_post_arg;
    if (pa->pInner->kind != 0x44)  { printf("FAIL kind\n");           return 4; }
    if (pa->pInner->a != &aMarker) { printf("FAIL inner.a\n");        return 5; }
    if (pa->pInner->b != &bMarker) { printf("FAIL inner.b\n");        return 6; }
    if (pa->pB != &pa->pInner->a)  { printf("FAIL pB alias\n");       return 7; }

    if (g_getA_this != this_block) { printf("FAIL GetA this\n");      return 8; }
    if (g_getB_this != this_block) { printf("FAIL GetB this\n");      return 9; }

    /* --- Case 2: Post returns null -> return 0, *out untouched. --- */
    g_post_ret = 0;
    void* untouched = (void*)0x12345678;
    outVal = untouched;
    rv = Func(this_block, 0, &outVal);
    if (rv != 0)                   { printf("FAIL rv!=0\n");          return 10; }
    if (outVal != untouched)       { printf("FAIL out clobbered\n");  return 11; }

    printf("FSE2_00893f80_TEST PASS\n");
    return 0;
}