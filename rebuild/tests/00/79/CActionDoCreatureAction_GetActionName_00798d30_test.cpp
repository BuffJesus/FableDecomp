#include <stdio.h>

class CCharString {
public:
    CCharString(const char* s, int len) { m_s = s; m_len = len; }
    const char* m_s;
    int m_len;
};

class CActionDoCreatureAction {
public:
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x0126C12C, -1);
}

int main()
{
    CActionDoCreatureAction obj;
    CCharString r = obj.GetActionName();
    if (r.m_s == (const char*)0x0126C12C && r.m_len == -1) {
        printf("00798d30_TEST PASS\n");
    } else {
        printf("FAIL s=%p len=%d\n", r.m_s, r.m_len);
    }
    return 0;
}