#include <cstdio>

struct CCharString {
    char* m_data;
    int   m_len;
    int   m_ref;
    CCharString(const char* s, int len) { m_data = (char*)s; m_len = len; m_ref = 0; }
};

static const char* const kActionNameLit = (const char*)0x0123d4b0;

struct CActionDoCreatureAction {
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString(kActionNameLit, -1);
}

int main()
{
    CActionDoCreatureAction obj;
    CCharString r = obj.GetActionName();
    if (r.m_data == (char*)0x0123d4b0 && r.m_len == -1)
        std::printf("007d2220_TEST PASS\n");
    else
        std::printf("FAIL data=%p len=%d\n", (void*)r.m_data, r.m_len);
    return 0;
}