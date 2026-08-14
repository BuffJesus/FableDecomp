#include <cstdio>

// Independent behaviour model of ~CMemoryBlock @ 0x009fc550:
//   - write derived vtable at entry
//   - if the +8 member pointer is non-null, invoke its Free(self)
//   - write base vtable at end
// Verifies the conditional-cleanup gate and the final vtable state.

static int g_freed = 0;

struct CMemoryBlock;
struct CMemoryBlockOwner {
    void Free(CMemoryBlock* self);
};

struct CMemoryBlock {
    unsigned int      vptr;
    int               pad4;
    CMemoryBlockOwner* member8;
};

void CMemoryBlockOwner::Free(CMemoryBlock* self) { g_freed++; (void)self; }

static const unsigned int kA = 0x012354a4u;
static const unsigned int kB = 0x012354d0u;

static void dtor(CMemoryBlock* self) {
    self->vptr = kA;
    if (self->member8 != 0) {
        self->member8->Free(self);
    }
    self->vptr = kB;
}

int main() {
    CMemoryBlockOwner owner;

    // member8 non-null -> Free runs once.
    CMemoryBlock a; a.vptr = 0; a.pad4 = 0; a.member8 = &owner;
    dtor(&a);

    // member8 null -> Free skipped.
    CMemoryBlock b; b.vptr = 0; b.pad4 = 0; b.member8 = 0;
    dtor(&b);

    if (g_freed == 1 && a.vptr == kB && b.vptr == kB) {
        printf("PARITY_OK\n");
    } else {
        printf("FAIL g=%d a=%x b=%x\n", g_freed, a.vptr, b.vptr);
    }
    return 0;
}