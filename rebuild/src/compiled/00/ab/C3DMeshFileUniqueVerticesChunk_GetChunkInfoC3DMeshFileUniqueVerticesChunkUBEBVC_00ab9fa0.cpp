struct CCharString {
    CCharString(const char* lit, int len);
    int dummy;
};

struct C3DMeshFileUniqueVerticesChunk {
    CCharString GetChunkInfo() const;
};

CCharString C3DMeshFileUniqueVerticesChunk::GetChunkInfo() const
{
    return CCharString((const char*)0x122d70e, -1);
}