#include <cstdio>
struct Base00b98d50 { void m(void* arg); };
static void* gBase; static void* gArg;
void Base00b98d50::m(void* arg){ gBase=this; gArg=arg; }
char __fastcall fn_00b98d50(char* self, int _edx, void* arg){ ((Base00b98d50*)(self+0x58))->m(arg); return 1; }
int main(){ char blk[0x100]; int a; char r=fn_00b98d50(blk,0,&a);
 if(r!=1||gBase!=blk+0x58||gArg!=&a){ printf("00b98d50_TEST FAIL\n"); return 1; }
 printf("00b98d50_TEST PASS\n"); return 0; }