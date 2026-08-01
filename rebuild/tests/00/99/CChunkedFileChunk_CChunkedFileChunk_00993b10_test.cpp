#include <cstdio>
#include <cstring>

struct CBase
{
    void** vtable;
    void RestoreVTable();
};

void* g_CBaseVTable_01231710[4] = { (void*)0x1111, (void*)0x2222, (void*)0x3333, (void*)0x4444 };

void CBase::RestoreVTable()
{
    vtable = g_CBaseVTable_01231710;
}

void* g_CChunkedFileChunkVTable_0129a69c[4] = { (void*)0x5555, (void*)0x6666, (void*)0x7777, (void*)0x8888 };

struct CChunkedFileChunk : CBase
{
    ~CChunkedFileChunk();
};

CChunkedFileChunk::~CChunkedFileChunk()
{
    vtable = g_CChunkedFileChunkVTable_0129a69c;
    RestoreVTable();
}

int main()
{
    CChunkedFileChunk obj;
    obj.vtable = (void**)0;

    obj.~CChunkedFileChunk();

    if (obj.vtable == g_CBaseVTable_01231710)
    {
        printf("marker_dtor_ok\n");
        return 0;
    }

    printf("marker_dtor_fail\n");
    return 1;
}