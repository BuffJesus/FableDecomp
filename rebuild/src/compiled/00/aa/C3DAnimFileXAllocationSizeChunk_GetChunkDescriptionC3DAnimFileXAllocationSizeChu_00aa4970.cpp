// C3DAnimFileXAllocationSizeChunk::GetChunkDescription
// retail 0x00aa4970 : returns CCharString by value (hidden return buffer),
// constructs it from a string literal with length -1.

struct CCharString {
    char* m_data;
    // ctor(const char*, int) lives at retail 0x99ebf0
    CCharString(const char* s, int n);
};

struct C3DAnimFileXAllocationSizeChunk {
    void* vptr;
    virtual CCharString GetChunkDescription() const;
};

CCharString C3DAnimFileXAllocationSizeChunk::GetChunkDescription() const {
    return CCharString("AllocationSize", -1);
}