#include <stdio.h>

struct CCharString {
    CCharString(const char* lit, int len);
    int dummy;
};

struct C3DMeshFileTransformationChunk {
    CCharString GetChunkInfo() const;
};

static const char* g_lit = 0;
static int g_len = 0;

CCharString::CCharString(const char* lit, int len)
{
    g_lit = lit;
    g_len = len;
    dummy = 0x1234;
}

CCharString C3DMeshFileTransformationChunk::GetChunkInfo() const
{
    return CCharString((const char*)0x122d70e, -1);
}

int main()
{
    C3DMeshFileTransformationChunk c;
    CCharString r = c.GetChunkInfo();
    if (g_lit == (const char*)0x122d70e && g_len == -1 && r.dummy == 0x1234) {
        printf("CHUNKINFO_OK\n");
        return 0;
    }
    printf("CHUNKINFO_FAIL\n");
    return 1;
}