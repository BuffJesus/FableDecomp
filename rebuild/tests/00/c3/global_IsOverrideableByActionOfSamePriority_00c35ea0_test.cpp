#include <cstdio>
struct OV { void** vtbl; };
static char gRet; static char __fastcall slot(OV* s){ return gRet; }
typedef char (__fastcall *pfo)(OV*);
int __fastcall fn_00c35ea0(OV* self){ char b=((pfo)self->vtbl[1])(self); return b ? 0 : (int)self; }
int main(){ static void* vt[4]; vt[1]=(void*)&slot; OV o; o.vtbl=vt;
 gRet=0; if(fn_00c35ea0(&o)!=(int)&o){ printf("00c35ea0_TEST FAIL\n"); return 1; }
 gRet=1; if(fn_00c35ea0(&o)!=0){ printf("00c35ea0_TEST FAIL\n"); return 1; }
 printf("00c35ea0_TEST PASS\n"); return 0; }