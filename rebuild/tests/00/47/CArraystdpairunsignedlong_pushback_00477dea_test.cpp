// Self-contained behavior test for CArray_push_back_00477dea.
//
// The retail body pushes the callee arguments on the stack (right-to-left) and
// does NOT clean them up itself, so the two masked callees must be __stdcall-
// style: they consume the pushed args and balance the stack.  The extern
// declarations in source_cpp mangle them as __fastcall (@name@N), so to keep
// the exact same mangled symbol yet still clean the caller's pushed dwords we
// implement each stub as a naked __fastcall that reads its arguments off the
// stack and returns with the correct `ret N`.
//
// Element stride is 0x88 (136) bytes.  We build a mock {first,last} range with a
// known element count and verify the branch selection:
//   * index < count  -> InsertAt(slot=first+index*0x88, last)
//   * index >= count -> AppendFill(last, fillcount=index-count, value)

#include <cstdio>

struct Range { unsigned char* first; unsigned char* last; };

extern "C" void __fastcall CArray_push_back_00477dea(void* thisp, void* edx, long index, void* value);

// ---- recorded callee arguments ---------------------------------------------
static int   g_insert_called   = 0;
static void* g_insert_slot     = 0;
static void* g_insert_last     = 0;

static int   g_append_called   = 0;
static void* g_append_last     = 0;
static long  g_append_fill     = 0;
static void* g_append_value    = 0;

// The retail body calls these with the arguments PUSHED on the stack (not in
// ecx/edx) and performs no post-call stack cleanup, so each stub must pop the
// pushed dwords itself (ret 8 / ret 0xC).  Declared __fastcall to keep the
// @name@N mangled symbol source.obj references; implemented naked so the actual
// arg passing is stack-based and the stack stays balanced.
extern "C" __declspec(naked) void __fastcall
CArray_push_back_00477dea_InsertAt_A(void* /*slot*/, void* /*last*/)
{
    // stack on entry: [esp+0]=ret, [esp+4]=slot, [esp+8]=last
    __asm
    {
        mov  eax, dword ptr [esp + 4]
        mov  g_insert_slot, eax
        mov  eax, dword ptr [esp + 8]
        mov  g_insert_last, eax
        mov  dword ptr g_insert_called, 1
        ret  8
    }
}

extern "C" __declspec(naked) void __fastcall
CArray_push_back_00477dea_AppendFill_B(void* /*last*/, long /*fillcount*/, void* /*value*/)
{
    // stack on entry: [esp+0]=ret, [esp+4]=last, [esp+8]=fillcount, [esp+0xC]=value
    __asm
    {
        mov  eax, dword ptr [esp + 4]
        mov  g_append_last, eax
        mov  eax, dword ptr [esp + 8]
        mov  g_append_fill, eax
        mov  eax, dword ptr [esp + 0Ch]
        mov  g_append_value, eax
        mov  dword ptr g_append_called, 1
        ret  0Ch
    }
}

int main()
{
    const long STRIDE = 0x88;
    const long COUNT  = 4;

    static unsigned char buffer[STRIDE * 16];
    Range r;
    r.first = buffer;
    r.last  = buffer + STRIDE * COUNT;   // 4 elements populated

    int value = 0x1234;

    // --- Case 1: index < count -> InsertAt --------------------------------
    g_insert_called = g_append_called = 0;
    long idx1 = 2;
    CArray_push_back_00477dea(&r, 0, idx1, &value);

    if (!g_insert_called || g_append_called) {
        std::printf("FAIL branch1\n");
        return 1;
    }
    if (g_insert_slot != (void*)(r.first + idx1 * STRIDE)) {
        std::printf("FAIL slot\n");
        return 1;
    }
    if (g_insert_last != (void*)r.last) {
        std::printf("FAIL last1\n");
        return 1;
    }

    // --- Case 2: index >= count -> AppendFill -----------------------------
    g_insert_called = g_append_called = 0;
    long idx2 = 7;
    CArray_push_back_00477dea(&r, 0, idx2, &value);

    if (g_insert_called || !g_append_called) {
        std::printf("FAIL branch2\n");
        return 1;
    }
    if (g_append_last != (void*)r.last) {
        std::printf("FAIL last2\n");
        return 1;
    }
    if (g_append_fill != (idx2 - COUNT)) {
        std::printf("FAIL fill\n");
        return 1;
    }
    if (g_append_value != (void*)&value) {
        std::printf("FAIL value\n");
        return 1;
    }

    std::printf("PUSH_BACK_PAIR_STRIDE88_TEST PASS\n");
    return 0;
}