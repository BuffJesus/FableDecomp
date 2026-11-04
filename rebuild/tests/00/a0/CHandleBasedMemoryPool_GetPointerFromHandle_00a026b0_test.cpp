#include "fable_handle_memory_pool.h"
#include <stdio.h>
#include <string.h>
// Calls the linked candidate; no local implementation of the tested method.
int main() {
    union { double align; unsigned char bytes[sizeof(CHandleBasedMemoryPool)]; } storage;
    memset(&storage, 0xa5, sizeof(storage));
    CHandleBasedMemoryPool* pool = (CHandleBasedMemoryPool*)&storage;
    CHandleBasedMemoryPool::CHeader headers[4];
    CHandleBasedMemoryPool::CHandleInfo handles[4];
    pool->Handles._M_start = handles;
    pool->Handles._M_finish = handles + 4;
    pool->Handles._M_end_of_storage._M_data = handles + 4;
    for (int i=0; i<4; ++i) handles[i].Header = &headers[3-i];
    unsigned char before[sizeof(storage)];
    memcpy(before, &storage, sizeof(storage));

    for (int i = 0; i < 4; ++i)
        if (pool->GetPointerFromHandle(i) != (void*)(&headers[3-i] + 1)) return 1;
    if (memcmp(before, &storage, sizeof(storage))) return 2;
    printf("GPFH_00a026b0_OK\n");
    return 0;
}
