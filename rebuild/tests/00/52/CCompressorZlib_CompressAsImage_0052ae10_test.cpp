
#include <stdio.h>

// ---- object under test ----
struct Obj;
typedef int (*Slot5)(Obj* self, int a1, int a2, int a3, int a4, int a8);

// captured call record
static Obj*  g_self  = 0;
static int   g_a[5];
static int   g_hits  = 0;

struct Obj {
    void** vtbl;
};

// virtual slot 5 target (index 0x14/4 = 5): __fastcall signature.
// It receives this in ecx; the forwarder pushed 5 dword args (arg8,arg4,arg3,arg2,arg1)
// in push order, so on stack after retaddr they appear:
//   [esp+4]=arg1,[esp+8]=arg2,[esp+0xc]=arg3,[esp+0x10]=arg4,[esp+0x14]=arg8
// (last pushed = arg1 is nearest esp). We capture via a __fastcall-style function.
int __fastcall Slot5Impl(Obj* self, int /*edx unused*/, int arg1, int arg2, int arg3, int arg4, int arg8)
{
    g_self = self;
    g_a[0]=arg1; g_a[1]=arg2; g_a[2]=arg3; g_a[3]=arg4; g_a[4]=arg8;
    g_hits++;
    return 0xBEEF;
}

// function under test (naked asm forwarder)
__declspec(naked) void CompressAsImage()
{
    __asm {
        mov edx, dword ptr [esp+0x20]
        mov eax, dword ptr [ecx]
        push edx
        mov edx, dword ptr [esp+0x14]
        push edx
        mov edx, dword ptr [esp+0x14]
        push edx
        mov edx, dword ptr [esp+0x14]
        push edx
        mov edx, dword ptr [esp+0x14]
        push edx
        call dword ptr [eax+0x14]
        ret 0x20
    }
}

int main()
{
    // build vtable: slot 5 (index 5, byte offset 0x14) -> Slot5Impl
    static void* vt[8] = {0,0,0,0,0,0,0,0};
    vt[5] = (void*)&Slot5Impl;

    Obj obj;
    obj.vtbl = vt;

    // args a1..a8
    int a1=11,a2=22,a3=33,a4=44,a5=55,a6=66,a7=77,a8=88;
    int ret = 0;
    Obj* pobj = &obj;

    // Invoke: this=ecx, then 8 stack args (a1..a8) pushed right-to-left.
    // Forwarder does ret 0x20 (cleans 32 bytes = 8 args), __fastcall/stdcall-like.
    __asm {
        push a8
        push a7
        push a6
        push a5
        push a4
        push a3
        push a2
        push a1
        mov ecx, pobj
        call CompressAsImage
        mov ret, eax
    }

    if (g_hits != 1) { printf("FAIL hits=%d\n", g_hits); return 1; }
    if (g_self != pobj) { printf("FAIL self\n"); return 1; }
    // forwarded args must be (arg1,arg2,arg3,arg4,arg8) = (11,22,33,44,88)
    if (g_a[0]!=11 || g_a[1]!=22 || g_a[2]!=33 || g_a[3]!=44 || g_a[4]!=88) {
        printf("FAIL args %d %d %d %d %d\n", g_a[0],g_a[1],g_a[2],g_a[3],g_a[4]);
        return 1;
    }
    if (ret != 0xBEEF) { printf("FAIL ret=%x\n", ret); return 1; }
    printf("OK_0x0052ae10 args=%d,%d,%d,%d,%d ret=%x\n", g_a[0],g_a[1],g_a[2],g_a[3],g_a[4], ret);
    return 0;
}