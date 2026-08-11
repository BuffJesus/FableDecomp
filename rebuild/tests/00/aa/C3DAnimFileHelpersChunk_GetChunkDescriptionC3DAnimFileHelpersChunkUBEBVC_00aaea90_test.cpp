#include <stdio.h>

class CCharString {
public:
    CCharString(const char* s, int n);
    char* m_data;
    int m_n;
};

class C3DAnimFileHelpersChunk {
public:
    CCharString GetChunkDescription() const;
    int m_dummy;
};

// Local stand-in for the real ctor at 0x99ebf0.
CCharString::CCharString(const char* s, int n)
{
    m_data = (char*)s;
    m_n = n;
}

CCharString C3DAnimFileHelpersChunk::GetChunkDescription() const
{
    return CCharString((const char*)0x129e564, -1);
}

int main()
{
    C3DAnimFileHelpersChunk c;
    CCharString r = c.GetChunkDescription();
    if (r.m_data == (char*)0x129e564 && r.m_n == -1)
        printf("CHUNKDESC_OK data=%p n=%d\n", r.m_data, r.m_n);
    else
        printf("FAIL\n");
    return 0;
}