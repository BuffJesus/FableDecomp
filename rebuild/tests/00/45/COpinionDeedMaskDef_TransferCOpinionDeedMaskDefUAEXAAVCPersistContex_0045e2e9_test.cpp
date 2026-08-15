#include <cstdio>

// Behaviour model: verify Transfer forwards to both callees with the
// correct sub-object pointers (+0x39 and +0x3c) and preserves ctx.

struct CtxRec { void* got[4]; int n; };

struct Ctx { CtxRec* rec; };
struct Def { char pad[0x40]; };

static void CalleeA(Ctx* ctx, void* obj) { ctx->rec->got[ctx->rec->n++] = obj; }
static void CalleeB(Ctx* ctx, void* obj) { ctx->rec->got[ctx->rec->n++] = obj; }

static void Transfer(Def* self, Ctx* ctx)
{
    char* base = reinterpret_cast<char*>(self);
    CalleeA(ctx, base + 0x39);
    CalleeB(ctx, base + 0x3c);
}

int main()
{
    Def d;
    CtxRec rec; rec.n = 0;
    Ctx ctx; ctx.rec = &rec;

    Transfer(&d, &ctx);

    char* base = reinterpret_cast<char*>(&d);
    bool ok = (rec.n == 2)
        && (rec.got[0] == base + 0x39)
        && (rec.got[1] == base + 0x3c);

    if (ok) printf("PARITY_OK\n");
    else printf("FAIL n=%d\n", rec.n);
    return 0;
}