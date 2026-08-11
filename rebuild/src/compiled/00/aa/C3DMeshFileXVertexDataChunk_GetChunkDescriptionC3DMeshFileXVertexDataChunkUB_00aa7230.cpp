// C3DMeshFileXVertexDataChunk::GetChunkDescription (const, returns CCharString by value)
// retail 0x00aa7230: build CCharString from literal @0x129e368 with len -1.

struct CCharString {
    char* m_data;
    CCharString(const char* s, int len);
};

// ctor @0x99ebf0
__declspec(noinline) CCharString::CCharString(const char* s, int len) {
    m_data = (char*)s;
    if (len) m_data = (char*)s + 1;
}

struct C3DMeshFileXVertexDataChunk {
    virtual CCharString GetChunkDescription() const;
};

CCharString C3DMeshFileXVertexDataChunk::GetChunkDescription() const {
    return CCharString((const char*)0x129e368, -1);
}