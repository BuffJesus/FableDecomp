#include <stdio.h>
#include <string.h>

class CCharString {
public:
    char* m_data;
    CCharString(const char* s, long n);
};

// Local stand-in for the engine ctor (0x0099ebf0): copy the literal.
CCharString::CCharString(const char* s, long n)
{
    static char buf[64];
    if (n < 0) n = (long)strlen(s);
    long i;
    for (i = 0; i < n && i < 63; ++i) buf[i] = s[i];
    buf[i] = 0;
    m_data = buf;
}

class C3DMeshFileXTextureIndexChunk {
public:
    virtual CCharString GetChunkDescription() const;
};

CCharString C3DMeshFileXTextureIndexChunk::GetChunkDescription() const
{
    return CCharString("XTextureIndex", -1);
}

int main()
{
    C3DMeshFileXTextureIndexChunk chunk;
    CCharString r = chunk.GetChunkDescription();
    if (r.m_data && strcmp(r.m_data, "XTextureIndex") == 0) {
        printf("PASS_XTEXIDX_CHUNKDESC\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}