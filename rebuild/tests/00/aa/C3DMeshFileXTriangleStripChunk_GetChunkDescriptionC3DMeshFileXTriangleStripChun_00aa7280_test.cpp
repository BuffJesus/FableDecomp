#include <stdio.h>

class CCharString {
public:
    CCharString(const char* s, int n);
    const char* ptr() const { return m_p; }
    int len() const { return m_len; }
private:
    char* m_p;
    int   m_len;
    int   m_cap;
};

// Capture what the ctor received.
static const char* g_seen_ptr = 0;
static int g_seen_len = 999;

CCharString::CCharString(const char* s, int n)
{
    g_seen_ptr = s;
    g_seen_len = n;
    m_p = (char*)s;
    m_len = n;
    m_cap = 0;
}

class C3DMeshFileXTriangleStripChunk {
public:
    virtual CCharString GetChunkDescription() const;
};

CCharString C3DMeshFileXTriangleStripChunk::GetChunkDescription() const
{
    return CCharString((const char*)0x129e3ac, -1);
}

int main()
{
    C3DMeshFileXTriangleStripChunk chunk;
    CCharString r = chunk.GetChunkDescription();
    if (g_seen_ptr == (const char*)0x129e3ac && g_seen_len == -1 &&
        r.ptr() == (const char*)0x129e3ac && r.len() == -1) {
        printf("STRIPCHUNK_DESC_OK\n");
        return 0;
    }
    printf("FAIL seen_ptr=%p seen_len=%d\n", (void*)g_seen_ptr, g_seen_len);
    return 1;
}