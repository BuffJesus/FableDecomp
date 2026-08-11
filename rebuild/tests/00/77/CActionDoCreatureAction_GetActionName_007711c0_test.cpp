#include <cstdio>

struct CCharString {
    char* p;
    CCharString(const char* s, int n);
};

struct CActionDoCreatureAction {
    int dummy;
    CCharString GetActionName();
};

static const char* g_last_s = 0;
static int g_last_n = 999;

CCharString::CCharString(const char* s, int n) {
    g_last_s = s;
    g_last_n = n;
    p = (char*)s;
}

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString("SomeActionName", -1);
}

int main() {
    CActionDoCreatureAction obj;
    obj.dummy = 42;
    CCharString r = obj.GetActionName();
    if (g_last_n == -1 && g_last_s != 0 && r.p == g_last_s) {
        printf("007711c0_TEST PASS\n");
    } else {
        printf("FAIL n=%d s=%p\n", g_last_n, (void*)g_last_s);
    }
    return 0;
}