#include <cstdio>
#include <cstring>

class CCharString
{
public:
    CCharString(const char* s, int len);
    const char* c_str() const { return m_data; }
private:
    char* m_data;
    char  m_buf[64];
};

// local stand-in for the ctor at 0x0099ebf0
CCharString::CCharString(const char* s, int len)
{
    int n = (len < 0) ? (int)strlen(s) : len;
    if (n > 63) n = 63;
    memcpy(m_buf, s, n);
    m_buf[n] = 0;
    m_data = m_buf;
}

class C3DMeshFileDummyObjectChunk
{
public:
    virtual const CCharString GetChunkDescription() const;
};

const CCharString C3DMeshFileDummyObjectChunk::GetChunkDescription() const
{
    return CCharString("DummyObject", -1);
}

int main()
{
    C3DMeshFileDummyObjectChunk chunk;
    CCharString d = chunk.GetChunkDescription();
    if (strcmp(d.c_str(), "DummyObject") == 0)
        printf("CHUNKDESC_OK\n");
    else
        printf("CHUNKDESC_FAIL\n");
    return strcmp(d.c_str(), "DummyObject") == 0 ? 0 : 1;
}