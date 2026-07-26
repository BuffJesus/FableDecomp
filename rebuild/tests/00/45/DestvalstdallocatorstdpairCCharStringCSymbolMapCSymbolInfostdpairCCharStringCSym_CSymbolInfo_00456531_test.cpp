
#include <cstdio>

struct Sub { char pad[32]; };
struct Owner { char pad[0x50]; };

static int g_sub_calls = 0;
static int g_base_calls = 0;
static char* g_last_sub[2];
static char* g_last_base = 0;

extern "C" void __fastcall Sub_dtor(Sub* s)
{
    if (g_sub_calls < 2) g_last_sub[g_sub_calls] = (char*)s;
    g_sub_calls++;
}
extern "C" void __fastcall Owner_base_dtor(Owner* o)
{
    g_base_calls++;
    g_last_base = (char*)o;
}

void __fastcall _Dest_val(Owner* self)
{
    Sub_dtor((Sub*)((char*)self + 0x48));
    Sub_dtor((Sub*)((char*)self + 0x3c));
    Owner_base_dtor(self);
}

int main()
{
    Owner o;
    char* base = (char*)&o;
    _Dest_val(&o);
    if (g_sub_calls != 2) { std::printf("FAIL sub_calls=%d\n", g_sub_calls); return 1; }
    if (g_base_calls != 1) { std::printf("FAIL base_calls=%d\n", g_base_calls); return 1; }
    if (g_last_sub[0] != base + 0x48) { std::printf("FAIL first sub offset\n"); return 1; }
    if (g_last_sub[1] != base + 0x3c) { std::printf("FAIL second sub offset\n"); return 1; }
    if (g_last_base != base) { std::printf("FAIL base ptr\n"); return 1; }
    std::printf("_Dest_val_00456531_TEST PASS\n");
    return 0;
}