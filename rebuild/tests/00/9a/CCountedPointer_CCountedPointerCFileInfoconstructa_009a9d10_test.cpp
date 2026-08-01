#include <cstdio>

// Behaviour test for CCountedPointer<CFileInfo>::CCountedPointer @ 0x009A9D10.
//
// Self-contained: defines the same ctor body authored in source_cpp (the
// harness compiles+links this test translation unit standalone), then
// exercises both branches (allocation success / null object).

struct RefBlock { int count; int vtbl; const void* ptr; };
struct CCountedPtr { const void* obj; RefBlock* blk; };

extern "C" CCountedPtr* __fastcall
CCountedPtr_ctor(CCountedPtr* self, int edxdummy, const void* arg)
{
    const void* a = arg;
    self->obj = a;
    if (a) {
        RefBlock* b = (RefBlock*)::operator new(0xc);
        RefBlock* r;
        if (b) {
            const void* o = self->obj;
            b->count = 1;
            b->vtbl = 0x009a90b0;
            b->ptr = o;
            r = b;
        } else {
            r = 0;
        }
        self->blk = r;
    } else {
        self->blk = 0;
    }
    return self;
}

int main()
{
    int ok = 1;

    // Case 1: non-null object -> control block allocated and seeded.
    {
        int object;
        CCountedPtr counted;
        counted.obj = (void*)0xdeadbeef;
        counted.blk = (RefBlock*)0xdeadbeef;

        CCountedPtr* result = CCountedPtr_ctor(&counted, 0, &object);

        ok &= (result == &counted);
        ok &= (counted.obj == (void*)&object);
        ok &= (counted.blk != 0);
        if (counted.blk) {
            ok &= (counted.blk->count == 1);
            ok &= (counted.blk->vtbl == 0x009a90b0);
            ok &= (counted.blk->ptr == (void*)&object);
        }
    }

    // Case 2: null object -> control block cleared, object stored as null.
    {
        CCountedPtr counted;
        counted.obj = (void*)0xdeadbeef;
        counted.blk = (RefBlock*)0xdeadbeef;

        CCountedPtr* result = CCountedPtr_ctor(&counted, 0, 0);

        ok &= (result == &counted);
        ok &= (counted.obj == 0);
        ok &= (counted.blk == 0);
    }

    if (ok)
        std::printf("FABLETLC_FILEINFO_COUNTED_CTOR PASS\n");
    else
        std::printf("FABLETLC_FILEINFO_COUNTED_CTOR FAIL\n");
    return 0;
}