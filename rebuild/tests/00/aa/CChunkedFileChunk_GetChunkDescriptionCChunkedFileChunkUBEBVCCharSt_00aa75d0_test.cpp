#include <cstdio>
#include <cstring>

// Independent behaviour test: model the same shape with a real ctor.
struct CCharString {
    const char* m_ptr;
    int   m_len;
    CCharString(const char* str, long len) : m_ptr(str), m_len((int)len) {}
};

static const char* const s_desc = "CHUNK_DESC_LITERAL";

struct CChunkedFileChunk {
    void* vtbl;
    CCharString GetChunkDescription() const;
};

CCharString CChunkedFileChunk::GetChunkDescription() const {
    return CCharString(s_desc, -1);
}

int main() {
    CChunkedFileChunk c;
    c.vtbl = 0;
    CCharString r = c.GetChunkDescription();
    if (r.m_ptr == s_desc && r.m_len == -1 &&
        std::strcmp(r.m_ptr, "CHUNK_DESC_LITERAL") == 0) {
        std::printf("GETCHUNKDESC_OK\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}