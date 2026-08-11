// C3DMeshFileXMappingInfoChunk::GetChunkDescription
// Returns a CCharString by value describing the chunk.

class CCharString {
public:
    CCharString(const char* s, int len);
    char* m_data;
    int   m_len;
};

class C3DMeshFileXMappingInfoChunk {
public:
    virtual CCharString GetChunkDescription() const;
    void* vtbl_pad;
};

CCharString C3DMeshFileXMappingInfoChunk::GetChunkDescription() const
{
    return CCharString("Mapping Information", -1);
}