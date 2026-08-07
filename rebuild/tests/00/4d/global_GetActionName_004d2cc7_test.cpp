#include <cstdio>
struct GAN004d2cc7 { void* m(void* s, int n); };
static void* gSelf; static void* gStr; static int gN;
void* GAN004d2cc7::m(void* s, int n){ gSelf=this; gStr=s; gN=n; return 0; }
void* __fastcall fn_004d2cc7(GAN004d2cc7* self){ self->m((void*)0x123a214, -1); return self; }
int main(){ GAN004d2cc7 x; void* r=fn_004d2cc7(&x);
 if(r!=&x||gSelf!=&x||gStr!=(void*)0x123a214||gN!=-1){ printf("004d2cc7_TEST FAIL\n"); return 1; }
 printf("004d2cc7_TEST PASS\n"); return 0; }