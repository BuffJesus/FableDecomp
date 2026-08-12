#include <stdio.h>
#include <stddef.h>
#include <new>

struct CLandscapeBackgroundPatch
{
    int marker;
    void* VectorDeletingDtor(unsigned int flags);
};

static int g_dtorCalls = 0;
static int g_deleteCalls = 0;

extern void __fastcall LandscapeBackgroundPatch_ScalarDtor(CLandscapeBackgroundPatch* self)
{
    if (self) { self->marker = 0xDEAD; }
    g_dtorCalls++;
}

// Track operator delete without actually freeing static storage.
void operator delete(void* p) throw()
{
    g_deleteCalls++;
    (void)p;
}

void* CLandscapeBackgroundPatch::VectorDeletingDtor(unsigned int flags)
{
    LandscapeBackgroundPatch_ScalarDtor(this);
    if (flags & 1)
        ::operator delete(this);
    return this;
}

int main()
{
    CLandscapeBackgroundPatch a; a.marker = 1;
    CLandscapeBackgroundPatch b; b.marker = 2;

    // flags bit0 clear: destructor runs, no delete
    g_dtorCalls = 0; g_deleteCalls = 0;
    void* r1 = a.VectorDeletingDtor(0);
    int ok1 = (r1 == &a) && (g_dtorCalls == 1) && (g_deleteCalls == 0) && (a.marker == 0xDEAD);

    // flags bit0 set: destructor runs, delete called once
    g_dtorCalls = 0; g_deleteCalls = 0;
    void* r2 = b.VectorDeletingDtor(1);
    int ok2 = (r2 == &b) && (g_dtorCalls == 1) && (g_deleteCalls == 1) && (b.marker == 0xDEAD);

    // even flag value with bit0 set (3) => delete
    g_dtorCalls = 0; g_deleteCalls = 0;
    CLandscapeBackgroundPatch c; c.marker = 3;
    void* r3 = c.VectorDeletingDtor(3);
    int ok3 = (r3 == &c) && (g_deleteCalls == 1);

    // even flag (2) => no delete
    g_dtorCalls = 0; g_deleteCalls = 0;
    CLandscapeBackgroundPatch d; d.marker = 4;
    d.VectorDeletingDtor(2);
    int ok4 = (g_deleteCalls == 0) && (g_dtorCalls == 1);

    if (ok1 && ok2 && ok3 && ok4)
        printf("VDD_PARITY_OK\n");
    else
        printf("VDD_FAIL o1=%d o2=%d o3=%d o4=%d\n", ok1, ok2, ok3, ok4);
    return (ok1 && ok2 && ok3 && ok4) ? 0 : 1;
}