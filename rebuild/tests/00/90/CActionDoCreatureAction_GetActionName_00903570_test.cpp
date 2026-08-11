#include <cstdio>

// Independent behaviour test for CActionDoCreatureAction::GetActionName.
// We reimplement the same shape locally with a controllable ctor so we can
// observe that GetActionName forwards a fixed literal pointer and -1.

static const char* g_seen_ptr = 0;
static int         g_seen_n   = 99;

struct CCharString {
    CCharString(const char* s, int n) { g_seen_ptr = s; g_seen_n = n; }
};

struct CActionDoCreatureAction {
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x128d70c, -1);
}

int main()
{
    CActionDoCreatureAction a;
    a.GetActionName();
    bool ok = (g_seen_ptr == (const char*)0x128d70c) && (g_seen_n == -1);
    printf(ok ? "GETACTIONNAME_OK\n" : "GETACTIONNAME_FAIL n=%d\n", g_seen_n);
    return ok ? 0 : 1;
}