struct CBase
{
    void** vtable;
    void RestoreVTable();
};

extern void* g_CChunkedFileChunkVTable_0129a69c[];

struct CChunkedFileChunk : CBase
{
    ~CChunkedFileChunk();
};

CChunkedFileChunk::~CChunkedFileChunk()
{
    vtable = g_CChunkedFileChunkVTable_0129a69c;
    RestoreVTable();
}