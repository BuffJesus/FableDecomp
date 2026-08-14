struct PoolEntry {
    char pad[0xf];
    unsigned char locked;   // +0xf
};

struct CHandleBasedMemoryPool {
    char pad0[0x18];
    unsigned long lockCount;    // +0x18
    char pad1[0x438 - 0x18 - 4];
    PoolEntry** entries;        // +0x438
};

void __fastcall Lock(CHandleBasedMemoryPool* self, int edxDummy, long handle)
{
    PoolEntry* e = self->entries[handle];
    if (e->locked == 0) {
        e->locked = 1;
        self->lockCount++;
    }
}