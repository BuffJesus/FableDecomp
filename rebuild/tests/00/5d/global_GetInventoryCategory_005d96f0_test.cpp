#include <cstdio>

struct GObj { char pad[0x6c]; long field_6c; };
GObj g_inst;
GObj* g_obj = &g_inst;

bool g_helper_ret = false;
bool HelperCheck(void) { return g_helper_ret; }

long __fastcall GetInventoryCategory(long);

int main()
{
    g_inst.field_6c = 77;

    g_helper_ret = true;
    long a = GetInventoryCategory(0);

    g_helper_ret = false;
    long b = GetInventoryCategory(0);

    if (a == 77 && b == 4)
        printf("005d96f0_TEST PASS\n");
    else
        printf("FAIL a=%ld b=%ld\n", a, b);
    return 0;
}