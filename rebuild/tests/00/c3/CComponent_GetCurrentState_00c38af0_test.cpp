#include <cstdio>

struct CUIState;

struct CInner {
    CUIState* Get(int* out);
};

static CUIState* g_ret;
static CInner* g_seen_self;
static int* g_seen_out;

CUIState* CInner::Get(int* out)
{
    g_seen_self = this;
    g_seen_out = out;
    return g_ret;
}

struct CComponent {
    char pad[0xd8];
    CInner inner;
};

CUIState* __fastcall CComponent_GetCurrentState(CComponent* self)
{
    int tmp;
    return self->inner.Get(&tmp);
}

int main()
{
    CComponent c;
    g_ret = (CUIState*)0x12345678;
    CUIState* r = CComponent_GetCurrentState(&c);
    if (r != (CUIState*)0x12345678) { std::printf("FAIL ret\n"); return 1; }
    if (g_seen_self != &c.inner) { std::printf("FAIL self\n"); return 1; }
    if ((char*)g_seen_self - (char*)&c != 0xd8) { std::printf("FAIL off\n"); return 1; }
    std::printf("CComponent_00c38af0_TEST PASS\n");
    return 0;
}