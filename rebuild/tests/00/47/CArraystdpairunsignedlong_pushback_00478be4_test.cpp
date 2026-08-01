#include <cstdio>

// Behaviour test for the byte-parity target CArray_push_back_00478be4 (defined
// in source.obj as extern "C" __fastcall / naked).  The verifier links test.obj
// alone first and, on the unresolved-external, falls back to linking
// source.obj + test.obj together.  So:
//   * declare (do NOT redefine) the target with the exact extern "C" __fastcall
//     linkage source.obj uses -> mangled name @CArray_push_back_00478be4@16.
//   * DEFINE the two masked callees the target references so the combined link
//     resolves with no duplicate symbols.
//
// CRITICAL: the retail body reaches its callees by PUSHing their arguments and
// relying on the callee to pop them (retail InsertAt/AppendFill are same-class
// members -> callee-cleans-stack, like __stdcall/__fastcall).  A plain
// extern "C" __fastcall(void*,void*) stub takes its args in ecx/edx and cleans
// 0 stack bytes, so the pushed dwords leak and the target's epilogue underflows
// -> access violation.  We must keep the __fastcall MANGLED NAME (so the link
// resolves) but consume the pushed stack args ourselves.  We therefore write
// the stubs as naked functions that read the pushed dwords off the stack,
// record them, and ret 8 / ret 12 -- balancing the stack exactly as the retail
// callees do.  Only eax/ecx/edx are touched (caller-saved); ebx/esi/edi/ebp are
// left untouched so the target's pop edi/esi/ebx and the caller frame survive.

extern "C" void __fastcall
CArray_push_back_00478be4(void* thisptr, void* edx, long index, void* value);

static int   g_InsertCalls;
static void* g_InsertSlot;
static void* g_InsertLast;

static int   g_AppendCalls;
static void* g_AppendLast;
static long  g_AppendFillCount;
static void* g_AppendValue;

// Retail insert-path push order: push ebx(last); push eax(slot); call.
// Naked entry: [esp+4]=slot (pushed last), [esp+8]=last.  ret 8.
extern "C" __declspec(naked) void __fastcall
CArray_push_back_00478be4_InsertAt_A(void* /*slot*/, void* /*last*/)
{
    __asm
    {
        mov  eax, dword ptr [esp + 4]   ; slot
        mov  ecx, dword ptr [esp + 8]   ; last
        mov  g_InsertSlot, eax
        mov  g_InsertLast, ecx
        mov  eax, g_InsertCalls
        add  eax, 1
        mov  g_InsertCalls, eax
        ret  8
    }
}

// Retail append-path push order: push [value]; push esi(count); push ebx(last); call.
// Naked entry: [esp+4]=last, [esp+8]=count, [esp+0xC]=value.  ret 12.
extern "C" __declspec(naked) void __fastcall
CArray_push_back_00478be4_AppendFill_B(void* /*last*/, long /*fillcount*/, void* /*value*/)
{
    __asm
    {
        mov  eax, dword ptr [esp + 4]    ; last
        mov  ecx, dword ptr [esp + 8]    ; fillcount
        mov  edx, dword ptr [esp + 0Ch]  ; value
        mov  g_AppendLast, eax
        mov  g_AppendFillCount, ecx
        mov  g_AppendValue, edx
        mov  eax, g_AppendCalls
        add  eax, 1
        mov  g_AppendCalls, eax
        ret  0Ch
    }
}

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

    const long STRIDE = 0x48;
    static char storage[0x48 * 4];
    int valueSentinel = 0x5151;

    RangeHdr r;
    r.first = &storage[0];
    r.last  = &storage[STRIDE * 4];   // count = (last-first)/0x48 == 4

    // index < count -> InsertAt(first + index*0x48, last)
    Reset();
    CArray_push_back_00478be4(&r, 0, 2, &valueSentinel);
    if (g_InsertCalls != 1 ||
        g_AppendCalls != 0 ||
        g_InsertSlot != (void*)(r.first + 2 * STRIDE) ||
        g_InsertLast != (void*)r.last)
    {
        std::printf("insert-path failed slot=%p last=%p ins=%d app=%d\n",
                    g_InsertSlot, g_InsertLast, g_InsertCalls, g_AppendCalls);
        ++failures;
    }

    // index == count -> AppendFill(last, index-count == 0, value)
    Reset();
    CArray_push_back_00478be4(&r, 0, 4, &valueSentinel);
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

    // index > count -> AppendFill(last, index-count == 3, value)
    Reset();
    CArray_push_back_00478be4(&r, 0, 7, &valueSentinel);
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
        std::printf("CARRAY_PUSH_BACK_00478BE4_TEST FAIL count=%d\n", failures);
        return 1;
    }

    std::printf("CARRAY_PUSH_BACK_00478BE4_TEST PASS\n");
    return 0;
}