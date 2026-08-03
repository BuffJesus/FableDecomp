#include <cstdio>
#include <new>

typedef void (__fastcall *DestroyFn)(void*);

struct RefBlock { int owners; DestroyFn destroy; void* obj; };
struct CCountedPtr { void* obj; RefBlock* blk; };

static int   g_destroyCalls = 0;
static void* g_destroyedObj = 0;

static void __fastcall ObjectDestroyThunk(void* obj)
{
    ++g_destroyCalls;
    g_destroyedObj = obj;
}

// Behavioural reconstruction of CCountedPointer<...>::Reset @ 0x00444130,
// mirroring the release-then-install semantics of the byte-matched source.
void __fastcall
CCountedPtr_Reset(CCountedPtr* self, int edxdummy, void* arg)
{
    RefBlock* old = self->blk;
    if (old) {
        --old->owners;
        if (self->blk->owners == 0) {
            RefBlock* b = self->blk;
            b->destroy(b->obj);
            ::operator delete(self->blk);
        }
    }

    void* a = arg;
    self->blk = 0;
    self->obj = a;
    if (a) {
        RefBlock* b = (RefBlock*)::operator new(0xc);
        RefBlock* r;
        if (b) {
            void* o = self->obj;
            b->owners = 1;
            b->destroy = (DestroyFn)0x443540;
            b->obj = o;
            r = b;
        } else {
            r = 0;
        }
        self->blk = r;
    }
}

static bool CheckSharedOwnerReplacement()
{
    g_destroyCalls = 0;
    g_destroyedObj = 0;
    int oldObj;
    int newObj;
    RefBlock oldBlk = { 2, &ObjectDestroyThunk, &oldObj };
    CCountedPtr cp = { &oldObj, &oldBlk };

    CCountedPtr_Reset(&cp, 0, &newObj);

    return oldBlk.owners == 1 &&
           g_destroyCalls == 0 &&
           cp.obj == &newObj &&
           cp.blk != 0 &&
           cp.blk->owners == 1 &&
           cp.blk->obj == &newObj &&
           cp.blk->destroy == (DestroyFn)0x443540;
}

static bool CheckLastOwnerRelease()
{
    g_destroyCalls = 0;
    g_destroyedObj = 0;
    int oldObj;
    RefBlock* oldBlk = (RefBlock*)::operator new(sizeof(RefBlock));
    oldBlk->owners = 1;
    oldBlk->destroy = &ObjectDestroyThunk;
    oldBlk->obj = &oldObj;
    CCountedPtr cp = { &oldObj, oldBlk };

    CCountedPtr_Reset(&cp, 0, 0);

    return g_destroyCalls == 1 &&
           g_destroyedObj == &oldObj &&
           cp.obj == 0 &&
           cp.blk == 0;
}

int main()
{
    bool ok = CheckSharedOwnerReplacement() && CheckLastOwnerRelease();
    if (ok)
        std::printf("FABLETLC_PHYSICS_FACES_COUNTED_RESET_444130 PASS\n");
    else
        std::printf("FABLETLC_PHYSICS_FACES_COUNTED_RESET_444130 FAIL\n");
    return 0;
}