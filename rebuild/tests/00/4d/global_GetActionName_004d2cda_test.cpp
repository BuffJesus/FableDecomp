#include <cstdio>
struct GAN004d2cda { void* m(void* s, int n); };
static void* gSelf; static void* gStr; static int gN;
void* GAN004d2cda::m(void* s, int n){ gSelf=this; gStr=s; gN=n; return 0; }
void* __fastcall fn_004d2cda(GAN004d2cda* self){ self->m((void*)0x123a238, -1); return self; }
int main(){ GAN004d2cda x; void* r=fn_004d2cda(&x);
 if(r!=&x||gSelf!=&x||gStr!=(void*)0x123a238||gN!=-1){ printf("004d2cda_TEST FAIL\n"); return 1; }
 printf("004d2cda_TEST PASS\n"); return 0; }