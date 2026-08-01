#include <cstdio>

// Masked callees are stack-arg / callee-clean in the retail idiom (the naked
// dispatcher pushes their args and does NOT clean up after the call), so the
// runnable stubs must use __stdcall for the exe to link and run without a
// stack imbalance.  The parity source declares them __fastcall, but the emitted
// call operands are relocation-masked there so convention is irrelevant to the
// byte-match; here we need a convention that actually matches how the asm
// passes the arguments (pushed left-to-right onto the stack, callee cleans).
extern "C" void __stdcall
CArray_push_back_00476123_InsertAt_A(void* slot, void* last);
extern "C" void __stdcall
CArray_push_back_00476123_AppendFill_B(void* last, long fillcount, void* value);

extern "C" __declspec(naked) void __fastcall
CArray_push_back_00476123(void* /*ecx this*/, void* /*edx*/, long /*index*/, void* /*value*/)
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
        push 50h
        cdq
        pop  esi
        idiv esi
        mov  esi, dword ptr [esp + 10h]
        cmp  esi, eax
        jae  append

        lea  eax, [esi + esi*4]
        shl  eax, 4
        push ebx
        add  eax, edi
        push eax
        call CArray_push_back_00476123_InsertAt_A
        jmp  done

    append:
        mov  ebx, dword ptr [ecx + 4]
        push dword ptr [esp + 14h]
        mov  eax, ebx
        sub  eax, edi
        push 50h
        pop  edi
        cdq
        idiv edi
        sub  esi, eax
        push esi
        push ebx
        call CArray_push_back_00476123_AppendFill_B

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
CArray_push_back_00476123_InsertAt_A(void* slot, void* last)
{
    ++g_InsertCalls; g_InsertSlot = slot; g_InsertLast = last;
}
extern "C" void __stdcall
CArray_push_back_00476123_AppendFill_B(void* last, long fillcount, void* value)
{
    ++g_AppendCalls; g_AppendLast = last; g_AppendFillCount = fillcount; g_AppendValue = value;
}

struct RangeHdr { char* first; char* last; };

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
    r.last  = &storage[STRIDE * 4];

    Reset();
    CArray_push_back_00476123(&r, 0, 2, &valueSentinel);
    if (g_InsertCalls != 1 || g_AppendCalls != 0 ||
        g_InsertSlot != (void*)(r.first + 2 * STRIDE) ||
        g_InsertLast != (void*)r.last) {
        std::printf("insert-path failed slot=%p last=%p ins=%d app=%d\n",
                    g_InsertSlot, g_InsertLast, g_InsertCalls, g_AppendCalls);
        ++failures;
    }

    Reset();
    CArray_push_back_00476123(&r, 0, 4, &valueSentinel);
    if (g_AppendCalls != 1 || g_InsertCalls != 0 ||
        g_AppendLast != (void*)r.last || g_AppendFillCount != 0 ||
        g_AppendValue != (void*)&valueSentinel) {
        std::printf("append-eq failed last=%p fc=%ld val=%p ins=%d app=%d\n",
                    g_AppendLast, g_AppendFillCount, g_AppendValue,
                    g_InsertCalls, g_AppendCalls);
        ++failures;
    }

    Reset();
    CArray_push_back_00476123(&r, 0, 7, &valueSentinel);
    if (g_AppendCalls != 1 || g_InsertCalls != 0 ||
        g_AppendLast != (void*)r.last || g_AppendFillCount != 3 ||
        g_AppendValue != (void*)&valueSentinel) {
        std::printf("append-gt failed last=%p fc=%ld val=%p ins=%d app=%d\n",
                    g_AppendLast, g_AppendFillCount, g_AppendValue,
                    g_InsertCalls, g_AppendCalls);
        ++failures;
    }

    if (failures != 0) {
        std::printf("CARRAY_PUSH_BACK_00476123_TEST FAIL count=%d\n", failures);
        return 1;
    }
    std::printf("CARRAY_PUSH_BACK_00476123_TEST PASS\n");
    return 0;
}