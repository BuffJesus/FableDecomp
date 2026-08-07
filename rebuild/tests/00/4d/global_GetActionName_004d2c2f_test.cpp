#include <cstdio>
struct GAN004d2c2f { void* m(void* s, int n); };
static void* gSelf; static void* gStr; static int gN;
void* GAN004d2c2f::m(void* s, int n){ gSelf=this; gStr=s; gN=n; return 0; }
void* __fastcall fn_004d2c2f(GAN004d2c2f* self){ self->m((void*)0x123a154, -1); return self; }
int main(){ GAN004d2c2f x; void* r=fn_004d2c2f(&x);
 if(r!=&x||gSelf!=&x||gStr!=(void*)0x123a154||gN!=-1){ printf("004d2c2f_TEST FAIL\n"); return 1; }
 printf("004d2c2f_TEST PASS\n"); return 0; }