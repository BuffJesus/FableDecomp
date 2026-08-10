#include <stdio.h>
struct Src {
    char pad0[0x71];
    unsigned char b71;
    char pad72[2];
    int  d74;
};
struct CShaderBankStateBlock {
    char pad0[8];
    unsigned char b8;
    char pad9[0x12f8 - 9];
    int  d12f8;
};
void __fastcall Set(CShaderBankStateBlock* self, int, void* srcv)
{
    Src* src = (Src*)srcv;
    self->b8    = src->b71;
    self->d12f8 = src->d74;
}
int main(){
    Src s; s.b71 = 0xAB; s.d74 = 0x11223344;
    CShaderBankStateBlock blk; blk.b8 = 0; blk.d12f8 = 0;
    Set(&blk, 0, &s);
    int ok = (blk.b8 == 0xAB) && (blk.d12f8 == 0x11223344);
    Src s2; s2.b71 = 0; s2.d74 = 0;
    Set(&blk, 0, &s2);
    ok = ok && (blk.b8 == 0) && (blk.d12f8 == 0);
    if(ok) printf("OK_00b50c60\n"); else printf("FAIL\n");
    return ok?0:1;
}