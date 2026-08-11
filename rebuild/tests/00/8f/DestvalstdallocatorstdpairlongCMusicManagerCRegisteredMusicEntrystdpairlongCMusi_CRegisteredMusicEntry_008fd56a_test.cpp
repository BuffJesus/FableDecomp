#include <cstdio>

// Model the two callees to observe call order + pointer offsets.
static void* g_helper_arg = 0;
static void* g_tail_arg   = 0;
static int   g_order = 0;
static int   g_helper_order = 0;
static int   g_tail_order = 0;

extern "C" void __fastcall helper_a01c10(void* self)
{
    g_helper_arg = self;
    g_helper_order = ++g_order;
}
extern "C" void __fastcall tail_8fd544(void* self)
{
    g_tail_arg = self;
    g_tail_order = ++g_order;
}

extern "C" void __fastcall _Dest_val_pair_long_CRegisteredMusicEntry(void* self);

int main()
{
    char obj[0x80];
    for (int i = 0; i < 0x80; ++i) obj[i] = 0;
    void* self = obj;

    _Dest_val_pair_long_CRegisteredMusicEntry(self);

    bool ok = true;
    if (g_helper_arg != (void*)((char*)self + 0x34)) ok = false; // helper got this+0x34
    if (g_tail_arg   != self)                        ok = false; // tail got this
    if (g_helper_order != 1)                         ok = false; // helper first
    if (g_tail_order   != 2)                         ok = false; // tail last

    if (ok) printf("008fd56a_TEST PASS\n");
    else    printf("008fd56a_TEST FAIL h=%p t=%p ho=%d to=%d\n",
                   g_helper_arg, g_tail_arg, g_helper_order, g_tail_order);
    return 0;
}