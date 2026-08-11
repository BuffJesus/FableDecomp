#include <cstdio>
#include <cstring>

class CCharString {
public:
    CCharString(const char* s, int len);
    char* m_data;
    int   m_len;
};

CCharString::CCharString(const char* s, int len)
{
    m_data = const_cast<char*>(s);
    m_len  = (len < 0) ? (int)std::strlen(s) : len;
}

class C3DMeshFileXMappingInfoChunk {
public:
    virtual CCharString GetChunkDescription() const;
    void* vtbl_pad;
};

CCharString C3DMeshFileXMappingInfoChunk::GetChunkDescription() const
{
    return CCharString("Mapping Information", -1);
}

int main()
{
    C3DMeshFileXMappingInfoChunk chunk;
    CCharString s = chunk.GetChunkDescription();
    bool ok = (s.m_len == (int)std::strlen("Mapping Information"))
              && (std::strcmp(s.m_data, "Mapping Information") == 0);
    if (ok) {
        std::printf("CHUNKDESC_OK\n");
        return 0;
    }
    std::printf("CHUNKDESC_FAIL\n");
    return 1;
}