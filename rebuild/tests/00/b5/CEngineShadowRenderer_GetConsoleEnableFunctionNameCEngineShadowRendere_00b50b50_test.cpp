#include <cstdio>
#include <cstring>

class CCharString {
public:
    char* m_pStr;
    int   m_len;
    CCharString(const char* s, long n);
};

struct CEngineShadowRenderer {
    void* vtbl;
    virtual CCharString GetConsoleEnableFunctionName();
};

// Local stand-in for the external CCharString ctor (0x99ebf0).
CCharString::CCharString(const char* s, long n)
{
    m_pStr = const_cast<char*>(s);
    m_len  = (n < 0) ? (int)strlen(s) : (int)n;
}

int main()
{
    CEngineShadowRenderer obj;
    obj.vtbl = 0;
    CCharString r = obj.GetConsoleEnableFunctionName();
    if (r.m_pStr && strcmp(r.m_pStr, "EnableShadows") == 0 && r.m_len == 13) {
        printf("SHADOW_CONSOLE_OK\n");
        return 0;
    }
    printf("SHADOW_CONSOLE_FAIL\n");
    return 1;
}