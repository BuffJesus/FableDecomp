// C3DMeshFileMaterialExtChunk::GetChunkDescription  (retail 0x00ac1920)
// virtual CCharString GetChunkDescription() const;
// Body: constructs a CCharString from a string literal with explicit length -1,
// returning it by value into the hidden struct-return buffer.

class CCharString {
public:
    // ctor from (const char*, int length)  -> retail 0x0099ebf0
    CCharString(const char* s, int len);
    CCharString(const CCharString& o);
    ~CCharString();
private:
    char* m_pData;
    int   m_nLength;
    int   m_nAlloc;
};

class C3DMeshFileMaterialExtChunk {
public:
    virtual CCharString GetChunkDescription() const;
    // enough padding virtuals not required; single virtual is fine for body parity
};

CCharString C3DMeshFileMaterialExtChunk::GetChunkDescription() const
{
    return CCharString("MaterialExt", -1);
}