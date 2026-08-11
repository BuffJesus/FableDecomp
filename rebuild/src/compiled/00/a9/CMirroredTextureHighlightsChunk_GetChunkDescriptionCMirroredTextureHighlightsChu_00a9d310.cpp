// CMirroredTextureHighlightsChunk::GetChunkDescription  (retail 0x00a9d310)
// __fastcall, returns CCharString by value (hidden retbuf as first stack arg).
// Body: return CCharString("...", -1);  -- 'this' unused.

class CCharString {
public:
    CCharString(const char *s, long n);   // 0x0099ebf0, out-of-line ctor
private:
    char *m_p;
};

class CMirroredTextureHighlightsChunk {
public:
    CCharString GetChunkDescription();
    int m_dummy;
};

CCharString CMirroredTextureHighlightsChunk::GetChunkDescription()
{
    return CCharString("MirroredTextureHighlights", -1);
}