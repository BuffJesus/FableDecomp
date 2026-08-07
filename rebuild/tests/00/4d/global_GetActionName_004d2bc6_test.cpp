#include <cstdio>
struct GAN004d2bc6 { void* m(void* s, int n); };
static void* gSelf; static void* gStr; static int gN;
void* GAN004d2bc6::m(void* s, int n){ gSelf=this; gStr=s; gN=n; return 0; }
void* __fastcall fn_004d2bc6(GAN004d2bc6* self){ self->m((void*)0x123a0f4, -1); return self; }
int main(){ GAN004d2bc6 x; void* r=fn_004d2bc6(&x);
 if(r!=&x||gSelf!=&x||gStr!=(void*)0x123a0f4||gN!=-1){ printf("004d2bc6_TEST FAIL\n"); return 1; }
 printf("004d2bc6_TEST PASS\n"); return 0; }