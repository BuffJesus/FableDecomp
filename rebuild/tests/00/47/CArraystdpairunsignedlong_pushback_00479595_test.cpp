#include <cstdio>

// Behaviour test for retail push_back @ 0x00479595.
//
// The target function is DEFINED in source_cpp (compiled to source.obj) and is
// only DECLARED here with the exact extern "C" __fastcall linkage it exports
// (@CArray_push_back_00479595@16); we call it.  The test-only link leaves the
// target unresolved, so the verifier falls back to linking source.obj+test.obj.
//
// The retail body dispatches to two callees by PUSHING their arguments on the
// stack (the container `this` stays in ecx) and performing NO post-call stack
// cleanup -- i.e. stack-passed args with callee-cleanup (ret 8 / ret 0Ch).  The
// source declares those callees extern "C" __fastcall, so their symbols decorate
// as @..._InsertAt_A@8 and @..._AppendFill_B@12.  We DEFINE matching naked shims
// that read the pushed stack args, record them, and clean their own args, so the
// combined link resolves with no unresolved externals and no ABI mismatch.

extern "C" void __fastcall
CArray_push_back_00479595(void* thisptr, void* edx, long index, void* value);

// Probe state for the two masked callees.
static int   g_InsertCalls;
static void* g_InsertSlot;
static void* g_InsertLast;

static int   g_AppendCalls;
static void* g_AppendLast;
static long  g_AppendFillCount;
static void* g_AppendValue;

static void RecordInsert(void* slot, void* last)
{
    ++g_InsertCalls;
    g_InsertSlot = slot;
    g_InsertLast = last;
}

static void RecordAppend(void* last, long fillcount, void* value)
{
    ++g_AppendCalls;
    g_AppendLast = last;
    g_AppendFillCount = fillcount;
    g_AppendValue = value;
}

// InsertAt(slot, last): retail pushes last then slot -> arg0=slot, arg1=last;
// callee cleans 8 bytes (ret 8).
extern "C" __declspec(naked) void __fastcall
CArray_push_back_00479595_InsertAt_A(void* /*slot*/, void* /*last*/)
{
    __asm
    {
        mov  eax, dword ptr [esp + 8]    // last
        push eax
        mov  eax, dword ptr [esp + 8]    // slot (shifted by the push above)
        push eax
        call RecordInsert
        add  esp, 8
        ret  8
    }
}

// AppendFill(last, fillcount, value): retail pushes value, fillcount, last ->
// arg0=last, arg1=fillcount, arg2=value; callee cleans 12 bytes (ret 0Ch).
extern "C" __declspec(naked) void __fastcall
CArray_push_back_00479595_AppendFill_B(void* /*last*/, long /*fillcount*/, void* /*value*/)
{
    __asm
    {
        mov  eax, dword ptr [esp + 12]   // value
        push eax
        mov  eax, dword ptr [esp + 12]   // fillcount
        push eax
        mov  eax, dword ptr [esp + 12]   // last
        push eax
        call RecordAppend
        add  esp, 12
        ret  12
    }
}

// Layout matching the retail {first,last} 0x50-byte-stride range.
struct RangeHdr
{
    char* first;
    char* last;
};

static void Reset()
{
    g_InsertCalls = 0; g_InsertSlot = 0; g_InsertLast = 0;
    g_AppendCalls = 0; g_AppendLast = 0; g_AppendFillCount = 0; g_AppendValue = 0;
}

int main()
{
    int failures = 0;

    const long STRIDE = 0x50;
    static char storage[0x50 * 8];    // room for 8 elements
    int valueSentinel = 0x5959;

    RangeHdr r;
    r.first = &storage[0];
    r.last  = &storage[STRIDE * 4];   // count = (4*0x50)/0x50 = 4

    // index < count : InsertAt(first + index*0x50, last)
    Reset();
    CArray_push_back_00479595(&r, 0, 2, &valueSentinel);
    if (g_InsertCalls != 1 ||
        g_AppendCalls != 0 ||
        g_InsertSlot != (void*)(r.first + 2 * STRIDE) ||
        g_InsertLast != (void*)r.last)
    {
        std::printf("insert-path failed slot=%p last=%p ins=%d app=%d\n",
                    g_InsertSlot, g_InsertLast, g_InsertCalls, g_AppendCalls);
        ++failures;
    }

    // index == count : append path, fillcount = index - count = 0
    Reset();
    CArray_push_back_00479595(&r, 0, 4, &valueSentinel);
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

    // index > count : append path, fillcount = index - count = 3
    Reset();
    CArray_push_back_00479595(&r, 0, 7, &valueSentinel);
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
        std::printf("CARRAY_PUSH_BACK_00479595_TEST FAIL count=%d\n", failures);
        return 1;
    }

    std::printf("CARRAY_PUSH_BACK_00479595_TEST PASS\n");
    return 0;
}