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

    const unsigned char locks[] = { 0, 1, 2, 255 };
    const long counts[] = { -10, 0, 4, 0x7fffffff };
    for (int selected = 0; selected < 4; ++selected)
    for (int flag = 0; flag < 4; ++flag)
    for (int count = 0; count < 4; ++count) {
        pool->LockCount = counts[count];
        for (int i=0; i<4; ++i) headers[i].Locked = 0x6b;
        headers[3-selected].Locked = locks[flag];
        unsigned char expectedPool[sizeof(storage)], expectedHeaders[sizeof(headers)];
        memcpy(expectedPool, &storage, sizeof(storage));
        memcpy(expectedHeaders, headers, sizeof(headers));
        long expectedCount = counts[count] - (locks[flag] ? 1 : 0);
        memcpy(expectedPool+offsetof(CHandleBasedMemoryPool,LockCount), &expectedCount, sizeof(expectedCount));
        expectedHeaders[(3-selected)*sizeof(headers[0])+offsetof(CHandleBasedMemoryPool::CHeader,Locked)] = 0;
        for (int repeat=0; repeat<2; ++repeat) {
            pool->Unlock(selected);
            if (memcmp(expectedPool, &storage, sizeof(storage)) || memcmp(expectedHeaders, headers, sizeof(headers))) return 1;
        }
    }
    printf("UNLOCK_OK\n");
    return 0;
}
