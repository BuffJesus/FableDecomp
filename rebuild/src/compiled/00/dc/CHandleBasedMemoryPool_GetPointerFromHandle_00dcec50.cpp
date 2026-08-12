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