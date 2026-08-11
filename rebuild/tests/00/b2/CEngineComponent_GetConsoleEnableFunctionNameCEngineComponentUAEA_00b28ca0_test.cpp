#include <stdio.h>

class CCharString {
public:
    char* m_p;
    long  m_n;
    CCharString(const char* s, long n) { m_p = (char*)s; m_n = n; }
};

class CEngineComponent {
public:
    virtual CCharString GetConsoleEnableFunctionName();
};

CCharString CEngineComponent::GetConsoleEnableFunctionName()
{
    return CCharString((const char*)0x122d70e, -1);
}

int main()
{
    CEngineComponent c;
    CCharString r = c.GetConsoleEnableFunctionName();
    if (r.m_p == (char*)0x122d70e && r.m_n == -1) {
        printf("GETCONSOLEENABLE_OK\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}