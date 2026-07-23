#include <cstdio>

struct CSmashableDef {
    char pad[0x25];
    unsigned char fieldA;   // +0x25
    char gap1[0x28-0x26];
    unsigned char fieldB;   // +0x28
    char gap2[0x2c-0x29];
    int fieldC;             // +0x2c
};

static unsigned char* g_a; static unsigned char* g_b; static int* g_c;

struct CPersistContext {
    int calls;
    void PersistA(unsigned char* p){ calls++; g_a=p; }
    void PersistB(unsigned char* p){ calls++; g_b=p; }
    void PersistC(int* p){ calls++; g_c=p; }
};

void __fastcall CSmashableDef_Transfer(CSmashableDef* self, int edx, CPersistContext* ctx)
{
    ctx->PersistA(&self->fieldA);
    ctx->PersistB(&self->fieldB);
    ctx->PersistC(&self->fieldC);
}

int main(){
    CSmashableDef d; CPersistContext c; c.calls=0;
    CSmashableDef_Transfer(&d, 0, &c);
    if (c.calls==3 && g_a==&d.fieldA && g_b==&d.fieldB && g_c==&d.fieldC){
        std::printf("CSmashableDef_004522c6_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}