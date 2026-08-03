#include <cstdio>

static int g_order = 0;
static int g_base_seq = 0;
static int g_delete_seq = 0;
static void* g_base_arg = 0;
static void* g_delete_arg = 0;
static unsigned long g_vptr_at_base_dtor = 0;

static const unsigned long kDerived = 0x01236794u;
static const unsigned long kBase    = 0x0122f83cu;

static unsigned long g_obj[4];

void __fastcall SplitBranch_BaseSubObject_dtor(void* thisptr)
{
    g_base_arg = thisptr;
    g_vptr_at_base_dtor = *(unsigned long*)((char*)thisptr - 8);
    g_base_seq = ++g_order;
}

void __cdecl fable_operator_delete(void* p)
{
    g_delete_arg = p;
    g_delete_seq = ++g_order;
}

void* __fastcall CLandscapeBackgroundSubNodeSplitBranch_scalar_deleting_dtor(
        void* thisptr, int /*edx*/, unsigned int flags)
{
    unsigned long* vfptr = (unsigned long*)thisptr;

    *vfptr = kDerived;
    SplitBranch_BaseSubObject_dtor((char*)thisptr + 8);
    *vfptr = kBase;

    if (flags & 1)
        fable_operator_delete(thisptr);

    return thisptr;
}

static int run(unsigned int flags, int expect_delete)
{
    g_obj[0] = g_obj[1] = g_obj[2] = g_obj[3] = 0;
    g_order = 0; g_base_seq = 0; g_delete_seq = 0;
    g_base_arg = 0; g_delete_arg = 0; g_vptr_at_base_dtor = 0;

    void* ret = CLandscapeBackgroundSubNodeSplitBranch_scalar_deleting_dtor(
        g_obj, 0, flags);

    int ok = 1;
    ok &= (ret == (void*)g_obj);
    ok &= (g_base_seq == 1);
    ok &= (g_base_arg == (void*)((char*)g_obj + 8));
    ok &= (g_vptr_at_base_dtor == kDerived);
    ok &= (g_obj[0] == kBase);
    if (expect_delete)
    {
        ok &= (g_delete_seq == 2);
        ok &= (g_delete_arg == (void*)g_obj);
    }
    else
    {
        ok &= (g_delete_seq == 0);
    }
    return ok;
}

int main()
{
    int ok = 1;
    ok &= run(1u, 1);
    ok &= run(0u, 0);
    ok &= run(2u, 0);
    ok &= run(3u, 1);

    if (ok)
        std::printf("SPLITBRANCH_SCALAR_DELETING_DTOR_00485524_TEST PASS\n");
    else
        std::printf("SPLITBRANCH_SCALAR_DELETING_DTOR_00485524_TEST FAIL\n");
    return 0;
}