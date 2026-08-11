#include <cstdio>

struct CCharString {
    const char* p;
    int len;
    CCharString(const char* lit, int l) : p(lit), len(l) {}
    int dummy;
};

struct C3DMeshFileUniqueVerticesChunk {
    CCharString GetChunkInfo() const;
};

CCharString C3DMeshFileUniqueVerticesChunk::GetChunkInfo() const
{
    return CCharString((const char*)0x122d70e, -1);
}

int main()
{
    C3DMeshFileUniqueVerticesChunk c;
    CCharString r = c.GetChunkInfo();
    if (r.p == (const char*)0x122d70e && r.len == -1) {
        printf("GETCHUNKINFO_OK\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}