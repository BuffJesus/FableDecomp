#include <cstdio>
#include <cstring>

// Independent behaviour test.
class CCharString
{
public:
    CCharString(const char* s, int len);
    const char* data() const { return m_data; }
private:
    char* m_data;
};

CCharString::CCharString(const char* s, int len)
{
    m_data = 0;
    if (s && len != 0)
        m_data = const_cast<char*>(s);
}

class CEngineScreenEffectOutlineGlow
{
public:
    virtual CCharString GetConsoleEnableFunctionName();
};

CCharString CEngineScreenEffectOutlineGlow::GetConsoleEnableFunctionName()
{
    return CCharString("EnableOutlineGlow", -1);
}

int main()
{
    CEngineScreenEffectOutlineGlow obj;
    CCharString name = obj.GetConsoleEnableFunctionName();
    bool ok = name.data() && std::strcmp(name.data(), "EnableOutlineGlow") == 0;
    if (ok)
        std::printf("OUTLINEGLOW_CONSOLE_NAME_OK\n");
    else
        std::printf("FAIL\n");
    return ok ? 0 : 1;
}