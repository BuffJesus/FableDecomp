#include <cstdio>

struct CCharStringData
{
    long m_begin;
    long m_end;
};

class CCharString
{
public:
    CCharStringData* m_data;

    long GetLength();
};

long CCharString::GetLength()
{
    CCharStringData* data = m_data;
    if (data == 0)
    {
        return 0;
    }
    return (data->m_end - data->m_begin) >> 1;
}

int main()
{
    CCharString emptyStr;
    emptyStr.m_data = 0;
    long lenEmpty = emptyStr.GetLength();

    CCharStringData chunk;
    chunk.m_begin = 1000;
    chunk.m_end = 1010;

    CCharString str;
    str.m_data = &chunk;
    long lenFive = str.GetLength();

    if (lenEmpty == 0 && lenFive == 5)
    {
        printf("CHARSTRING_GETLENGTH_OK\n");
    }
    else
    {
        printf("CHARSTRING_GETLENGTH_FAIL %ld %ld\n", lenEmpty, lenFive);
    }
    return 0;
}