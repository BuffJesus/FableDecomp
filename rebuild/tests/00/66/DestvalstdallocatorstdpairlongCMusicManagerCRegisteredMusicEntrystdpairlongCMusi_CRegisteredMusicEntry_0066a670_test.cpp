#include <cstdio>

struct Sub { char b; };
struct Owner { char pad[0x114]; Sub sub; int flag; };

static int g_sub_called = 0;
static int g_tail_called = 0;
static Sub* g_sub_arg = 0;
static Owner* g_tail_arg = 0;

void __fastcall Sub_Method(Sub* s) { g_sub_called++; g_sub_arg = s; }
void __fastcall Owner_Tail(Owner* o) { g_tail_called++; g_tail_arg = o; }

void __fastcall DestVal_Music_C(Owner* self)
{
    Sub_Method((Sub*)((char*)self + 0x114));
    Owner_Tail(self);
}

int main()
{
    Owner o;
    DestVal_Music_C(&o);
    if (g_sub_called != 1) { printf("FAIL sub_called=%d\n", g_sub_called); return 1; }
    if (g_tail_called != 1) { printf("FAIL tail_called=%d\n", g_tail_called); return 1; }
    if (g_sub_arg != (Sub*)((char*)&o + 0x114)) { printf("FAIL sub_arg\n"); return 1; }
    if (g_tail_arg != &o) { printf("FAIL tail_arg\n"); return 1; }
    printf("OK_0066a670\n");
    return 0;
}