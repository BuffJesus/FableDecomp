#include <stdio.h>
struct Self { char pad[0x10]; unsigned short r; };
unsigned short g_ret = 0;
void* g_arg = 0;
unsigned short __stdcall TreeInsert_A_helper(void* arg){ g_arg = arg; return g_ret; }
void __fastcall TreeInsert_A(Self* self, int edx, void* arg);
int main(){
    Self s; s.r = 0xFFFF;
    g_ret = 0x1234; int x = 7;
    TreeInsert_A(&s, 0, &x);
    if (s.r != 0x1234) { printf("FAIL r\n"); return 1; }
    if (g_arg != &x) { printf("FAIL arg\n"); return 1; }
    g_ret = 0xABCD;
    TreeInsert_A(&s, 0, 0);
    if (s.r != 0xABCD) { printf("FAIL r2\n"); return 1; }
    if (g_arg != 0) { printf("FAIL arg2\n"); return 1; }
    printf("OK_00c1bb80\n");
    return 0;
}