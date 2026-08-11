// C3DMeshFilePrimitiveChunk::GetChunkDescription
// Retail 0x00abee90 - virtual, returns CCharString by value from a string literal.

class CCharString {
public:
    // (const char*, int length = -1) constructor; out-of-line (matches call 0x99ebf0)
    CCharString(const char* s, int len);
    CCharString(const CCharString& o);
    ~CCharString();
private:
    char* m_data;
};

class C3DMeshFilePrimitiveChunk {
public:
    virtual CCharString GetChunkDescription() const;
};

CCharString C3DMeshFilePrimitiveChunk::GetChunkDescription() const {
    return CCharString("Primitive", -1);
}