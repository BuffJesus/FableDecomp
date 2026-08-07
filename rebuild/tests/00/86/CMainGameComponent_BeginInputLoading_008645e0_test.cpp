#include <cstdio>
static int gArg; static void* gP;
int __stdcall h_008645e0(int arg, void* p){ gArg=arg; gP=p; return 3; }
int __fastcall fn_008645e0(void* self, int _edx, int arg){ return h_008645e0(arg, (void*)0x661600); }
int main(){ if(fn_008645e0(0,0,42)!=3||gArg!=42||gP!=(void*)0x661600){ printf("008645e0_TEST FAIL\n"); return 1; }
 printf("008645e0_TEST PASS\n"); return 0; }