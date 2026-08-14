// CHandleBasedMemoryPool::Unlock  0x00a02710
// __fastcall void Unlock(long index)
// self+0x438 = pointer to array of Entry*; index*4 into it -> Entry*
// Entry+0xf = "locked" byte flag; if set, clear it and decrement self+0x18 (locked count)

struct Entry {
    char pad[0xf];
    unsigned char locked;   // +0xf
};

struct CHandleBasedMemoryPool {
    char pad0[0x18];
    int lockedCount;        // +0x18
    char pad1[0x438 - 0x1c];
    Entry** table;          // +0x438
};

// Model __fastcall: this in ecx, index on stack.
// Two leading register params (ecx=self, edx=dummy) force `index` onto the stack,
// and returning a stdcall-cleaned frame via the extra stack arg yields `ret 4`.
void __fastcall Unlock(CHandleBasedMemoryPool* self, int edxDummy, long index)
{
    Entry* e = self->table[index];
    if (e->locked) {
        e->locked = 0;
        self->lockedCount--;
    }
}