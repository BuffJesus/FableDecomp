#include <cstdio>
struct Base00baa410 { void m(void* arg); };
static void* gBase; static void* gArg;
void Base00baa410::m(void* arg){ gBase=this; gArg=arg; }
char __fastcall fn_00baa410(char* self, int _edx, void* arg){ ((Base00baa410*)(self+0x74))->m(arg); return 1; }
int main(){ char blk[0x100]; int a; char r=fn_00baa410(blk,0,&a);
 if(r!=1||gBase!=blk+0x74||gArg!=&a){ printf("00baa410_TEST FAIL\n"); return 1; }
 printf("00baa410_TEST PASS\n"); return 0; }