// C3DMeshFileXBoundingSphereChunk::GetChunkDescription  (retail 0x00aa7100)
// virtual CCharString GetChunkDescription() const;  -- returns a CCharString by
// value built from a fixed string literal via CCharString::CCharString(char const*, int).

class CCharString {
public:
    // ctor at 0x0099ebf0 : CCharString(char const* s, int n)
    CCharString(const char* s, int n);
private:
    char* m_p;
};

class C3DMeshFileXBoundingSphereChunk {
public:
    virtual CCharString GetChunkDescription() const;
};

CCharString C3DMeshFileXBoundingSphereChunk::GetChunkDescription() const
{
    return CCharString("Bounding Sphere", -1);
}