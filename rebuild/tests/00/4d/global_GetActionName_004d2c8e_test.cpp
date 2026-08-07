#include <cstdio>
struct GAN004d2c8e { void* m(void* s, int n); };
static void* gSelf; static void* gStr; static int gN;
void* GAN004d2c8e::m(void* s, int n){ gSelf=this; gStr=s; gN=n; return 0; }
void* __fastcall fn_004d2c8e(GAN004d2c8e* self){ self->m((void*)0x123a1cc, -1); return self; }
int main(){ GAN004d2c8e x; void* r=fn_004d2c8e(&x);
 if(r!=&x||gSelf!=&x||gStr!=(void*)0x123a1cc||gN!=-1){ printf("004d2c8e_TEST FAIL\n"); return 1; }
 printf("004d2c8e_TEST PASS\n"); return 0; }