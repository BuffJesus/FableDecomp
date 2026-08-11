struct CCharString {
    CCharString(const char* lit, int len);
    int dummy;
};

struct CChunkedFileChunk {
    CCharString GetChunkInfo() const;
};

CCharString CChunkedFileChunk::GetChunkInfo() const
{
    return CCharString((const char*)0x122d70e, -1);
}