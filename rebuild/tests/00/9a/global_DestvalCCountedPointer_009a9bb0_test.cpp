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

void __fastcall Dest_val_CCountedPointer(CCountedPtr* self, int /*edx*/)
{
    RefBlock* b = self->blk;
    if (b != 0) {
        --b->count;
        b = self->blk;
        if (b->count == 0) {
            b->dtor(b->ptr);
            b = self->blk;
            ::operator delete(b);
        }
    }
    self->obj = 0;
    self->blk = 0;
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
    // Case 1: refcount drops to zero -> dtor invoked, block deleted, fields nulled.
    {
        RefBlock* blk = new RefBlock();
        int dummyObj = 0;
        blk->count = 1;
        blk->dtor = FakeDtor;
        blk->ptr = &dummyObj;

        CCountedPtr cp;
        cp.obj = &dummyObj;
        cp.blk = blk;

        g_dtorCalls = 0;
        g_dtorArg = 0;

        Dest_val_CCountedPointer(&cp, 0);

        if (g_dtorCalls != 1) { std::printf("FAIL case1 dtorCalls=%d\n", g_dtorCalls); return 1; }
        if (g_dtorArg != &dummyObj) { std::printf("FAIL case1 dtorArg\n"); return 1; }
        if (cp.obj != 0 || cp.blk != 0) { std::printf("FAIL case1 fields not null\n"); return 1; }
    }

    // Case 2: refcount stays above zero -> no dtor call, no delete, fields still nulled.
    {
        RefBlock* blk = new RefBlock();
        int dummyObj = 0;
        blk->count = 2;
        blk->dtor = FakeDtor;
        blk->ptr = &dummyObj;

        CCountedPtr cp;
        cp.obj = &dummyObj;
        cp.blk = blk;

        g_dtorCalls = 0;
        g_dtorArg = 0;

        Dest_val_CCountedPointer(&cp, 0);

        if (g_dtorCalls != 0) { std::printf("FAIL case2 dtorCalls=%d\n", g_dtorCalls); return 1; }
        if (blk->count != 1) { std::printf("FAIL case2 count=%d\n", blk->count); return 1; }
        if (cp.obj != 0 || cp.blk != 0) { std::printf("FAIL case2 fields not null\n"); return 1; }

        delete blk;
    }

    // Case 3: null block -> no crash, fields nulled, no dtor.
    {
        CCountedPtr cp;
        cp.obj = 0;
        cp.blk = 0;

        g_dtorCalls = 0;

        Dest_val_CCountedPointer(&cp, 0);

        if (g_dtorCalls != 0) { std::printf("FAIL case3 dtorCalls=%d\n", g_dtorCalls); return 1; }
        if (cp.obj != 0 || cp.blk != 0) { std::printf("FAIL case3 fields not null\n"); return 1; }
    }

    std::printf("PASS_009a9bb0\n");
    return 0;
}