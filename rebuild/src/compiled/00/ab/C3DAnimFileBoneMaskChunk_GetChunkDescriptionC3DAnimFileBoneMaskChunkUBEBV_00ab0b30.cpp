// C3DAnimFileBoneMaskChunk::GetChunkDescription
// Retail 0x00ab0b30 : struct-return __fastcall virtual method.
// Builds a CCharString from a literal (const char*, -1) into the hidden
// return buffer and returns it by value.

class CCharString {
public:
    // ctor(const char*, int) at 0x0099ebf0
    CCharString(const char* s, int n);
private:
    char* m_data;
};

// The string literal lives at 0x0129e9ac in the retail image.
static const char* const kDesc = (const char*)0x0129e9ac;

class C3DAnimFileBoneMaskChunk {
public:
    virtual const CCharString GetChunkDescription() const;
};

const CCharString C3DAnimFileBoneMaskChunk::GetChunkDescription() const
{
    return CCharString(kDesc, -1);
}