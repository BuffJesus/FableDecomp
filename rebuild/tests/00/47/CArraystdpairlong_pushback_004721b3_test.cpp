#include <cstdio>

// Probe state for the two masked callees.
static int   g_InsertCalls;
static void* g_InsertSlot;
static void* g_InsertLast;

static int   g_AppendCalls;
static void* g_AppendLast;
static long  g_AppendFillCount;
static void* g_AppendValue;

// The retail body invokes both callees with a hand-rolled `push` sequence and a
// plain `call @name@N` referencing the __fastcall-mangled symbols.  Despite the
// __fastcall mangling the naked caller passes EVERY argument on the stack (it
// never loads ecx/edx for the callee), and it relies on the callee to pop those
// pushed bytes (matching a stdcall-style `ret N`).  So the stubs must also be
// naked: read the args off the stack and `ret N` to keep the stack balanced.
// A normal __fastcall stub would read ecx/edx (garbage) and `ret 0`, corrupting
// the stack and crashing on return.  The `@name@N` mangling is preserved by
// declaring them extern "C" __fastcall.
//
// InsertAt_A: caller does `push esi(last); push eax(slot); call` ->
//   [esp+4]=slot, [esp+8]=last ; cleans 8 bytes.
// AppendFill_B: caller does `push [value]; push eax(fill); push esi(last); call` ->
//   [esp+4]=last, [esp+8]=fill, [esp+0Ch]=value ; cleans 12 bytes.

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

extern "C" __declspec(naked) void __fastcall
CArray_push_back_004721b3_InsertAt_A(void* /*slot*/, void* /*last*/)
{
    __asm
    {
        mov  eax, dword ptr [esp + 8]   ; last
        push eax
        mov  eax, dword ptr [esp + 8]   ; slot (esp shifted by the push above)
        push eax
        call RecordInsert               ; __cdecl helper
        add  esp, 8
        ret  8
    }
}

extern "C" __declspec(naked) void __fastcall
CArray_push_back_004721b3_AppendFill_B(void* /*last*/, long /*fillcount*/, void* /*value*/)
{
    __asm
    {
        mov  eax, dword ptr [esp + 0Ch] ; value
        push eax
        mov  eax, dword ptr [esp + 0Ch] ; fillcount
        push eax
        mov  eax, dword ptr [esp + 0Ch] ; last
        push eax
        call RecordAppend               ; __cdecl helper
        add  esp, 12
        ret  12
    }
}

// Layout matching the retail {first,last} 4-byte-stride range.
struct RangeHdr
{
    long* first;
    long* last;
};

extern "C" void __fastcall
CArray_push_back_004721b3(void* thisptr, void* edx, long index, void* value);

static void Reset()
{
    g_InsertCalls = 0; g_InsertSlot = 0; g_InsertLast = 0;
    g_AppendCalls = 0; g_AppendLast = 0; g_AppendFillCount = 0; g_AppendValue = 0;
}

int main()
{
    int failures = 0;

    long storage[4] = { 10, 20, 30, 40 };
    int  valueSentinel = 0x5151;

    RangeHdr r;
    r.first = &storage[0];
    r.last  = &storage[4];            // count = (16 bytes)>>2 = 4

    // index < count : InsertAt(first + index*4, last)
    Reset();
    CArray_push_back_004721b3(&r, 0, 2, &valueSentinel);
    if (g_InsertCalls != 1 ||
        g_AppendCalls != 0 ||
        g_InsertSlot != (void*)(r.first + 2) ||
        g_InsertLast != (void*)r.last)
    {
        std::printf("insert-path failed slot=%p last=%p ins=%d app=%d\n",
                    g_InsertSlot, g_InsertLast, g_InsertCalls, g_AppendCalls);
        ++failures;
    }

    // index == count : append path, fillcount = index - count = 0
    Reset();
    CArray_push_back_004721b3(&r, 0, 4, &valueSentinel);
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
    CArray_push_back_004721b3(&r, 0, 7, &valueSentinel);
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
        std::printf("CARRAY_PUSH_BACK_004721B3_TEST FAIL count=%d\n", failures);
        return 1;
    }

    std::printf("CARRAY_PUSH_BACK_004721B3_TEST PASS\n");
    return 0;
}