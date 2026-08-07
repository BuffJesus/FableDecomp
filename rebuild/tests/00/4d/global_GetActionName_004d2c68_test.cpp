#include <cstdio>
struct GAN004d2c68 { void* m(void* s, int n); };
static void* gSelf; static void* gStr; static int gN;
void* GAN004d2c68::m(void* s, int n){ gSelf=this; gStr=s; gN=n; return 0; }
void* __fastcall fn_004d2c68(GAN004d2c68* self){ self->m((void*)0x123a198, -1); return self; }
int main(){ GAN004d2c68 x; void* r=fn_004d2c68(&x);
 if(r!=&x||gSelf!=&x||gStr!=(void*)0x123a198||gN!=-1){ printf("004d2c68_TEST FAIL\n"); return 1; }
 printf("004d2c68_TEST PASS\n"); return 0; }