#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" wchar_t g_FableEmptyWideString_0129A8E0[1] = { 0 };

struct CWideStringData
{
    wchar_t* m_begin;
    wchar_t* m_end;
};

class CCharString
{
public:
    char* m_data;

    CCharString(const char* text, long length);
};

CCharString::CCharString(const char* text, long length)
{
    if (length < 0)
        length = (long)strlen(text);
    m_data = (char*)malloc(length + 1);
    memcpy(m_data, text, length);
    m_data[length] = 0;
}

struct CConvertBufferGuard
{
    char* p;
    __forceinline CConvertBufferGuard(char* p_) : p(p_) {}
    __forceinline ~CConvertBufferGuard() { if (p) free(p); }
};

class CWideString
{
public:
    CWideStringData* m_data;

    CCharString ConvertToCharString() const;
};

static long GetWideLength(CWideStringData* data)
{
    if (data == 0)
        return 0;
    return ((long)(char*)data->m_end - (long)(char*)data->m_begin) >> 1;
}

CCharString CWideString::ConvertToCharString() const
{
    char* buffer;
    long len;
    long i;
    long allocSize;

    len = GetWideLength(m_data);
    allocSize = len + 1;

    if (allocSize != 0)
        buffer = (char*)malloc(allocSize);
    else
        buffer = 0;

    memset(buffer, 0, allocSize);

    for (i = 0; i < GetWideLength(m_data); ++i)
    {
        if (m_data == 0)
        {
            buffer[i] = (char)g_FableEmptyWideString_0129A8E0[i];
        }
        else
        {
            buffer[i] = (char)m_data->m_begin[i];
        }
    }

    len = GetWideLength(m_data);
    buffer[len] = 0;

    CConvertBufferGuard guard(buffer);

    return CCharString(buffer, -1);
}

int main()
{
    wchar_t text[] = L"Hi";
    CWideStringData wd;
    wd.m_begin = text;
    wd.m_end = text + 2;

    CWideString ws;
    ws.m_data = &wd;

    CCharString cs = ws.ConvertToCharString();

    int ok = (cs.m_data != 0) && (strcmp(cs.m_data, "Hi") == 0);

    CWideString empty;
    empty.m_data = 0;
    CCharString cs2 = empty.ConvertToCharString();
    int ok2 = (cs2.m_data != 0) && (strcmp(cs2.m_data, "") == 0);

    if (ok && ok2)
        printf("CONVERTOK_PASS_0099B2C0\n");
    else
        printf("CONVERTOK_FAIL\n");

    return 0;
}
