#include <cstdio>
#include <cstring>

struct CCharString
{
    char* m_data;
    long m_length;
    unsigned long m_flags;
    unsigned char m_flagByte;

    void AssignFromData(const char* data, int len);

    CCharString* InitFromCStr(const char* s);
};

int g_lastAssignLen = -1;
const char* g_lastAssignData = 0;

void CCharString::AssignFromData(const char* data, int len)
{
    g_lastAssignData = data;
    g_lastAssignLen = len;
}

CCharString* CCharString::InitFromCStr(const char* s)
{
    m_flags = m_flags & 0x80000000;
    m_flagByte = m_flagByte | 1;
    m_data = 0;
    m_length = 0;

    const char* p = s;
    int len = 0;
    if (*p != 0)
    {
        do
        {
            ++p;
            ++len;
        } while (*p != 0);
    }

    AssignFromData(s, len);
    return this;
}

int main()
{
    CCharString str;
    str.m_data = (char*)0x1234;
    str.m_length = 999;
    str.m_flags = 0x80000005;
    str.m_flagByte = 0xF0;

    const char* input = "hello world";
    CCharString* ret = str.InitFromCStr(input);

    bool ok = true;
    if (ret != &str) ok = false;
    if (str.m_data != 0) ok = false;
    if (str.m_length != 0) ok = false;
    if (str.m_flags != 0x80000000) ok = false;
    if (str.m_flagByte != 0xF1) ok = false;
    if (g_lastAssignData != input) ok = false;
    if (g_lastAssignLen != (int)strlen(input)) ok = false;

    if (ok)
        printf("TEST_PASS_marker_InitFromCStr\n");
    else
        printf("TEST_FAIL\n");

    return 0;
}