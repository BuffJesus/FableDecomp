#include <cstdio>
static int g_calls=0;
static int g_seq[8]; static int g_n=0;
struct CMessageEventDef {
    char pad[0x3c];
    int f3c;
    int f40;
    int f44;
    int f48;
    int f4c;
};
struct CPersistContext {
    void TransferA(void* p){ g_calls++; g_seq[g_n++]=1; }
    void TransferB(void* p){ g_calls++; g_seq[g_n++]=2; }
    void TransferC(void* p){ g_calls++; g_seq[g_n++]=3; }
};

void __fastcall CMessageEventDef_Transfer(CMessageEventDef* self, void* edx, CPersistContext* ctx) {
    ctx->TransferA(&self->f3c);
    ctx->TransferA(&self->f40);
    ctx->TransferA(&self->f44);
    ctx->TransferB(&self->f4c);
    ctx->TransferC(&self->f48);
}

int main(){
    CMessageEventDef d;
    CPersistContext c;
    CMessageEventDef_Transfer(&d, 0, &c);
    int ok = (g_calls==5) && g_seq[0]==1 && g_seq[1]==1 && g_seq[2]==1 && g_seq[3]==2 && g_seq[4]==3;
    if(ok){ std::printf("CMessageEventDef_004556ce_TEST PASS\n"); return 0; }
    std::printf("FAIL calls=%d\n", g_calls); return 1;
}