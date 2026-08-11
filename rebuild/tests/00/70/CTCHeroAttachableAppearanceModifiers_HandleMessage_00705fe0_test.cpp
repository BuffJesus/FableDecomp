#include <cstdio>

struct Msg;
struct MsgVtbl {
    int (__fastcall *slot0)(Msg* self);
    int (__fastcall *slot1)(Msg* self);
};
struct Msg { MsgVtbl* vtbl; };
struct CTCHeroAttachableAppearanceModifiers {
    char pad[0x14];
    unsigned char f14;
};

void __fastcall HandleMessage(CTCHeroAttachableAppearanceModifiers* self, int, Msg* msg);

static int g_ret;
static int __fastcall retFn(Msg*) { return g_ret; }

int main()
{
    MsgVtbl vt; vt.slot0 = 0; vt.slot1 = retFn;
    Msg m; m.vtbl = &vt;

    CTCHeroAttachableAppearanceModifiers o1;
    o1.f14 = 0;
    g_ret = 3;
    HandleMessage(&o1, 0, &m);
    if (o1.f14 != 1) { printf("FAIL set\n"); return 1; }

    CTCHeroAttachableAppearanceModifiers o2;
    o2.f14 = 0;
    g_ret = 5;
    HandleMessage(&o2, 0, &m);
    if (o2.f14 != 0) { printf("FAIL noset\n"); return 1; }

    CTCHeroAttachableAppearanceModifiers o3;
    o3.f14 = 0x80;
    g_ret = 3;
    HandleMessage(&o3, 0, &m);
    if (o3.f14 != 0x81) { printf("FAIL or\n"); return 1; }

    printf("00705fe0_TEST PASS\n");
    return 0;
}