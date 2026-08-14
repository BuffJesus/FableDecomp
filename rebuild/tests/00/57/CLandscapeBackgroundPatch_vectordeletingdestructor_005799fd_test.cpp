#include <cstdio>

// Behaviour model of the vector deleting destructor.
// Semantics:
//   - always run sub-object destructor on this+4
//   - if (flags & 1) call operator delete on this
//   - return this

static int g_subDtorCalls = 0;
static int g_deleteCalls = 0;
static void* g_lastSubDtorArg = 0;
static void* g_lastDeleteArg = 0;

static void sub_dtor(void* self)
{
    ++g_subDtorCalls;
    g_lastSubDtorArg = self;
}

static void eng_delete(void* p)
{
    ++g_deleteCalls;
    g_lastDeleteArg = p;
}

static void* vecdtor(void* self, unsigned int flags)
{
    sub_dtor((char*)self + 4);
    if (flags & 1)
        eng_delete(self);
    return self;
}

int main()
{
    char storage[64];
    void* obj = (void*)storage;

    // Case 1: flags bit0 clear -> destruct, no delete
    g_subDtorCalls = g_deleteCalls = 0;
    void* r1 = vecdtor(obj, 0);
    bool ok1 = (r1 == obj) && (g_subDtorCalls == 1) &&
               (g_deleteCalls == 0) &&
               (g_lastSubDtorArg == (void*)((char*)obj + 4));

    // Case 2: flags bit0 set -> destruct AND delete
    g_subDtorCalls = g_deleteCalls = 0;
    void* r2 = vecdtor(obj, 1);
    bool ok2 = (r2 == obj) && (g_subDtorCalls == 1) &&
               (g_deleteCalls == 1) &&
               (g_lastDeleteArg == obj);

    // Case 3: higher bits set but bit0 clear -> no delete
    g_subDtorCalls = g_deleteCalls = 0;
    void* r3 = vecdtor(obj, 2);
    bool ok3 = (r3 == obj) && (g_subDtorCalls == 1) && (g_deleteCalls == 0);

    // Case 4: bit0 set with other bits -> delete
    g_subDtorCalls = g_deleteCalls = 0;
    void* r4 = vecdtor(obj, 3);
    bool ok4 = (r4 == obj) && (g_deleteCalls == 1);

    if (ok1 && ok2 && ok3 && ok4)
        printf("VECDTOR_OK\n");
    else
        printf("VECDTOR_FAIL\n");
    return 0;
}