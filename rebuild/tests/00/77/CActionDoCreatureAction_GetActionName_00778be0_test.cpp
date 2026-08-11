#include <cstdio>
#include <cstring>

// Self-contained model of the CCharString(const char*, int) construction that
// GetActionName performs. len==-1 means "use strlen".

class CCharString {
public:
    char buf[64];
    const char* src;
    int len;
    CCharString(const char* s, int n) {
        src = s; len = n;
        if (n < 0) strncpy(buf, s, 63);
        else       strncpy(buf, s, (n < 63 ? n : 63));
        buf[63] = 0;
    }
};

class CActionDoCreatureAction {
public:
    CCharString GetActionName();
};

// The literal 0x123b918 in retail is a pointer into .rdata; here we fake it.
static const char* g_actionName = "DoCreatureAction";

CCharString CActionDoCreatureAction::GetActionName()
{
    // mirrors: return CCharString(<literal ptr>, -1);
    return CCharString(g_actionName, -1);
}

int main() {
    CActionDoCreatureAction a;
    CCharString r = a.GetActionName();
    bool ok = (r.len == -1) && (r.src == g_actionName)
              && (strcmp(r.buf, "DoCreatureAction") == 0);
    if (ok) printf("00778be0_TEST PASS\n");
    else    printf("FAIL len=%d\n", r.len);
    return 0;
}