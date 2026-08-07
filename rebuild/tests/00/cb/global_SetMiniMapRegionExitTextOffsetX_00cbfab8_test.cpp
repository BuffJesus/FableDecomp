#include <cstdio>
struct Glob { int* h(void* self); };
static int slot; static void* gG; static void* gSelf;
int* Glob::h(void* self){ gG=this; gSelf=self; return &slot; }
void __fastcall fn_00cbfab8(void* self, int arg){ int* p = ((Glob*)0x143e910)->h(self); *p = arg; }
int main(){ fn_00cbfab8(0, 55); if(slot!=55||gG!=(void*)0x143e910){ printf("00cbfab8_TEST FAIL\n"); return 1; }
 printf("00cbfab8_TEST PASS\n"); return 0; }