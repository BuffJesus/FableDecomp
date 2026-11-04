#include "fable_handle_memory_pool.h"
#include <new>
#include <stdio.h>
#include <string.h>

// Only Free is a boundary: every ownership operation comes from the header.
static CScopedMemoryHandle* observed;
static CHandleBasedMemoryPool* expectedPool;
static long expectedHandle;
static int calls, failed;
void CHandleBasedMemoryPool::Free(long handle)
{
    if (this != expectedPool || handle != expectedHandle) failed = 1;
    // Ownership must still refer to the old allocation during its release.
    if (!observed || observed->Pool != this || observed->Handle != handle) failed = 2;
    ++calls;
}

int main()
{
    const long values[] = { -1, 0, 1, 123, (long)0x80000000, 0x7fffffff };
    CHandleBasedMemoryPool* a = (CHandleBasedMemoryPool*)0x12340000;
    CHandleBasedMemoryPool* b = (CHandleBasedMemoryPool*)0x56780000;
    int cases = 0;
    for (int i = 0; i < 6; ++i)
    for (int j = 0; j < 6; ++j)
    {
        union { double align; unsigned char bytes[8]; } storage;
        memset(&storage, 0xa5, sizeof(storage));
        CScopedMemoryHandle* handle = new (&storage) CScopedMemoryHandle;
        observed = handle; calls = failed = 0;
        if (handle->Pool || handle->Handle != -1) return 1;
        handle->Clear();
        if (calls || handle->Pool || handle->Handle != -1) return 2;
        handle->Initialise(a, values[i]);
        if (calls || handle->Pool != a || handle->Handle != values[i]) return 3;
        expectedPool = a; expectedHandle = values[i];
        handle->Initialise(b, values[j]);
        if (calls != 1 || failed || handle->Pool != b || handle->Handle != values[j]) return 4;
        expectedPool = b; expectedHandle = values[j];
        handle->~CScopedMemoryHandle();
        if (calls != 2 || failed) return 5;

        handle = new (&storage) CScopedMemoryHandle;
        observed = handle; calls = failed = 0;
        handle->Initialise(a, values[i]);
        expectedPool = a; expectedHandle = values[i];
        handle->Clear();
        if (calls != 1 || failed || handle->Pool || handle->Handle != -1) return 6;
        handle->Clear();
        handle->~CScopedMemoryHandle();
        if (calls != 1 || failed) return 7;

        handle = new (&storage) CScopedMemoryHandle;
        observed = handle; calls = failed = 0;
        handle->Initialise(0, values[i]);
        handle->Clear();
        if (calls || handle->Pool || handle->Handle != values[i]) return 8;
        handle->~CScopedMemoryHandle();
        if (calls || failed) return 9;

        // Automatic destruction must release the live allocation once.
        calls = failed = 0;
        {
            CScopedMemoryHandle automatic;
            observed = &automatic;
            automatic.Initialise(a, values[i]);
            expectedPool = a; expectedHandle = values[i];
        }
        if (calls != 1 || failed) return 10;
        ++cases;
    }
    printf("SCOPED_MEMORY_HANDLE_PASS scenarios=%d\n", cases);
    return 0;
}
