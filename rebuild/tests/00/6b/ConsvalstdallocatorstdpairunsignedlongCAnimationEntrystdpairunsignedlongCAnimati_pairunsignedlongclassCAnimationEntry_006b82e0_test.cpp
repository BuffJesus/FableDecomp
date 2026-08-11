#include <cstdio>

struct Sub {
    void M(void* a, void* b);
};

static void* g_this;
static void* g_arg0;
static void* g_arg1;

void Sub::M(void* a, void* b)
{
    g_this = this;
    g_arg0 = a;
    g_arg1 = b;
}

void __fastcall _Cons_val_pair_ulong_CAnimationEntry(void* self, int, void* a, void* b);

int main()
{
    char base[0x100];
    void* a = (void*)0x11111111;
    void* b = (void*)0x22222222;
    _Cons_val_pair_ulong_CAnimationEntry(base, 0, a, b);
    // source calls ((Sub*)(self+0x90))->M(b, a): arg0=b, arg1=a
    bool ok = (g_this == (void*)(base + 0x90)) && (g_arg0 == b) && (g_arg1 == a);
    if (ok) printf("006b82e0_TEST PASS\n");
    else printf("FAIL this=%p arg0=%p arg1=%p\n", g_this, g_arg0, g_arg1);
    return 0;
}