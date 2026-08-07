#include <cstdio>
struct GAN004d2a91 { void* m(void* s, int n); };
static void* gSelf; static void* gStr; static int gN;
void* GAN004d2a91::m(void* s, int n){ gSelf=this; gStr=s; gN=n; return 0; }
void* __fastcall fn_004d2a91(GAN004d2a91* self){ self->m((void*)0x1239fc0, -1); return self; }
int main(){ GAN004d2a91 x; void* r=fn_004d2a91(&x);
 if(r!=&x||gSelf!=&x||gStr!=(void*)0x1239fc0||gN!=-1){ printf("004d2a91_TEST FAIL\n"); return 1; }
 printf("004d2a91_TEST PASS\n"); return 0; }