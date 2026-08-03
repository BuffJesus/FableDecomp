#include <stdio.h>

struct Elem { int a; int flag; int b; int c; int d; }; /* 20 bytes */
struct CRenderTarget {
    char pad0[8];
    Elem* arr;              /* +0x8 */
    char pad1[0x24-0xC];
    int  idx;               /* +0x24 */
};

/* Captured argument for the tail-called subroutine. */
static Elem* g_seen = 0;
static int   g_calls = 0;

/* SubTarget is tail-called with ecx = &element (this=ecx). */
extern "C" __declspec(naked) void SubTarget(void)
{
    __asm {
        mov  eax, ecx        /* grab the element (this) pointer */
        mov  g_seen, eax
        inc  g_calls
        ret
    }
}

__declspec(naked) void __fastcall DetachTarget(void)
{
    __asm {
        mov  edx, dword ptr [ecx+0x24]
        push esi
        mov  esi, dword ptr [ecx+0x8]
        lea  ecx, [edx+edx*4]
        xor  eax, eax
        shl  ecx, 2
        cmp  dword ptr [ecx+esi*1+0x4], eax
        je   done
        add  ecx, esi
        pop  esi
        jmp  SubTarget
    done:
        pop  esi
        ret
    }
}

/* Call DetachTarget with this=ecx via a shim. */
static void call_detach(CRenderTarget* self)
{
    __asm {
        mov ecx, self
        call DetachTarget
    }
}

int main(void)
{
    static Elem elems[4];
    CRenderTarget rt;
    rt.arr = elems;

    /* Case 1: index 2, flag != 0  -> SubTarget invoked with &elems[2]. */
    for (int i = 0; i < 4; ++i) { elems[i].a=0; elems[i].flag=0; elems[i].b=0; elems[i].c=0; elems[i].d=0; }
    elems[2].flag = 0x1234;
    rt.idx = 2;
    g_seen = 0; g_calls = 0;
    call_detach(&rt);
    if (g_calls != 1 || g_seen != &elems[2]) { printf("FAIL case1 calls=%d\n", g_calls); return 1; }

    /* Case 2: index 1, flag == 0 -> SubTarget NOT invoked. */
    rt.idx = 1;
    g_seen = 0; g_calls = 0;
    call_detach(&rt);
    if (g_calls != 0) { printf("FAIL case2 calls=%d\n", g_calls); return 1; }

    /* Case 3: different index resolves to the right element pointer (base + idx*20). */
    elems[3].flag = 0x55;
    rt.idx = 3;
    g_seen = 0; g_calls = 0;
    call_detach(&rt);
    if (g_calls != 1 || g_seen != &elems[3]) { printf("FAIL case3 calls=%d\n", g_calls); return 1; }

    printf("OK_0x005bd993\n");
    return 0;
}