#include <cstdio>

struct CCharString {
    int d;
    CCharString(const char* s, int n);
};

// Fake engine ctor: record the args it was called with.
static const char* g_last_s;
static int         g_last_n;
CCharString::CCharString(const char* s, int n) {
    g_last_s = s;
    g_last_n = n;
    this->d = (int)s;
}

struct CActionDoCreatureAction {
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName() {
    return CCharString((const char*)0x123ad54, -1);
}

int main() {
    CActionDoCreatureAction a;
    CCharString r = a.GetActionName();
    if (g_last_s == (const char*)0x123ad54 && g_last_n == -1 && r.d == 0x123ad54) {
        std::printf("00750a40_TEST PASS\n");
    } else {
        std::printf("FAIL s=%p n=%d d=%x\n", g_last_s, g_last_n, r.d);
    }
    return 0;
}