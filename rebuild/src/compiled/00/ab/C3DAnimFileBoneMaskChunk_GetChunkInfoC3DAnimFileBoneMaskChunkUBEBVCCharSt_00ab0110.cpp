// C3DAnimFileBoneMaskChunk::GetChunkInfo  (retail 0x00ab0110)
// Virtual const member returning CCharString by value.
// Body: return CCharString("BoneMask");  (a fixed fourcc/name literal)

class CCharString {
public:
    CCharString(const char* s, int len = -1);
    char* m_p;
};

class C3DAnimFileBoneMaskChunk {
public:
    virtual CCharString GetChunkInfo() const;
};

CCharString C3DAnimFileBoneMaskChunk::GetChunkInfo() const
{
    return CCharString("BoneMask");
}