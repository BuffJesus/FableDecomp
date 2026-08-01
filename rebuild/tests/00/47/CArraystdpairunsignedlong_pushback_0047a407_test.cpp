/* Behavior test for CArray_push_back_0047a407 (retail 0x0047a407).
 * Compiles under cl /Od; the target lives in source.obj so the verifier's
 * fallback link (source.obj + test.obj) resolves it.
 *
 * The target is __fastcall (modeled naked-__fastcall): ecx=this, a {first,last}
 * range of 0x50-byte element slots, plus two stack args (index, value ptr).
 * It computes count = (last-first)/0x50 and dispatches:
 *   index <  count : InsertAt_A(first + index*0x50, last)
 *   index >= count : AppendFill_B(last, index - count, value)
 *
 * Both callees are relocation-masked externs whose bodies live here as stubs.
 * The retail body invokes them by PUSHING their arguments and `call`-ing, with
 * NO caller-side cleanup (it falls straight into pop edi/esi/ebx; ret 8), so
 * each callee must clean its own pushed args (callee-cleanup).  We implement the
 * stubs naked -- reading args positionally off the stack and `ret N` to pop them
 * -- while the extern "C" __fastcall declaration keeps the exact decorated names
 * (@..._InsertAt_A@8 / @..._AppendFill_B@12) that source.obj references.
 *
 *   InsertAt_A:  retail pushes ebx(last), eax(slot)
 *                -> [esp+4]=slot [esp+8]=last, ret 8.
 *   AppendFill_B: retail pushes value, esi(fillcount), ebx(last)
 *                -> [esp+4]=last [esp+8]=fillcount [esp+0xC]=value, ret 12.
 */

#include <cstdio>

static int   g_InsertCalls;
static void* g_InsertSlot;
static void* g_InsertLast;

static int   g_AppendCalls;
static void* g_AppendLast;
static long  g_AppendFillCount;
static void* g_AppendValue;

extern "C" __declspec(naked) void __fastcall
CArray_push_back_0047a407_InsertAt_A(void* /*slot*/, void* /*last*/)
{
    __asm
    {
        mov  eax, dword ptr [esp + 4]    ; slot
        mov  dword ptr g_InsertSlot, eax
        mov  eax, dword ptr [esp + 8]    ; last
        mov  dword ptr g_InsertLast, eax
        inc  dword ptr g_InsertCalls
        ret  8
    }
}

extern "C" __declspec(naked) void __fastcall
CArray_push_back_0047a407_AppendFill_B(void* /*last*/, long /*fillcount*/, void* /*value*/)
{
    __asm
    {
        mov  eax, dword ptr [esp + 4]     ; last
        mov  dword ptr g_AppendLast, eax
        mov  eax, dword ptr [esp + 8]     ; fillcount
        mov  dword ptr g_AppendFillCount, eax
        mov  eax, dword ptr [esp + 0Ch]   ; value
        mov  dword ptr g_AppendValue, eax
        inc  dword ptr g_AppendCalls
        ret  12
    }
}

struct RangeHdr
{
    char* first;
    char* last;
};

extern "C" void __fastcall
CArray_push_back_0047a407(void* thisptr, void* edx, long index, void* value);

static void Reset()
{
    g_InsertCalls = 0; g_InsertSlot = 0; g_InsertLast = 0;
    g_AppendCalls = 0; g_AppendLast = 0; g_AppendFillCount = 0; g_AppendValue = 0;
}

int main()
{
    int failures = 0;

    const long STRIDE = 0x50;
    static char storage[0x50 * 4];
    int valueSentinel = 0x5151;

    RangeHdr r;
    r.first = &storage[0];
    r.last  = &storage[STRIDE * 4];     // count = (last-first)/0x50 = 4

    // index 2 < count 4  -> InsertAt(first + 2*0x50, last)
    Reset();
    CArray_push_back_0047a407(&r, 0, 2, &valueSentinel);
    if (g_InsertCalls != 1 ||
        g_AppendCalls != 0 ||
        g_InsertSlot != (void*)(r.first + 2 * STRIDE) ||
        g_InsertLast != (void*)r.last)
    {
        std::printf("insert-path failed slot=%p last=%p ins=%d app=%d\n",
                    g_InsertSlot, g_InsertLast, g_InsertCalls, g_AppendCalls);
        ++failures;
    }

    // index 4 == count 4  -> AppendFill(last, 0, value)
    Reset();
    CArray_push_back_0047a407(&r, 0, 4, &valueSentinel);
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

    // index 7 >  count 4  -> AppendFill(last, 3, value)
    Reset();
    CArray_push_back_0047a407(&r, 0, 7, &valueSentinel);
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
        std::printf("CARRAY_PUSH_BACK_0047A407_TEST FAIL count=%d\n", failures);
        return 1;
    }

    std::printf("CARRAY_PUSH_BACK_0047A407_TEST PASS\n");
    return 0;
}