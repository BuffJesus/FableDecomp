#include <cstdio>
struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;
struct IInner {
    virtual bool slot7(CTCCreatureNavigation* nav, C3DVector* dest) = 0;
};
struct CTCCreatureNavigation {
    void* pad0;
    IInner* inner;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest) {
        return this->inner->slot7(this, dest);
    }
};
static CTCCreatureNavigation* g_expect_nav;
static C3DVector* g_expect_dest;
static bool g_ret;
struct Impl : IInner {
    bool slot7(CTCCreatureNavigation* nav, C3DVector* dest) {
        if (nav != g_expect_nav) { std::printf("FAIL nav\n"); return false; }
        if (dest != g_expect_dest) { std::printf("FAIL dest\n"); return false; }
        return g_ret;
    }
};
int main() {
    Impl impl;
    CTCCreatureNavigation nav; nav.pad0 = 0; nav.inner = &impl;
    C3DVector d; d.x=1; d.y=2; d.z=3;
    g_expect_nav = &nav; g_expect_dest = &d;
    g_ret = true;
    if (nav.IsNewDestinationGoingToResetNavigation(&d) != true) { std::printf("FAIL true\n"); return 1; }
    g_ret = false;
    if (nav.IsNewDestinationGoingToResetNavigation(&d) != false) { std::printf("FAIL false\n"); return 1; }
    std::printf("OK_00c09be0\n");
    return 0;
}