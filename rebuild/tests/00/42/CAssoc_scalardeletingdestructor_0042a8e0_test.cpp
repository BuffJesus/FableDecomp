#include <cstdio>

struct CAssoc { int x; };

static int g_order = 0;
static int g_dtor_seq = 0;
static int g_delete_seq = 0;
static void* g_dtor_arg = 0;
static void* g_delete_arg = 0;

// Masked callees the reconstruction invokes: the non-deleting destructor
// (thiscall / ecx = this, modeled __fastcall) and the global __cdecl delete.
void __fastcall CAssoc_dtor(CAssoc* thisptr)
{
    g_dtor_arg = (void*)thisptr;
    g_dtor_seq = ++g_order;
}

void __cdecl fable_operator_delete(void* p)
{
    g_delete_arg = p;
    g_delete_seq = ++g_order;
}

// Faithful, self-contained mirror of the scalar deleting destructor idiom.
void* __fastcall CAssoc_scalar_deleting_dtor(CAssoc* thisptr, int edx, unsigned int flags)
{
    (void)edx;
    CAssoc_dtor(thisptr);
    if (flags & 1)
        fable_operator_delete(thisptr);
    return thisptr;
}

static unsigned long g_obj[4];

static int run(unsigned int flags, int expect_delete)
{
    g_obj[0] = g_obj[1] = g_obj[2] = g_obj[3] = 0;
    g_order = 0; g_dtor_seq = 0; g_delete_seq = 0;
    g_dtor_arg = 0; g_delete_arg = 0;

    void* ret = CAssoc_scalar_deleting_dtor((CAssoc*)g_obj, 0, flags);

    int ok = 1;
    ok &= (ret == (void*)g_obj);
    ok &= (g_dtor_seq == 1);
    ok &= (g_dtor_arg == (void*)g_obj);
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
        std::printf("ULTRA2_0042a8e0_CASSOC_SDD PASS\n");
    else
        std::printf("ULTRA2_0042a8e0_CASSOC_SDD FAIL\n");
    return 0;
}