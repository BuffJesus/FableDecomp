#include <cstdio>

struct CPersistContext { int log[8]; int n; };
struct CPlayerDef { char pad3c[0x3c]; char m3c[4]; char m40[4]; };

static void CtxTransferA(CPersistContext* ctx, void* member) { ctx->log[ctx->n++] = 0x3c; (void)member; }
static void CtxTransferB(CPersistContext* ctx, void* member) { ctx->log[ctx->n++] = 0x40; (void)member; }

static void Transfer(CPlayerDef* self, CPersistContext* ctx)
{
    CtxTransferA(ctx, &self->m3c);
    CtxTransferB(ctx, &self->m40);
}

int main()
{
    CPlayerDef d; CPersistContext c; c.n = 0;
    Transfer(&d, &c);
    if (c.n == 2 && c.log[0] == 0x3c && c.log[1] == 0x40)
        printf("PARITY_OK\n");
    else
        printf("FAIL\n");
    return 0;
}