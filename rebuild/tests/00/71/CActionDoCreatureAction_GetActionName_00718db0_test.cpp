#include <cstdio>

static const char* g_captured_s;
static int g_captured_n;

struct CCharString {
    char* buf;
    CCharString(const char* s, int n) {
        g_captured_s = s;
        g_captured_n = n;
        buf = (char*)s;
    }
    CCharString(const CCharString& o) { buf = o.buf; }
};

struct CActionDoCreatureAction {
    int _pad;
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x1265850, -1);
}

int main()
{
    CActionDoCreatureAction obj; obj._pad = 0;
    CCharString r = obj.GetActionName();
    bool ok = (g_captured_s == (const char*)0x1265850)
           && (g_captured_n == -1)
           && (r.buf == (char*)0x1265850);
    if (ok) printf("00718db0_TEST PASS\n");
    else printf("FAIL s=%p n=%d buf=%p\n", g_captured_s, g_captured_n, r.buf);
    return 0;
}