#include <cstdio>
struct Inner {
    unsigned char pad[0x1964];
    void* target;
};
struct CMid {
    unsigned char pad[0x18];
    Inner* p18;
};
struct CGameScriptInterface {
    void* vt;
    CMid* mid;
};
static int g_calls = 0;
static void* g_this; static int g_a, g_b, g_c;
extern "C" void __declspec(naked) CalleeColourFadeTo(void*, int, int, int) {
    __asm {
        mov g_this, ecx
        mov eax, DWORD PTR [esp+0x4]
        mov g_a, eax
        mov eax, DWORD PTR [esp+0x8]
        mov g_b, eax
        mov eax, DWORD PTR [esp+0x0C]
        mov g_c, eax
        inc g_calls
        ret 0x0C
    }
}
void __declspec(naked) __fastcall Fn(CGameScriptInterface* self, int p1, int p2)
{
    __asm {
        mov eax, DWORD PTR [ecx+0x4]
        mov edx, DWORD PTR [esp+0x8]
        mov ecx, DWORD PTR [eax+0x18]
        mov eax, DWORD PTR [esp+0x4]
        mov ecx, DWORD PTR [ecx+0x1964]
        push edx
        push 0x1
        push eax
        call CalleeColourFadeTo
        ret 0x8
    }
}
int main(){
    Inner inr; inr.target = (void*)0xABCD;
    CMid md; md.p18 = &inr;
    CGameScriptInterface gsi; gsi.mid = &md;
    __asm {
        push 0x22
        push 0x11
        lea ecx, gsi
        call Fn
    }
    if (g_calls==1 && g_this==(void*)0xABCD && g_a==0x11 && g_b==1 && g_c==0x22) {
        std::printf("CGameScriptInterface_008901c0_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL calls=%d this=%p a=%d b=%d c=%d\n", g_calls,g_this,g_a,g_b,g_c);
    return 1;
}