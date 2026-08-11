#include <cstdio>
#include <cstring>

struct CCharString {
    char* m_pStr;
    int   m_len;
    CCharString(const char* s, int n);
};

struct C3DMeshFileSubMeshChunk {
    void* vtbl;
    virtual CCharString GetChunkDescription() const;
};

// Local stand-in for the external CCharString ctor (0x99ebf0).
CCharString::CCharString(const char* s, int n)
{
    m_pStr = const_cast<char*>(s);
    m_len  = (n < 0) ? (int)strlen(s) : n;
}

int main()
{
    C3DMeshFileSubMeshChunk obj;
    obj.vtbl = 0;
    CCharString r = obj.GetChunkDescription();
    if (r.m_pStr && strcmp(r.m_pStr, "SubMesh") == 0 && r.m_len == 7) {
        printf("CHUNKDESC_OK\n");
        return 0;
    }
    printf("CHUNKDESC_FAIL\n");
    return 1;
}