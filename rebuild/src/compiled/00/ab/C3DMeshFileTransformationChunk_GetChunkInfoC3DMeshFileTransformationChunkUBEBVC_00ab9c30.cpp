struct CCharString {
    CCharString(const char* lit, int len);
    int dummy;
};

struct C3DMeshFileTransformationChunk {
    CCharString GetChunkInfo() const;
};

CCharString C3DMeshFileTransformationChunk::GetChunkInfo() const
{
    return CCharString((const char*)0x122d70e, -1);
}