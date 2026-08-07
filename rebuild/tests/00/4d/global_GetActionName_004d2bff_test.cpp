#include <cstdio>
struct GAN004d2bff { void* m(void* s, int n); };
static void* gSelf; static void* gStr; static int gN;
void* GAN004d2bff::m(void* s, int n){ gSelf=this; gStr=s; gN=n; return 0; }
void* __fastcall fn_004d2bff(GAN004d2bff* self){ self->m((void*)0x123a13c, -1); return self; }
int main(){ GAN004d2bff x; void* r=fn_004d2bff(&x);
 if(r!=&x||gSelf!=&x||gStr!=(void*)0x123a13c||gN!=-1){ printf("004d2bff_TEST FAIL\n"); return 1; }
 printf("004d2bff_TEST PASS\n"); return 0; }