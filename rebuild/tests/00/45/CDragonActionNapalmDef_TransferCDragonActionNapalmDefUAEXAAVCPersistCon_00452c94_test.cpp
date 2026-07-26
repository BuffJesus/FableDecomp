#include <cstdio>

struct Field { unsigned char b[4]; };

struct CPersistContext {
    int calls;
    void* last;
    void Xfer(Field* p) { calls++; last = p; }
};

struct CDragonActionNapalmDef {
    unsigned char _pad[0x28];
    Field f0;
    Field f1;
    Field f2;
    Field f3;
};

void __fastcall CDragonActionNapalmDef_Transfer(CDragonActionNapalmDef* self, int edx, CPersistContext* ctx)
{
    ctx->Xfer(&self->f0);
    ctx->Xfer(&self->f1);
    ctx->Xfer(&self->f2);
    ctx->Xfer(&self->f3);
}

int main()
{
    CDragonActionNapalmDef d;
    CPersistContext ctx;
    ctx.calls = 0;
    ctx.last = 0;
    CDragonActionNapalmDef_Transfer(&d, 0, &ctx);
    if (ctx.calls == 4 && ctx.last == (void*)&d.f3) {
        std::printf("CDragonActionNapalmDef_00452c94_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL calls=%d\n", ctx.calls);
    return 1;
}