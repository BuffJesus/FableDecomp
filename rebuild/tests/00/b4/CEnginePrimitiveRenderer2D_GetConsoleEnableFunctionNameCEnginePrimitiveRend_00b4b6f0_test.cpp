// Independent behaviour test for
// CEnginePrimitiveRenderer2D::GetConsoleEnableFunctionName.
// Reproduces the same shape (virtual returning a constant CCharString built
// from a literal + -1) with a locally-defined CCharString ctor, so it links
// standalone.
#include <cstdio>
#include <cstring>

class CCharString
{
public:
    CCharString(const char* s, int n);
    char* m_data;
    int   m_len;
};

// Local stand-in for the engine ctor: length -1 means strlen.
CCharString::CCharString(const char* s, int n)
{
    m_len  = (n < 0) ? (int)std::strlen(s) : n;
    m_data = new char[m_len + 1];
    std::memcpy(m_data, s, m_len);
    m_data[m_len] = 0;
}

class CEnginePrimitiveRenderer2D
{
public:
    virtual CCharString GetConsoleEnableFunctionName();
};

CCharString CEnginePrimitiveRenderer2D::GetConsoleEnableFunctionName()
{
    return CCharString("ConsoleEnable", -1);
}

int main()
{
    CEnginePrimitiveRenderer2D r;
    CCharString cs = r.GetConsoleEnableFunctionName();
    bool ok = (cs.m_len == 13) && std::strcmp(cs.m_data, "ConsoleEnable") == 0;
    if (ok)
        std::printf("GETCONSOLEENABLE_OK len=%d val=%s\n", cs.m_len, cs.m_data);
    else
        std::printf("GETCONSOLEENABLE_FAIL\n");
    return ok ? 0 : 1;
}