// Faithful VC7.1 translation of C3DMeshFileMaterialListChunk::GetChunkDescription (00ac1270)
// Struct-return const method: return CCharString("...", -1);

struct CCharString {
    char* m_ptr;
    int   m_len;
    // ctor at 0x0099ebf0: CCharString(const char* str, long len)
    CCharString(const char* str, long len);
};

// The literal at 0x129ee0c
static const char* const s_desc = (const char*)0x129ee0c;

struct C3DMeshFileMaterialListChunk {
    void* vtbl;
    CCharString GetChunkDescription() const;
};

CCharString C3DMeshFileMaterialListChunk::GetChunkDescription() const {
    return CCharString(s_desc, -1);
}