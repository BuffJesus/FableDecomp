#include <stdio.h>
#include <string.h>

class CCharString {
public:
    CCharString(const char* s, int len);
    const char* c_str() const { return m_pData; }
private:
    char* m_pData;
    int   m_len;
};

static char g_buf[64];
CCharString::CCharString(const char* s, int len)
{
    if (len < 0) len = (int)strlen(s);
    strncpy(g_buf, s, sizeof(g_buf)-1);
    g_buf[sizeof(g_buf)-1] = 0;
    m_pData = g_buf;
    m_len = len;
}

const char g_actionName[] = "DoCreatureAction";

class CActionDoCreatureAction {
public:
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString(g_actionName, -1);
}

int main()
{
    CActionDoCreatureAction a;
    CCharString r = a.GetActionName();
    if (strcmp(r.c_str(), "DoCreatureAction") == 0)
        printf("OK name=%s\n", r.c_str());
    else
        printf("FAIL\n");
    return 0;
}