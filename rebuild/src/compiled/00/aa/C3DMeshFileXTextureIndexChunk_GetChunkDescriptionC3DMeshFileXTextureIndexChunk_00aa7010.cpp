// C3DMeshFileXTextureIndexChunk::GetChunkDescription (const, virtual)
// retail 0x00aa7010: returns CCharString built from a fixed string literal.

class CCharString {
public:
    char* m_data;
    CCharString(const char* s, long n);
};

// The real engine ctor lives at 0x0099ebf0 (CCharString::CCharString(char const*, long)).
// Model it as an out-of-line ctor so VC7.1 emits push n; push str; mov ecx,this; call ctor.

class C3DMeshFileXTextureIndexChunk {
public:
    virtual CCharString GetChunkDescription() const;
};

CCharString C3DMeshFileXTextureIndexChunk::GetChunkDescription() const
{
    return CCharString("XTextureIndex", -1);
}