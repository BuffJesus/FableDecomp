#include <cstdio>

struct CTCMapwho;

struct Helper2 {
    void Notify(CTCMapwho* thing);
};
struct HelperObj {
    char pad[0x10];
    Helper2* f10;
};
struct CTCMapwho {
    char pad[0x50];
    unsigned char f50;
};

static Helper2* g_expected_that = 0;
static CTCMapwho* g_expected_thing = 0;
static int g_called = 0;

static HelperObj g_obj;

HelperObj* helper_686d30(void) {
    return &g_obj;
}

void Helper2::Notify(CTCMapwho* thing) {
    g_called = 1;
    g_expected_that = this;
    g_expected_thing = thing;
}

void __fastcall CTCMapwho_OnDie(CTCMapwho* self);

int main() {
    Helper2 fakeThat;
    g_obj.f10 = &fakeThat;

    CTCMapwho m;
    for (int i = 0; i < (int)sizeof(m.pad); ++i) m.pad[i] = 0;

    m.f50 = 0;
    g_called = 0;
    CTCMapwho_OnDie(&m);
    if (g_called != 0) { printf("FAIL: called when bit clear\n"); return 1; }

    m.f50 = 1;
    g_called = 0;
    CTCMapwho_OnDie(&m);
    if (g_called != 1) { printf("FAIL: not called\n"); return 1; }
    if (g_expected_that != &fakeThat) { printf("FAIL: that mismatch\n"); return 1; }
    if (g_expected_thing != &m) { printf("FAIL: thing mismatch\n"); return 1; }

    m.f50 = 2;
    g_called = 0;
    CTCMapwho_OnDie(&m);
    if (g_called != 0) { printf("FAIL: called when only bit1 set\n"); return 1; }

    printf("0071a730_TEST PASS\n");
    return 0;
}