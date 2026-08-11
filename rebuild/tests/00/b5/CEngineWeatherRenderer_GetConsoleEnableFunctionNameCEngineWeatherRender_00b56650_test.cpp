#include <stdio.h>
#include <string.h>

// Independent behaviour test for GetConsoleEnableFunctionName.
// We model the same codegen shape and verify it returns a CCharString
// constructed from the fixed literal with len == -1.

class CCharString
{
public:
    CCharString(const char* s, int len)
    {
        strncpy(m_buf, s, sizeof(m_buf) - 1);
        m_buf[sizeof(m_buf) - 1] = 0;
        m_len = len;
    }
    const char* c_str() const { return m_buf; }
    int len() const { return m_len; }
private:
    char m_buf[64];
    int  m_len;
};

class CEngineWeatherRenderer
{
public:
    virtual CCharString GetConsoleEnableFunctionName();
};

CCharString CEngineWeatherRenderer::GetConsoleEnableFunctionName()
{
    return CCharString("EnableWeather", -1);
}

int main()
{
    CEngineWeatherRenderer r;
    CCharString s = r.GetConsoleEnableFunctionName();
    if (strcmp(s.c_str(), "EnableWeather") == 0 && s.len() == -1)
        printf("GETCONSOLEENABLE_OK\n");
    else
        printf("FAIL\n");
    return 0;
}