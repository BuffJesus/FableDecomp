// C3DMeshFileTransformationChunk::GetChunkDescription
// Retail 0x00abd3e0 - virtual, returns CCharString by value from a string literal.

class CCharString {
public:
    // (const char*, int length = -1) constructor; out-of-line (matches call 0x99ebf0)
    CCharString(const char* s, int len);
    CCharString(const CCharString& o);
    ~CCharString();
private:
    char* m_data;
};

class C3DMeshFileTransformationChunk {
public:
    virtual CCharString GetChunkDescription() const;
};

CCharString C3DMeshFileTransformationChunk::GetChunkDescription() const {
    return CCharString("Transformation", -1);
}