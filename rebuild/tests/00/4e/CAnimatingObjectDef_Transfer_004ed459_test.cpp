#include <cstdio>

struct SubObject;

static int g_step = 0;
static void* g_ctx_self = 0;
static void* g_ctx_arg = 0;
static void* g_sub_self = 0;
static int g_ctx_order = -1;
static int g_sub_order = -1;

struct SubObject {
    int dummy;
    void Finalise();
};

struct CPersistContext {
    void Persist(SubObject* p);
};

void SubObject::Finalise() {
    g_sub_self = this;
    g_sub_order = g_step++;
}
void CPersistContext::Persist(SubObject* p) {
    g_ctx_self = this;
    g_ctx_arg = p;
    g_ctx_order = g_step++;
}

struct CAnimatingObjectDef {
    char pad[0x28];
    SubObject sub;
};

void __fastcall Transfer(CAnimatingObjectDef* self, int, CPersistContext& ctx) {
    SubObject* s = &self->sub;
    ctx.Persist(s);
    s->Finalise();
}

int main() {
    CAnimatingObjectDef obj;
    CPersistContext ctx;
    Transfer(&obj, 0, ctx);
    SubObject* expectedSub = &obj.sub;
    bool ok = (g_ctx_self == &ctx)
           && (g_ctx_arg == expectedSub)
           && (g_sub_self == expectedSub)
           && (g_ctx_order == 0)
           && (g_sub_order == 1);
    if (ok) printf("004ed459_TEST PASS\n");
    else printf("FAIL step=%d\n", g_step);
    return 0;
}