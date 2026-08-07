#include <cstdio>
struct GAN004d2c7b { void* m(void* s, int n); };
static void* gSelf; static void* gStr; static int gN;
void* GAN004d2c7b::m(void* s, int n){ gSelf=this; gStr=s; gN=n; return 0; }
void* __fastcall fn_004d2c7b(GAN004d2c7b* self){ self->m((void*)0x123a1bc, -1); return self; }
int main(){ GAN004d2c7b x; void* r=fn_004d2c7b(&x);
 if(r!=&x||gSelf!=&x||gStr!=(void*)0x123a1bc||gN!=-1){ printf("004d2c7b_TEST FAIL\n"); return 1; }
 printf("004d2c7b_TEST PASS\n"); return 0; }