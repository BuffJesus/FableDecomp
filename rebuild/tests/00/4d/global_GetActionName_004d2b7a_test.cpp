#include <cstdio>
struct GAN004d2b7a { void* m(void* s, int n); };
static void* gSelf; static void* gStr; static int gN;
void* GAN004d2b7a::m(void* s, int n){ gSelf=this; gStr=s; gN=n; return 0; }
void* __fastcall fn_004d2b7a(GAN004d2b7a* self){ self->m((void*)0x123a094, -1); return self; }
int main(){ GAN004d2b7a x; void* r=fn_004d2b7a(&x);
 if(r!=&x||gSelf!=&x||gStr!=(void*)0x123a094||gN!=-1){ printf("004d2b7a_TEST FAIL\n"); return 1; }
 printf("004d2b7a_TEST PASS\n"); return 0; }