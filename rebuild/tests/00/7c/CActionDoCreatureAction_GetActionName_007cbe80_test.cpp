#include <cstdio>
#include <cstring>

// Minimal CCharString that records what it was constructed with.
class CCharString {
public:
    CCharString(const char* s, long len) {
        m_data = s;
        m_len = len;
    }
    const char* m_data;
    long m_len;
};

class CActionDoCreatureAction {
public:
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString("DoCreatureAction", -1);
}

int main()
{
    CActionDoCreatureAction a;
    CCharString r = a.GetActionName();
    bool ok = (r.m_len == -1) && (std::strcmp(r.m_data, "DoCreatureAction") == 0);
    printf("GetActionName len=%ld str=%s %s\n", r.m_len, r.m_data, ok ? "OK" : "FAIL");
    if (!ok) return 1;
    return 0;
}