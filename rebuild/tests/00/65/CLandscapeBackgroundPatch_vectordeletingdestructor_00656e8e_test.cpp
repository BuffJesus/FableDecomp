#include <stdio.h>

// Behaviour model of the vector deleting destructor:
//  - always runs the member subobject destructor at this+0x10
//  - frees 'this' iff (flags & 1)
//  - returns 'this'

static int g_subDtorCalls = 0;
static int g_deleteCalls  = 0;
static void* g_lastDeleted = 0;

struct SubObj { int x; };

struct CLandscapeBackgroundPatch {
    char   pad0[0x10];
    SubObj sub;
};

static void SubObj_dtor(SubObj* self) { (void)self; ++g_subDtorCalls; }
static void eng_operator_delete(void* p) { ++g_deleteCalls; g_lastDeleted = p; }

static void* model(CLandscapeBackgroundPatch* self, unsigned int flags)
{
    SubObj_dtor(&self->sub);
    if (flags & 1)
        eng_operator_delete(self);
    return self;
}

int main(void)
{
    CLandscapeBackgroundPatch obj;

    // flags=0 -> dtor runs, no delete, returns self
    g_subDtorCalls = g_deleteCalls = 0; g_lastDeleted = 0;
    void* r0 = model(&obj, 0);
    if (r0 != &obj) { printf("FAIL ret0\n"); return 1; }
    if (g_subDtorCalls != 1) { printf("FAIL dtor0\n"); return 1; }
    if (g_deleteCalls != 0) { printf("FAIL del0\n"); return 1; }

    // flags=1 -> dtor runs AND delete(self), returns self
    g_subDtorCalls = g_deleteCalls = 0; g_lastDeleted = 0;
    void* r1 = model(&obj, 1);
    if (r1 != &obj) { printf("FAIL ret1\n"); return 1; }
    if (g_subDtorCalls != 1) { printf("FAIL dtor1\n"); return 1; }
    if (g_deleteCalls != 1) { printf("FAIL del1\n"); return 1; }
    if (g_lastDeleted != &obj) { printf("FAIL delptr\n"); return 1; }

    // only low bit matters
    g_subDtorCalls = g_deleteCalls = 0;
    model(&obj, 2);
    if (g_deleteCalls != 0) { printf("FAIL delbit\n"); return 1; }
    g_subDtorCalls = g_deleteCalls = 0;
    model(&obj, 3);
    if (g_deleteCalls != 1) { printf("FAIL delbit3\n"); return 1; }

    printf("VDD_OK\n");
    return 0;
}