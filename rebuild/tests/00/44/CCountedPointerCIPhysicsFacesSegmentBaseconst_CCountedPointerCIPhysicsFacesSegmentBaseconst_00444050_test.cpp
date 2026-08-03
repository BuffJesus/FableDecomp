#include <cstdio>
#include <cstring>
#include <new>

// Behaviour harness for
// CCountedPointer<CIPhysicsFacesSegmentBase const>::Reset @ 0x00444050.
//
// The genuine reconstruction (source_cpp) is compiled to source.obj and
// defines the target as extern "C" __fastcall CCountedPtr_Reset -> the
// decorated symbol @CCountedPtr_Reset@12. This test declares it with the
// IDENTICAL calling convention and signature (so the mangling matches and the
// verifier's source.obj + test.obj fallback link resolves it) and never
// redefines it.
//
// source.obj's only undefined externals are the masked allocator pair
//   ??2@YAPAXI@Z  operator new(unsigned int)
//   ??3@YAXPAX@Z  operator delete(void*)
// so the test DEFINES those two global operators (no duplicate target, no
// duplicate operators -- the source only references them). The 0x00443500
// destroy-thunk literal is only stored, never dereferenced by Reset, so the
// test seeds its own destroy stub into the control blocks it builds.

typedef void (__fastcall *DestroyFn)(void* object);

struct RefBlock { int owners; DestroyFn destroy; void* object; };
struct CCountedPtr { void* obj; RefBlock* blk; };

extern "C" void __fastcall
CCountedPtr_Reset(CCountedPtr* self, int edxdummy, void* arg);

// --- observation state ---------------------------------------------------
static unsigned g_newCalls = 0;
static unsigned g_delCalls = 0;
static unsigned g_destroyCalls = 0;
static void*    g_lastDeleted = 0;
static void*    g_lastDestroyed = 0;
static bool     g_failAlloc = false;
static RefBlock g_block;

void __fastcall TestDestroy(void* object)
{
    ++g_destroyCalls;
    g_lastDestroyed = object;
}

// operator new(unsigned int) -> ??2@YAPAXI@Z, referenced by source.obj.
void* operator new(size_t sz)
{
    ++g_newCalls;
    if (g_failAlloc || sz != sizeof(RefBlock)) return 0;
    void* p = static_cast<void*>(&g_block);
    std::memset(p, 0, sizeof(g_block));
    return p;
}

// operator delete(void*) -> ??3@YAXPAX@Z, referenced by source.obj.
void operator delete(void* p)
{
    ++g_delCalls;
    g_lastDeleted = p;
}

static void ResetCounters()
{
    g_newCalls = g_delCalls = g_destroyCalls = 0;
    g_lastDeleted = g_lastDestroyed = 0;
    g_failAlloc = false;
    std::memset(&g_block, 0, sizeof(g_block));
}

// Held block is shared (owners > 1): the release only decrements; the new
// object gets a freshly allocated, seeded control block.
static bool CheckSharedRelease()
{
    ResetCounters();
    int oldObj, newObj;
    RefBlock old = { 2, &TestDestroy, &oldObj };
    CCountedPtr cp = { &oldObj, &old };

    CCountedPtr_Reset(&cp, 0, &newObj);

    return old.owners == 1 &&
           g_destroyCalls == 0 && g_delCalls == 0 &&
           g_newCalls == 1 &&
           cp.obj == &newObj && cp.blk == &g_block &&
           g_block.owners == 1 && g_block.object == &newObj &&
           g_block.destroy == (DestroyFn)0x443500;
}

// Held block is sole owner (owners == 1) and the new object is null: the
// release destroys+deletes and nothing is reinstalled.
static bool CheckLastOwnerRelease()
{
    ResetCounters();
    int oldObj;
    RefBlock old = { 1, &TestDestroy, &oldObj };
    CCountedPtr cp = { &oldObj, &old };

    CCountedPtr_Reset(&cp, 0, 0);

    return old.owners == 0 &&
           g_destroyCalls == 1 && g_lastDestroyed == &oldObj &&
           g_delCalls == 1 && g_lastDeleted == &old &&
           g_newCalls == 0 &&
           cp.obj == 0 && cp.blk == 0;
}

// No held block; allocation fails: obj is still stored, blk is cleared to 0.
static bool CheckAllocFailure()
{
    ResetCounters();
    g_failAlloc = true;
    int obj;
    CCountedPtr cp = { 0, 0 };

    CCountedPtr_Reset(&cp, 0, &obj);

    return g_newCalls == 1 && cp.obj == &obj && cp.blk == 0;
}

int main()
{
    bool ok = CheckSharedRelease() &&
              CheckLastOwnerRelease() &&
              CheckAllocFailure();
    if (ok)
        std::printf("FABLETLC_PHYSICS_FACES_COUNTED_RESET_00444050 PASS\n");
    else
        std::printf("FABLETLC_PHYSICS_FACES_COUNTED_RESET_00444050 FAIL\n");
    return ok ? 0 : 1;
}