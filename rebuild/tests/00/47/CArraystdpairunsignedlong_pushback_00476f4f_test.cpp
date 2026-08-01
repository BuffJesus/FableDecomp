#include <cstdio>

// Behavior test for CArray<std::pair<unsigned_long,...> >::push_back @ 0x00476f4f.
//
// source_cpp defines the target as a naked __fastcall (ecx=this) and CALLS the
// two masked callees by PUSHING their arguments onto the stack, relying on the
// callee to clean them (ret 8 / ret 0xC).  The declared `__fastcall` names only
// fix the decorated symbol (@name@N); the actual argument transfer is on the
// stack, callee-cleaned.  So the callee stubs here must also be naked, read
// their args from the stack, record them, and `ret` the matching byte count --
// a plain __fastcall stub (args in ecx/edx, ret 0) would double-unwind the
// pushed args and crash on return.
//
// Insert branch pushes: [ecx+4]=last, then slot  -> [esp+4]=slot, [esp+8]=last ; ret 8
// Append branch pushes: value, fillcount, [ecx+4]=last
//                       -> [esp+4]=last, [esp+8]=fillcount, [esp+0xC]=value ; ret 0xC

static int   g_InsertCalls;
static void* g_InsertSlot;
static void* g_InsertLast;

static int   g_AppendCalls;
static void* g_AppendLast;
static long  g_AppendFillCount;
static void* g_AppendValue;

// Plain C helpers the naked stubs forward the captured args to.
extern "C" void RecordInsert(void* slot, void* last)
{
    ++g_InsertCalls;
    g_InsertSlot = slot;
    g_InsertLast = last;
}

extern "C" void RecordAppend(void* last, long fillcount, void* value)
{
    ++g_AppendCalls;
    g_AppendLast = last;
    g_AppendFillCount = fillcount;
    g_AppendValue = value;
}

// Masked callee A: source pushes (last, slot) then calls @..._A@8; stack-clean 8.
extern "C" __declspec(naked) void __fastcall
CArray_push_back_00476f4f_InsertAt_A(void* /*slot*/, void* /*last*/)
{
    __asm
    {
        mov  eax, dword ptr [esp + 8]   ; last
        mov  ecx, dword ptr [esp + 4]   ; slot
        push eax
        push ecx
        call RecordInsert
        add  esp, 8
        ret  8
    }
}

// Masked callee B: source pushes (value, fillcount, last) then calls @..._B@12; clean 12.
extern "C" __declspec(naked) void __fastcall
CArray_push_back_00476f4f_AppendFill_B(void* /*last*/, long /*fillcount*/, void* /*value*/)
{
    __asm
    {
        mov  eax, dword ptr [esp + 0Ch]  ; value
        mov  edx, dword ptr [esp + 8]    ; fillcount
        mov  ecx, dword ptr [esp + 4]    ; last
        push eax
        push edx
        push ecx
        call RecordAppend
        add  esp, 0Ch
        ret  0Ch
    }
}

struct RangeHdr
{
    char* first;
    char* last;
};

extern "C" void __fastcall
CArray_push_back_00476f4f(void* thisptr, void* edx, long index, void* value);

static void Reset()
{
    g_InsertCalls = 0; g_InsertSlot = 0; g_InsertLast = 0;
    g_AppendCalls = 0; g_AppendLast = 0; g_AppendFillCount = 0; g_AppendValue = 0;
}

int main()
{
    int failures = 0;

    const long STRIDE = 0xc4;
    static char storage[0xc4 * 4];
    int valueSentinel = 0x5151;

    RangeHdr r;
    r.first = &storage[0];
    r.last  = &storage[STRIDE * 4];   // count = 4

    // index (2) < count (4) -> insert at first + 2*stride, last.
    Reset();
    CArray_push_back_00476f4f(&r, 0, 2, &valueSentinel);
    if (g_InsertCalls != 1 ||
        g_AppendCalls != 0 ||
        g_InsertSlot != (void*)(r.first + 2 * STRIDE) ||
        g_InsertLast != (void*)r.last)
    {
        std::printf("insert-path failed slot=%p last=%p ins=%d app=%d\n",
                    g_InsertSlot, g_InsertLast, g_InsertCalls, g_AppendCalls);
        ++failures;
    }

    // index (4) == count (4) -> append (jae), fillcount = 4-4 = 0.
    Reset();
    CArray_push_back_00476f4f(&r, 0, 4, &valueSentinel);
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

    // index (7) > count (4) -> append, fillcount = 7-4 = 3.
    Reset();
    CArray_push_back_00476f4f(&r, 0, 7, &valueSentinel);
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
        std::printf("CARRAY_PUSH_BACK_00476F4F_TEST FAIL count=%d\n", failures);
        return 1;
    }

    std::printf("CARRAY_PUSH_BACK_00476F4F_TEST PASS\n");
    return 0;
}