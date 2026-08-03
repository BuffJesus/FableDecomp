#include <cstdio>

// Behaviour test for CCountedPointer<CProgressDisplay>::CCountedPointer @ 0x00403D20.
//
// The target function is defined in source_cpp as
//   extern "C" CCountedPtr* __fastcall
//   CCountedPtr_ctor(CCountedPtr* self, int edxdummy, void* arg);
// We DECLARE it here with the identical calling convention + signature so the
// C linkage / fastcall decoration matches (@CCountedPtr_ctor@12) and the
// fallback link (source.obj + test.obj) resolves it. We do NOT redefine it.
//
// The only external callee source_cpp references is ::operator new, resolved
// from the CRT at link time, so no masked-callee stub is required here.
// The 0x403600 destroy-thunk value is a plain literal store (never called).

struct RefBlock { int count; int vtbl; void* ptr; };
struct CCountedPtr { void* obj; RefBlock* blk; };

extern "C" CCountedPtr* __fastcall
CCountedPtr_ctor(CCountedPtr* self, int edxdummy, void* arg);

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
            ok &= (counted.blk->vtbl == 0x403600);
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
        std::printf("FABLETLC_PROGRESS_DISPLAY_COUNTED_CTOR PASS\n");
    else
        std::printf("FABLETLC_PROGRESS_DISPLAY_COUNTED_CTOR FAIL\n");
    return 0;
}