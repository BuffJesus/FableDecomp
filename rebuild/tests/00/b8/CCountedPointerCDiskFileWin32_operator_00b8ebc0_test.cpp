#include <cstdio>

struct RefBlock
{
    int count;
    void (__fastcall *dtor)(void*);
    void* ptr;
};

struct CCountedPtr
{
    void* obj;
    RefBlock* blk;
};

CCountedPtr* __fastcall CCountedPtr_assign(CCountedPtr* self, int /*edx*/, CCountedPtr* rhs)
{
    void* rhsObj = rhs->obj;
    RefBlock* rhsBlk = rhs->blk;
    RefBlock* curBlk = self->blk;

    if (curBlk != rhsBlk)
    {
        if (curBlk != 0)
        {
            --curBlk->count;
            curBlk = self->blk;
            if (curBlk->count == 0)
            {
                curBlk->dtor(curBlk->ptr);
                curBlk = self->blk;
                ::operator delete(curBlk);
            }
        }

        self->obj = rhsObj;
        self->blk = rhsBlk;

        if (rhsBlk != 0)
        {
            ++rhsBlk->count;
        }
    }

    return self;
}

static int g_dtorCalls = 0;
static void* g_dtorArg = 0;

void __fastcall FakeDtor(void* p)
{
    g_dtorCalls++;
    g_dtorArg = p;
}

int main()
{
    // Case 1: this had a block whose count drops to zero -> dtor + delete fire,
    // then this takes rhs's obj/block and increments rhs's refcount.
    {
        RefBlock* oldBlk = new RefBlock();
        int oldObj = 0;
        oldBlk->count = 1;
        oldBlk->dtor = FakeDtor;
        oldBlk->ptr = &oldObj;

        RefBlock* newBlk = new RefBlock();
        int newObj = 0;
        newBlk->count = 3;
        newBlk->dtor = FakeDtor;
        newBlk->ptr = &newObj;

        CCountedPtr self;
        self.obj = &oldObj;
        self.blk = oldBlk;

        CCountedPtr rhs;
        rhs.obj = &newObj;
        rhs.blk = newBlk;

        g_dtorCalls = 0;
        g_dtorArg = 0;

        CCountedPtr* ret = CCountedPtr_assign(&self, 0, &rhs);

        if (ret != &self) { std::printf("FAIL case1 retval\n"); return 1; }
        if (g_dtorCalls != 1) { std::printf("FAIL case1 dtorCalls=%d\n", g_dtorCalls); return 1; }
        if (g_dtorArg != &oldObj) { std::printf("FAIL case1 dtorArg\n"); return 1; }
        if (self.obj != &newObj) { std::printf("FAIL case1 obj\n"); return 1; }
        if (self.blk != newBlk) { std::printf("FAIL case1 blk\n"); return 1; }
        if (newBlk->count != 4) { std::printf("FAIL case1 newcount=%d\n", newBlk->count); return 1; }

        delete newBlk;
    }

    // Case 2: this's old block count stays above zero -> no dtor call, no delete,
    // but still swaps to rhs's block and increments it.
    {
        RefBlock* oldBlk = new RefBlock();
        int oldObj = 0;
        oldBlk->count = 2;
        oldBlk->dtor = FakeDtor;
        oldBlk->ptr = &oldObj;

        RefBlock* newBlk = new RefBlock();
        int newObj = 0;
        newBlk->count = 1;
        newBlk->dtor = FakeDtor;
        newBlk->ptr = &newObj;

        CCountedPtr self;
        self.obj = &oldObj;
        self.blk = oldBlk;

        CCountedPtr rhs;
        rhs.obj = &newObj;
        rhs.blk = newBlk;

        g_dtorCalls = 0;

        CCountedPtr_assign(&self, 0, &rhs);

        if (g_dtorCalls != 0) { std::printf("FAIL case2 dtorCalls=%d\n", g_dtorCalls); return 1; }
        if (oldBlk->count != 1) { std::printf("FAIL case2 oldcount=%d\n", oldBlk->count); return 1; }
        if (self.blk != newBlk) { std::printf("FAIL case2 blk\n"); return 1; }
        if (newBlk->count != 2) { std::printf("FAIL case2 newcount=%d\n", newBlk->count); return 1; }

        delete oldBlk;
        delete newBlk;
    }

    // Case 3: this had a null block, rhs has a null block too (same block ==
    // no-op path) -> nothing changes, still returns this.
    {
        CCountedPtr self;
        self.obj = 0;
        self.blk = 0;

        CCountedPtr rhs;
        rhs.obj = 0;
        rhs.blk = 0;

        g_dtorCalls = 0;

        CCountedPtr* ret = CCountedPtr_assign(&self, 0, &rhs);

        if (ret != &self) { std::printf("FAIL case3 retval\n"); return 1; }
        if (g_dtorCalls != 0) { std::printf("FAIL case3 dtorCalls=%d\n", g_dtorCalls); return 1; }
        if (self.obj != 0 || self.blk != 0) { std::printf("FAIL case3 fields\n"); return 1; }
    }

    // Case 4: this had a null block, rhs has a real block -> just copies and
    // increments rhs's refcount (no dtor path taken).
    {
        RefBlock* newBlk = new RefBlock();
        int newObj = 0;
        newBlk->count = 5;
        newBlk->dtor = FakeDtor;
        newBlk->ptr = &newObj;

        CCountedPtr self;
        self.obj = 0;
        self.blk = 0;

        CCountedPtr rhs;
        rhs.obj = &newObj;
        rhs.blk = newBlk;

        g_dtorCalls = 0;

        CCountedPtr_assign(&self, 0, &rhs);

        if (g_dtorCalls != 0) { std::printf("FAIL case4 dtorCalls=%d\n", g_dtorCalls); return 1; }
        if (self.obj != &newObj || self.blk != newBlk) { std::printf("FAIL case4 fields\n"); return 1; }
        if (newBlk->count != 6) { std::printf("FAIL case4 count=%d\n", newBlk->count); return 1; }

        delete newBlk;
    }

    std::printf("PASS_009a9bf0\n");
    return 0;
}