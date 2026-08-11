// C3DMeshFileTrianglesChunk::GetChunkDescription
// retail 0x00abec90 : returns CCharString by value (hidden return buffer),
// constructs it from a string literal (0x129ed60) with length -1.

struct CCharString {
    char* m_data;
    // ctor(const char*, int) lives at retail 0x99ebf0
    CCharString(const char* s, int n);
};

struct C3DMeshFileTrianglesChunk {
    void* vptr;
    virtual CCharString GetChunkDescription() const;
};

CCharString C3DMeshFileTrianglesChunk::GetChunkDescription() const {
    return CCharString("Triangles", -1);
}