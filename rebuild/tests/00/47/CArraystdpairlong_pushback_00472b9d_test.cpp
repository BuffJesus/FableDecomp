/* Behavior test for push_back_00472b9d (retail 0x00472B9D).
 *
 * The retail body dispatches to two masked container helpers using a
 * CALLEE-CLEANS (stdcall-style) sequence: it pushes the args and does NO
 * post-call esp adjustment, then pop edi / pop esi / ret 8. So the real
 * EraseTail (2 args) is ret 8 and AppendFill (3 args) is ret 0Ch.
 *
 * source_cpp declares them `extern "C" __cdecl`, which only fixes the SYMBOL
 * NAME (_push_back_00472b9d_EraseTail / _AppendFill, no @N suffix). We must
 * therefore DEFINE the stubs as naked functions that keep that cdecl name yet
 * clean their own stack (ret 8 / ret 0Ch) so esp balances on return -- the same
 * trick the landed _Fill_n_0047b6c0 test uses for its Sub_520060 stub.
 */

#include <stdio.h>
#include <string.h>

extern "C" void __fastcall
push_back_00472b9d(void* self, void*, long count, const void* value);

struct Array
{
    char* begin;
    char* end;
};

static int   g_erase_count;
static void* g_erase_first;
static void* g_erase_last;

static int         g_append_count;
static void*       g_append_dst;
static long        g_append_grow;
static const void* g_append_value;

/* EraseTail(first, last): 2 stack args -> callee cleans 8 (ret 8). */
extern "C" __declspec(naked) void push_back_00472b9d_EraseTail(void* /*first*/, void* /*last*/)
{
    __asm
    {
        mov  eax, [esp + 4]        ; first
        mov  g_erase_first, eax
        mov  eax, [esp + 8]        ; last
        mov  g_erase_last, eax
        inc  g_erase_count
        ret  8
    }
}

/* AppendFill(dst, grow, value): 3 stack args -> callee cleans 12 (ret 0Ch). */
extern "C" __declspec(naked) void push_back_00472b9d_AppendFill(void* /*dst*/, long /*grow*/, const void* /*value*/)
{
    __asm
    {
        mov  eax, [esp + 4]        ; dst
        mov  g_append_dst, eax
        mov  eax, [esp + 8]        ; grow
        mov  g_append_grow, eax
        mov  eax, [esp + 0Ch]      ; value
        mov  g_append_value, eax
        inc  g_append_count
        ret  0Ch
    }
}

static void Reset()
{
    g_erase_count = 0;
    g_erase_first = 0;
    g_erase_last = 0;
    g_append_count = 0;
    g_append_dst = 0;
    g_append_grow = 0;
    g_append_value = 0;
}

static int Check(int cond, const char* msg)
{
    if (!cond)
    {
        printf("FAIL: %s\n", msg);
        return 0;
    }
    return 1;
}

int main()
{
    int ok = 1;
    Array arr;
    char storage[0x40 * 6];
    long value;

    memset(storage, 0, sizeof(storage));
    value = 0x1234;

    /* size = 4 elements (end - begin = 4 * 0x40) */
    arr.begin = storage;
    arr.end = storage + 0x40 * 4;

    /* count (2) < size (4) -> EraseTail(begin + 2*0x40, end) */
    Reset();
    push_back_00472b9d(&arr, 0, 2, &value);
    ok &= Check(g_erase_count == 1 && g_append_count == 0,
                "shrink path must call EraseTail");
    ok &= Check(g_erase_first == storage + 0x40 * 2,
                "EraseTail first must be begin + count*stride");
    ok &= Check(g_erase_last == storage + 0x40 * 4,
                "EraseTail last must be end");

    /* count (7) >= size (4) -> AppendFill(end, 7-4, value) */
    Reset();
    push_back_00472b9d(&arr, 0, 7, &value);
    ok &= Check(g_append_count == 1 && g_erase_count == 0,
                "grow path must call AppendFill");
    ok &= Check(g_append_dst == storage + 0x40 * 4,
                "AppendFill dst must be end");
    ok &= Check(g_append_grow == 3,
                "AppendFill grow must be count - size");
    ok &= Check(g_append_value == &value,
                "AppendFill must receive the value pointer");

    /* count (4) == size (4) -> append (jae) with grow 0 */
    Reset();
    push_back_00472b9d(&arr, 0, 4, &value);
    ok &= Check(g_append_count == 1 && g_erase_count == 0,
                "equal path must call AppendFill (jae)");
    ok &= Check(g_append_grow == 0,
                "AppendFill grow must be 0 when count == size");

    if (!ok)
        return 1;

    puts("PUSH_BACK_PAIR_LONG_00472B9D_TEST PASS");
    return 0;
}