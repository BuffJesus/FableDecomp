// C3DMeshFileXTriangleStripChunk::GetChunkDescription -- retail 0x00aa7280
// Returns a CCharString by value built from a fixed literal with length -1.

class CCharString {
public:
    CCharString(const char* s, int n);   // out-of-line ctor (retail 0x99ebf0)
private:
    char* m_p;
    int   m_len;
    int   m_cap;
};

class C3DMeshFileXTriangleStripChunk {
public:
    virtual CCharString GetChunkDescription() const;
};

CCharString C3DMeshFileXTriangleStripChunk::GetChunkDescription() const
{
    return CCharString((const char*)0x129e3ac, -1);
}