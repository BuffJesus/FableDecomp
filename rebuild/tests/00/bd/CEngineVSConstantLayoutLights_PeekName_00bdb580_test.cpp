#include <cstdio>
#include <cstring>

// Independent behaviour test. We provide a local CCharString ctor that copies
// the pointer/length so we can observe PeekName wiring the literal + (-1).

struct CCharString {
    char* m_data;
    int   m_len;
    CCharString(const char* s, int len) { m_data = (char*)s; m_len = len; }
};

class CEngineVSConstantLayoutLights {
public:
    CCharString PeekName();
};

CCharString CEngineVSConstantLayoutLights::PeekName()
{
    return CCharString((const char*)0x012a7d2c, -1);
}

int main()
{
    CEngineVSConstantLayoutLights obj;
    CCharString s = obj.PeekName();
    if (s.m_data == (char*)0x012a7d2c && s.m_len == -1)
        printf("PEEKNAME_OK\n");
    else
        printf("PEEKNAME_FAIL %p %d\n", (void*)s.m_data, s.m_len);
    return (s.m_data == (char*)0x012a7d2c && s.m_len == -1) ? 0 : 1;
}