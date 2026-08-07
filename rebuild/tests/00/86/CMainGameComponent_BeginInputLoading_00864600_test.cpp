#include <cstdio>
static int gArg; static void* gP;
int __stdcall h_00864600(int arg, void* p){ gArg=arg; gP=p; return 3; }
int __fastcall fn_00864600(void* self, int _edx, int arg){ return h_00864600(arg, (void*)0x643b3b); }
int main(){ if(fn_00864600(0,0,42)!=3||gArg!=42||gP!=(void*)0x643b3b){ printf("00864600_TEST FAIL\n"); return 1; }
 printf("00864600_TEST PASS\n"); return 0; }