#include <cstdio>

struct CHandleBasedMemoryPool {
    char pad[0x28];
    long m_index;      // +0x28
    long* m_refcounts; // +0x2c

    void* GetPointerFromHandle(long handle);
};

void* CHandleBasedMemoryPool::GetPointerFromHandle(long handle)
{
    --m_refcounts[m_index];
    m_index = handle;
    long* p = &m_refcounts[handle];
    ++(*p);
    return p;
}

int main()
{
    long counts[8] = {10, 20, 30, 40, 50, 60, 70, 80};
    CHandleBasedMemoryPool pool;
    pool.m_index = 2;      // currently pointing at slot 2 (value 30)
    pool.m_refcounts = counts;

    void* r = pool.GetPointerFromHandle(5);

    // slot 2 decremented: 30 -> 29
    // m_index becomes 5
    // slot 5 incremented: 60 -> 61
    // return &counts[5]
    bool ok = (counts[2] == 29) && (pool.m_index == 5) &&
              (counts[5] == 61) && (r == (void*)&counts[5]);

    if (ok)
        printf("PARITY_OK_dcec50\n");
    else
        printf("FAIL c2=%ld idx=%ld c5=%ld\n", counts[2], pool.m_index, counts[5]);
    return ok ? 0 : 1;
}