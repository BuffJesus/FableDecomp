#include <cstdio>

struct CPersistContext {
    int calls;
    void PersistA(int& r);
    void PersistB(int& r);
};

struct CVillagerInteractionsDef {
    char pad[0x3c];
    int fieldA;
    int fieldB;
    int fieldC;
};

static int g_seq[4];
static int g_n;

void CPersistContext::PersistA(int& r) { calls++; g_seq[g_n++] = 1; }
void CPersistContext::PersistB(int& r) { calls++; g_seq[g_n++] = 2; }

void __fastcall CVillagerInteractionsDef_Transfer(CVillagerInteractionsDef* self, void* edx, CPersistContext* ctx)
{
    ctx->PersistA(self->fieldA);
    ctx->PersistA(self->fieldB);
    ctx->PersistB(self->fieldC);
}

int main() {
    CVillagerInteractionsDef d;
    CPersistContext c; c.calls = 0; g_n = 0;
    CVillagerInteractionsDef_Transfer(&d, 0, &c);
    if (c.calls == 3 && g_seq[0]==1 && g_seq[1]==1 && g_seq[2]==2) {
        std::printf("CVillagerInteractionsDef_00456f27_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}