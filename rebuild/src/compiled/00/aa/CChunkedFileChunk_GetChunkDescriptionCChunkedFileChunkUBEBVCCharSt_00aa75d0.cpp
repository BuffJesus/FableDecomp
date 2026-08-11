// Faithful VC7.1 translation of CChunkedFileChunk::GetChunkDescription (00aa75d0)
// Struct-return const method: return CCharString("...", -1);

struct CCharString {
    char* m_ptr;
    int   m_len;
    // ctor at 0x0099ebf0: CCharString(const char* str, long len)
    CCharString(const char* str, long len);
};

// The literal at 0x129e4a0
static const char* const s_desc = (const char*)0x129e4a0;

struct CChunkedFileChunk {
    void* vtbl;
    CCharString GetChunkDescription() const;
};

CCharString CChunkedFileChunk::GetChunkDescription() const {
    return CCharString(s_desc, -1);
}