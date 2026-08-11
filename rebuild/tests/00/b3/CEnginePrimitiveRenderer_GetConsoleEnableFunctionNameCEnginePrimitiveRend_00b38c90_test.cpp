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

class CEnginePrimitiveRenderer
{
public:
    virtual CCharString GetConsoleEnableFunctionName();
};

CCharString CEnginePrimitiveRenderer::GetConsoleEnableFunctionName()
{
    return CCharString("ENABLE_PRIMITIVE_RENDERER", -1);
}

int main()
{
    CEnginePrimitiveRenderer obj;
    CCharString name = obj.GetConsoleEnableFunctionName();
    if (name.data() && std::strcmp(name.data(), "ENABLE_PRIMITIVE_RENDERER") == 0)
        std::printf("PRIMRENDERER_CONSOLEFN_OK\n");
    else
        std::printf("FAIL\n");
    return (name.data() && std::strcmp(name.data(), "ENABLE_PRIMITIVE_RENDERER") == 0) ? 0 : 1;
}