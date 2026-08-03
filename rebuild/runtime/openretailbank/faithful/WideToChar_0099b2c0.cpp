#include <stdlib.h>
#include <string.h>

extern "C" wchar_t g_FableEmptyWideString_0129A8E0[1];

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
