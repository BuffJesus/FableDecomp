#include "fable_handle_memory_pool.h"
#include <stdio.h>
#include <string.h>
int main()
{
    const unsigned long sizes[] = { 0, 1, 16, 0x7fffffff, 0x80000000, 0xffffffff };
    union { double align; unsigned char bytes[sizeof(CHandleBasedMemoryPool)]; } storage;
    memset(&storage, 0xa5, sizeof(storage));
    CHandleBasedMemoryPool* pool = (CHandleBasedMemoryPool*)&storage;
    CHandleBasedMemoryPool::CHeader headers[4];
    CHandleBasedMemoryPool::CHandleInfo handles[4];
    pool->Handles._M_start = handles;
    for (int i=0; i<4; ++i) handles[i].Header = &headers[3-i];
    unsigned char beforePool[sizeof(storage)], beforeHeaders[sizeof(headers)];
    memcpy(beforePool, &storage, sizeof(storage));
    for (int v=0; v<6; ++v)
    {
        for (int i=0; i<4; ++i)
        {
            headers[i].Size = sizes[(v+i)%6];
            headers[i].Padding = 31;
        }
        memcpy(beforeHeaders, headers, sizeof(headers));
        for (int i=0; i<4; ++i)
            if (pool->GetSizeFromHandle(i) != sizes[(v+3-i)%6]) return 1;
        if (memcmp(beforePool, &storage, sizeof(storage)) || memcmp(beforeHeaders, headers, sizeof(headers))) return 2;
    }
    printf("POOL_SIZE_FROM_HANDLE_OK\n");
    return 0;
}
