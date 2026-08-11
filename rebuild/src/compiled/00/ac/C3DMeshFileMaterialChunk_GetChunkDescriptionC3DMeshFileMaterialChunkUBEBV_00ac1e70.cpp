// C3DMeshFileMaterialChunk::GetChunkDescription
// retail 0x00ac1e70 : returns CCharString by value ("...const string literal...")

class CCharString {
public:
    // ctor takes (const char*, int) ; second defaults to -1 at call sites
    CCharString(const char* s, int n);
    CCharString(const CCharString& o);
    ~CCharString();
private:
    char* m_p;
};

// The string literal lives at 0x129ef0c in retail. Content is irrelevant to
// codegen shape; use a plausible chunk-description literal.
class C3DMeshFileMaterialChunk {
public:
    virtual CCharString GetChunkDescription() const;
};

CCharString C3DMeshFileMaterialChunk::GetChunkDescription() const
{
    return CCharString("MaterialChunk", -1);
}