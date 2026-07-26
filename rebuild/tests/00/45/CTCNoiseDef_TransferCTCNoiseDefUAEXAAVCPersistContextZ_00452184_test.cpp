#include <cstdio>

struct CPersistContext {
    int calls;
    void* last;
    void TransferA(int* v) { calls++; last = v; }
    void TransferB(int* v) { calls++; last = v; }
};

struct CTCNoiseDef {
    char pad[0x28];
    int a;
    int b;
    int c;
};

void __fastcall CTCNoiseDef_Transfer(CTCNoiseDef* self, void* edx, CPersistContext* ctx)
{
    ctx->TransferA(&self->a);
    ctx->TransferB(&self->b);
    ctx->TransferB(&self->c);
}

int main() {
    CTCNoiseDef def;
    CPersistContext ctx;
    ctx.calls = 0; ctx.last = 0;
    CTCNoiseDef_Transfer(&def, 0, &ctx);
    if (ctx.calls == 3 && ctx.last == (void*)&def.c) {
        std::printf("CTCNoiseDef_00452184_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL calls=%d\n", ctx.calls);
    return 1;
}