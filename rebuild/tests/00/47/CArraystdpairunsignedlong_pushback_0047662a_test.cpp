#include <cstdio>

// Self-contained behaviour test for CArray<std::pair<unsigned_long,...> >::push_back
// @ 0x0047662a.  The residue verifier links THIS object file ALONE, so the test
// must define the target function AND its (masked) callees itself.
//
// The target body below is a byte-for-byte copy of the landed naked-__fastcall
// source; it is NOT the parity artifact (that lives in source_cpp) -- it is only
// here so the standalone link resolves and the two dispatch branches can run.
//
// The retail callees receive their operands on the STACK (the target `push`es
// them and cleans nothing itself for them), so the stubs are declared __stdcall
// (callee-cleans-its-own-stack-args).  Argument order matches the push order:
//   InsertAt : push last; push slot; call  -> (slot, last)
//   AppendFill: push value; ...; push count; push last; call -> (last, count, value)
// Declaring them __fastcall instead would leave the pushed dwords un-popped and
// corrupt the stack on return -- so __stdcall is load-bearing here.

extern "C" void __stdcall CArray_push_back_0047662a_InsertAt_A(void* slot, void* last);
extern "C" void __stdcall CArray_push_back_0047662a_AppendFill_B(void* last, long fillcount, void* value);

extern "C" __declspec(naked) void __fastcall
CArray_push_back_0047662a(void* /*ecx this*/, void* /*edx*/, long /*index*/, void* /*value*/)
{
    __asm
    {
        push ebx
        mov  ebx, dword ptr [ecx + 4]
        push esi
        push edi
        mov  edi, dword ptr [ecx]
        mov  eax, ebx
        sub  eax, edi
        push 5Ch
        cdq
        pop  esi
        idiv esi
        mov  esi, dword ptr [esp + 10h]
        cmp  esi, eax
        jae  append

        imul esi, esi, 5Ch
        push ebx
        add  esi, edi
        push esi
        call CArray_push_back_0047662a_InsertAt_A
        jmp  done

    append:
        mov  ebx, dword ptr [ecx + 4]
        push dword ptr [esp + 14h]
        mov  eax, ebx
        sub  eax, edi
        push 5Ch
        pop  edi
        cdq
        idiv edi
        sub  esi, eax
        push esi
        push ebx
        call CArray_push_back_0047662a_AppendFill_B

    done:
        pop  edi
        pop  esi
        pop  ebx
        ret  8
    }
}

static int   g_InsertCalls;
static void* g_InsertSlot;
static void* g_InsertLast;

static int   g_AppendCalls;
static void* g_AppendLast;
static long  g_AppendFillCount;
static void* g_AppendValue;

extern "C" void __stdcall
CArray_push_back_0047662a_InsertAt_A(void* slot, void* last)
{
    ++g_InsertCalls;
    g_InsertSlot = slot;
    g_InsertLast = last;
}

extern "C" void __stdcall
CArray_push_back_0047662a_AppendFill_B(void* last, long fillcount, void* value)
{
    ++g_AppendCalls;
    g_AppendLast = last;
    g_AppendFillCount = fillcount;
    g_AppendValue = value;
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

    const long STRIDE = 0x5c;
    static char storage[0x5c * 4];
    int valueSentinel = 0x5151;

    RangeHdr r;
    r.first = &storage[0];
    r.last  = &storage[STRIDE * 4];

    // count = (last-first)/0x5c = 4.

    // index 2 < count(4) -> INSERT: slot = first + 2*0x5c, last = r.last.
    Reset();
    CArray_push_back_0047662a(&r, 0, 2, &valueSentinel);
    if (g_InsertCalls != 1 ||
        g_AppendCalls != 0 ||
        g_InsertSlot != (void*)(r.first + 2 * STRIDE) ||
        g_InsertLast != (void*)r.last)
    {
        std::printf("insert-path failed slot=%p last=%p ins=%d app=%d\n",
                    g_InsertSlot, g_InsertLast, g_InsertCalls, g_AppendCalls);
        ++failures;
    }

    // index 4 == count(4) -> APPEND (jae): last=r.last, fill = 4-4 = 0, value=&sentinel.
    Reset();
    CArray_push_back_0047662a(&r, 0, 4, &valueSentinel);
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

    // index 7 > count(4) -> APPEND: fill = 7-4 = 3.
    Reset();
    CArray_push_back_0047662a(&r, 0, 7, &valueSentinel);
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
        std::printf("CARRAY_PUSH_BACK_0047662A_TEST FAIL count=%d\n", failures);
        return 1;
    }

    std::printf("CARRAY_PUSH_BACK_0047662A_TEST PASS\n");
    return 0;
}