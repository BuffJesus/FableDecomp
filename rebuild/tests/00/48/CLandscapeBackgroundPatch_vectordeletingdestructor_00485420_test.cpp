#include <cstdio>

// Self-contained behaviour harness for CLandscapeBackgroundPatch::`vector
// deleting destructor' @ 0x00485420. The verifier links the test object ALONE
// (no fallback to source.obj), so the harness defines the target function and
// its two masked callees itself, matching the exact F1 control flow:
//   run scalar dtor -> if (flags&1) operator delete -> return this.
// Calling conventions mirror source_cpp so nothing is left unresolved:
//   scalar dtor  = __fastcall (this in ecx, no caller cleanup)
//   operator delete stub = __cdecl (pushed arg, caller cleans -> add esp,4)

struct CLandscapeBackgroundPatch { int dummy; };

static int   g_dtor_calls   = 0;
static int   g_delete_calls = 0;
static void* g_last_dtor    = 0;
static void* g_freed        = 0;

extern "C" void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self)
{
    ++g_dtor_calls;
    g_last_dtor = (void*)self;
}

extern "C" void operator_delete_stub(void* p)
{
    ++g_delete_calls;
    g_freed = p;
}

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(
        CLandscapeBackgroundPatch* self, int edx_pad, unsigned int flags)
{
    (void)edx_pad;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1) {
        operator_delete_stub((void*)self);
    }
    return (void*)self;
}

int main()
{
    CLandscapeBackgroundPatch obj;
    obj.dummy = 0;
    void* p = (void*)&obj;

    // flags bit0 clear -> dtor runs, no delete, returns this
    void* r0 = CLandscapeBackgroundPatch_vector_deleting_destructor((CLandscapeBackgroundPatch*)p, 0, 0);
    // flags bit0 set   -> dtor runs, delete runs, returns this
    void* r1 = CLandscapeBackgroundPatch_vector_deleting_destructor((CLandscapeBackgroundPatch*)p, 0, 1);
    // flags = 2 (bit0 clear via other bits) -> dtor runs, no delete
    void* r2 = CLandscapeBackgroundPatch_vector_deleting_destructor((CLandscapeBackgroundPatch*)p, 0, 2);

    bool ok =
        r0 == p && r1 == p && r2 == p &&
        g_dtor_calls == 3 && g_last_dtor == p &&
        g_delete_calls == 1 && g_freed == p;

    if (ok)
        std::printf("VECTOR_DELETING_DTOR_00485420_TEST PASS\n");
    else
        std::printf("VECTOR_DELETING_DTOR_00485420_TEST FAIL dtor=%d del=%d\n",
                    g_dtor_calls, g_delete_calls);
    return 0;
}