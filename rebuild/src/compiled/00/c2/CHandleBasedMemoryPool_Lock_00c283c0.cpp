struct Entry {
    char pad[0xf];
    unsigned char flag;   // +0xf
};

struct CHandleBasedMemoryPool {
    char pad0[0x18];
    unsigned long lockCount;   // +0x18
    char pad1[0x440 - 0x18 - 4];
    Entry** table;             // +0x440
};

void __fastcall Lock(CHandleBasedMemoryPool* self, int /*edx*/, long index)
{
    Entry** base = self->table;
    Entry* volatile* slot = base + index;
    Entry* e = *slot;
    if (e->flag == 0) {
        e->flag = 1;
        self->lockCount++;
    }
}