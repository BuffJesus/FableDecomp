#include <cstdio>
struct GAN004d2b8d { void* m(void* s, int n); };
static void* gSelf; static void* gStr; static int gN;
void* GAN004d2b8d::m(void* s, int n){ gSelf=this; gStr=s; gN=n; return 0; }
void* __fastcall fn_004d2b8d(GAN004d2b8d* self){ self->m((void*)0x123a0a8, -1); return self; }
int main(){ GAN004d2b8d x; void* r=fn_004d2b8d(&x);
 if(r!=&x||gSelf!=&x||gStr!=(void*)0x123a0a8||gN!=-1){ printf("004d2b8d_TEST FAIL\n"); return 1; }
 printf("004d2b8d_TEST PASS\n"); return 0; }