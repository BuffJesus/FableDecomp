#include <cstdio>
struct GAN004d2ced { void* m(void* s, int n); };
static void* gSelf; static void* gStr; static int gN;
void* GAN004d2ced::m(void* s, int n){ gSelf=this; gStr=s; gN=n; return 0; }
void* __fastcall fn_004d2ced(GAN004d2ced* self){ self->m((void*)0x123a250, -1); return self; }
int main(){ GAN004d2ced x; void* r=fn_004d2ced(&x);
 if(r!=&x||gSelf!=&x||gStr!=(void*)0x123a250||gN!=-1){ printf("004d2ced_TEST FAIL\n"); return 1; }
 printf("004d2ced_TEST PASS\n"); return 0; }