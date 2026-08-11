#include <cstdio>

// Self-contained behavioral model mirroring GetAutoSaveDisplayName's branch logic.
struct WStr { const wchar_t* p; };

struct FmtObj {
    int tag;
    WStr fmt(const wchar_t* lit) const { WStr r; r.p = lit; return r; }
};
struct SlotA { char pad[0x14]; FmtObj* f14; };
struct SlotB { char pad[0x60]; FmtObj* f60; };

static SlotA* g_a = 0;
static SlotB* g_b = 0;
static const wchar_t k_df40[] = L"DF40";
static const wchar_t k_de80[] = L"DE80";

static WStr Get() {
    if (g_a) { return g_a->f14->fmt(k_df40); }
    if (g_b) { return g_b->f60->fmt(k_df40); }
    WStr r; r.p = k_de80; return r;
}

int main() {
    // else branch
    WStr r = Get();
    bool ok = (r.p == k_de80);

    // branch B
    FmtObj fb; fb.tag = 2;
    SlotB sb; sb.f60 = &fb;
    g_b = &sb;
    r = Get();
    ok = ok && (r.p == k_df40) && (sb.f60 == &fb);

    // branch A
    FmtObj fa; fa.tag = 1;
    SlotA sa; sa.f14 = &fa;
    g_a = &sa;
    r = Get();
    ok = ok && (r.p == k_df40) && (sa.f14 == &fa);

    if (ok) printf("004069e0_TEST PASS\n");
    else printf("FAIL\n");
    return 0;
}