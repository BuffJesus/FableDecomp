#include <cstdio>

static void* g_arg_seen;
static bool* g_flag_seen;

extern "C" void* __stdcall helper_486189(void* arg, bool* pflag)
{
    g_arg_seen = arg;
    g_flag_seen = pflag;
    return (void*)0xABCD;
}

void* __fastcall _Umove_EWeaponClass(void* self, int edx, void* arg);

int main()
{
    void* obj = (void*)0x1000;
    void* a = (void*)0x2000;
    void* r = _Umove_EWeaponClass(obj, 0, a);
    if (r == (void*)0xABCD && g_arg_seen == a && *g_flag_seen == false)
        printf("004861df_TEST PASS\n");
    else
        printf("FAIL r=%p arg=%p flag=%d\n", r, g_arg_seen, g_flag_seen?*g_flag_seen:9);
    return 0;
}