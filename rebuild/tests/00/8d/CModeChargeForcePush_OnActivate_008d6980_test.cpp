#include <cstdio>

struct Sub {
    char pad[0x10];
    unsigned char flag;          // +0x10
    void Helper(int a, int b);   // __fastcall member -> call 0x833a10
};

struct CModeChargeForcePush {
    void* vt;
    Sub*  sub;

    void BaseActivate();
    void OnActivate();
};

void CModeChargeForcePush::OnActivate()
{
    BaseActivate();
    Sub* s = sub;
    if (s->flag == 1)
        s->Helper(0, 0);
}

static int g_base = 0;
static int g_helper = 0;
static Sub* g_helper_this = 0;
static int g_helper_a = -1, g_helper_b = -1;

void CModeChargeForcePush::BaseActivate() { g_base++; }
void Sub::Helper(int a, int b) { g_helper++; g_helper_this = this; g_helper_a = a; g_helper_b = b; }

int main()
{
    // flag == 1 -> Helper called on sub with (0,0)
    Sub s1; s1.flag = 1;
    CModeChargeForcePush o1; o1.vt = 0; o1.sub = &s1;
    g_base = g_helper = 0; g_helper_this = 0; g_helper_a = g_helper_b = -1;
    o1.OnActivate();
    bool ok = (g_base == 1 && g_helper == 1 && g_helper_this == &s1 &&
               g_helper_a == 0 && g_helper_b == 0);

    // flag != 1 -> Helper skipped
    Sub s2; s2.flag = 0;
    CModeChargeForcePush o2; o2.vt = 0; o2.sub = &s2;
    g_base = g_helper = 0;
    o2.OnActivate();
    ok = ok && (g_base == 1 && g_helper == 0);

    // flag == 2 -> also skipped (strict ==1)
    Sub s3; s3.flag = 2;
    CModeChargeForcePush o3; o3.vt = 0; o3.sub = &s3;
    g_base = g_helper = 0;
    o3.OnActivate();
    ok = ok && (g_base == 1 && g_helper == 0);

    if (ok) printf("FORCEPUSH_ONACTIVATE_OK\n");
    else printf("FAIL\n");
    return ok ? 0 : 1;
}