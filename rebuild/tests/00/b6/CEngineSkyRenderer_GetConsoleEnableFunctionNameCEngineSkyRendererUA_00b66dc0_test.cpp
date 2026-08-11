#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Local standalone model mirroring the source shape.

class CCharString {
public:
    char* m_ptr;
    int m_len;
    CCharString(const char* s, int len);
};

CCharString::CCharString(const char* s, int len)
{
    if (len < 0) len = (int)strlen(s);
    m_len = len;
    m_ptr = (char*)malloc(len + 1);
    memcpy(m_ptr, s, len);
    m_ptr[len] = 0;
}

class CEngineSkyRenderer {
public:
    virtual CCharString GetConsoleEnableFunctionName();
};

CCharString CEngineSkyRenderer::GetConsoleEnableFunctionName()
{
    return CCharString("SkyRenderer", -1);
}

int main()
{
    CEngineSkyRenderer r;
    CCharString s = r.GetConsoleEnableFunctionName();
    if (s.m_len == 11 && strcmp(s.m_ptr, "SkyRenderer") == 0) {
        printf("PASS_SKYRENDER_NAME\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}