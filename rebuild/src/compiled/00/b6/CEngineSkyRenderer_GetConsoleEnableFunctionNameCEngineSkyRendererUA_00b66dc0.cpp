// CEngineSkyRenderer::GetConsoleEnableFunctionName
// Retail 0x00b66dc0: constructs a CCharString from a string literal, returns by value.

class CCharString {
public:
    char* m_ptr;
    CCharString(const char* s, int len);
};

class CEngineSkyRenderer {
public:
    virtual CCharString GetConsoleEnableFunctionName();
};

CCharString CEngineSkyRenderer::GetConsoleEnableFunctionName()
{
    return CCharString("SkyRenderer", -1);
}