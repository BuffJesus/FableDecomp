/* Behavior test for CArray_push_back_00479ff3 (retail 0x00479ff3).
 *
 * Self-contained (no #include of the source .cpp): the target function lives in
 * source.obj; the verifier links source.obj + test.obj together, so this file
 * must ONLY forward-declare the target and DEFINE the two masked callees the
 * source references.  Including the source here would (a) not resolve the
 * relative path from the verifier work dir and (b) duplicate the target symbol.
 *
 * Target is a naked __fastcall (ecx=this, edx unused) with two stack args
 * (index, value); ret 8.  It computes count = (last-first)/0x48 and dispatches:
 *   index <  count : InsertAt_A(slot = first + index*0x48, last)
 *   index >= count : AppendFill_B(last, fillcount = index - count, value)
 *
 * The two callees are called with STACK-pushed args (naked hand-written pushes),
 * not register __fastcall args, so the stubs below are themselves naked and read
 * their arguments off the stack, then `ret` the exact pushed byte count:
 *   InsertAt_A : caller pushes [last][slot]        -> [esp+4]=slot [esp+8]=last , ret 8
 *   AppendFill_B: caller pushes [value][fill][last]-> [esp+4]=last [esp+8]=fill
 *                                                     [esp+0Ch]=value           , ret 0Ch
 */

#include <cstdio>

static int   g_InsertCalls;
static void* g_InsertSlot;
static void* g_InsertLast;

static int   g_AppendCalls;
static void* g_AppendLast;
static long  g_AppendFillCount;
static void* g_AppendValue;

/* Masked callee A: stub reads (slot, last) off the caller-pushed stack. */
extern "C" __declspec(naked) void __fastcall
CArray_push_back_00479ff3_InsertAt_A(void* /*slot*/, void* /*last*/)
{
    __asm
    {
        mov  eax, [esp + 4]        ; slot
        mov  g_InsertSlot, eax
        mov  eax, [esp + 8]        ; last
        mov  g_InsertLast, eax
        inc  g_InsertCalls
        ret  8                     ; pop the two pushed dwords
    }
}

/* Masked callee B: stub reads (last, fillcount, value) off the pushed stack. */
extern "C" __declspec(naked) void __fastcall
CArray_push_back_00479ff3_AppendFill_B(void* /*last*/, long /*fillcount*/, void* /*value*/)
{
    __asm
    {
        mov  eax, [esp + 4]        ; last
        mov  g_AppendLast, eax
        mov  eax, [esp + 8]        ; fillcount
        mov  g_AppendFillCount, eax
        mov  eax, [esp + 0Ch]      ; value
        mov  g_AppendValue, eax
        inc  g_AppendCalls
        ret  0Ch                   ; pop the three pushed dwords
    }
}

struct RangeHdr
{
    char* first;
    char* last;
};

extern "C" void __fastcall
CArray_push_back_00479ff3(void* thisptr, void* edx, long index, void* value);

static void Reset()
{
    g_InsertCalls = 0; g_InsertSlot = 0; g_InsertLast = 0;
    g_AppendCalls = 0; g_AppendLast = 0; g_AppendFillCount = 0; g_AppendValue = 0;
}

int main()
{
    int failures = 0;

    const long STRIDE = 0x48;
    static char storage[0x48 * 4];
    int valueSentinel = 0x5151;

    RangeHdr r;
    r.first = &storage[0];
    r.last  = &storage[STRIDE * 4];   /* count = (last-first)/0x48 == 4 */

    /* index 2 < count 4 -> insert path, slot = first + 2*stride */
    Reset();
    CArray_push_back_00479ff3(&r, 0, 2, &valueSentinel);
    if (g_InsertCalls != 1 ||
        g_AppendCalls != 0 ||
        g_InsertSlot != (void*)(r.first + 2 * STRIDE) ||
        g_InsertLast != (void*)r.last)
    {
        std::printf("insert-path failed slot=%p last=%p ins=%d app=%d\n",
                    g_InsertSlot, g_InsertLast, g_InsertCalls, g_AppendCalls);
        ++failures;
    }

    /* index 4 == count 4 -> append path (jae), fillcount = 0 */
    Reset();
    CArray_push_back_00479ff3(&r, 0, 4, &valueSentinel);
    if (g_AppendCalls != 1 ||
        g_InsertCalls != 0 ||
        g_AppendLast != (void*)r.last ||
        g_AppendFillCount != 0 ||
        g_AppendValue != (void*)&valueSentinel)
    {
        std::printf("append-eq failed last=%p fc=%ld val=%p ins=%d app=%d\n",
                    g_AppendLast, g_AppendFillCount, g_AppendValue,
                    g_InsertCalls, g_AppendCalls);
        ++failures;
    }

    /* index 7 > count 4 -> append path, fillcount = 3 */
    Reset();
    CArray_push_back_00479ff3(&r, 0, 7, &valueSentinel);
    if (g_AppendCalls != 1 ||
        g_InsertCalls != 0 ||
        g_AppendLast != (void*)r.last ||
        g_AppendFillCount != 3 ||
        g_AppendValue != (void*)&valueSentinel)
    {
        std::printf("append-gt failed last=%p fc=%ld val=%p ins=%d app=%d\n",
                    g_AppendLast, g_AppendFillCount, g_AppendValue,
                    g_InsertCalls, g_AppendCalls);
        ++failures;
    }

    if (failures != 0)
    {
        std::printf("CARRAY_PUSH_BACK_00479FF3_TEST FAIL count=%d\n", failures);
        return 1;
    }

    std::printf("CARRAY_PUSH_BACK_00479FF3_TEST PASS\n");
    return 0;
}