#include <cstdio>

struct Sub {
    void** vtbl;
};

static int g_helper_called = 0;
static int g_base_called = 0;
static void* g_vtbl_marker = 0;

void* Sub_vtbl_1230ba0;  // definition of the extern global

void __fastcall helper_9fc550(Sub* sub)
{
    g_helper_called = 1;
    g_vtbl_marker = *sub->vtbl == 0 ? (void*)1 : (void*)1; // touch it
    (void)sub;
}

struct CTCKraken {
    char pad[0x70];
    Sub sub;

    void EndAttackBeam();
    void Base454b4c();
};

void CTCKraken::EndAttackBeam()
{
    this->sub.vtbl = (void**)&Sub_vtbl_1230ba0;
    helper_9fc550(&this->sub);
    this->Base454b4c();
}

void CTCKraken::Base454b4c()
{
    g_base_called = 1;
}

int main()
{
    CTCKraken k;
    for (int i = 0; i < (int)sizeof(k); ++i) ((char*)&k)[i] = 0;
    k.EndAttackBeam();

    bool ok = true;
    if (k.sub.vtbl != (void**)&Sub_vtbl_1230ba0) ok = false;
    if (!g_helper_called) ok = false;
    if (!g_base_called) ok = false;

    if (ok)
        printf("0045c5f6_TEST PASS\n");
    else
        printf("FAIL vtbl=%p helper=%d base=%d\n", (void*)k.sub.vtbl, g_helper_called, g_base_called);
    return 0;
}