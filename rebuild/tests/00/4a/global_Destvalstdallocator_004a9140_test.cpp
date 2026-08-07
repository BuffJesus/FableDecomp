#include <cstdio>
struct DV { char pad[4]; char f4; void m(); };
static int gN; static void* gLast; void DV::m(){ gN++; gLast=this; }
void __fastcall fn_004a9140(DV* self){ ((DV*)((char*)self+4))->m(); self->m(); }
int main(){ char blk[0x20]; DV* s=(DV*)blk; gN=0; fn_004a9140(s);
 if(gN!=2||gLast!=s){ printf("004a9140_TEST FAIL\n"); return 1; }
 printf("004a9140_TEST PASS\n"); return 0; }