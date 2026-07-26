// CGameScriptInterface::DisplacementMonochromeEffectColourFadeTo
// __fastcall (this in ecx) + 2 stack params; forwards to an engine
// thiscall on self->mid->p18 (member at +0x1964), callee-cleaned (ret 8).
// Written naked because the harness rewrites __fastcall->__fastcall, which
// cannot express "this in ecx + 3 pure stack args".
struct Inner {
    unsigned char pad[0x1964];
    void* target; // +0x1964
};
struct CMid {
    unsigned char pad[0x18];
    Inner* p18; // +0x18
};
struct CGameScriptInterface {
    void* vt;    // +0x0
    CMid* mid;   // +0x4
};

extern "C" void __fastcall CalleeColourFadeTo(void* thisptr, int a, int b, int c);

void __declspec(naked) __fastcall CGameScriptInterface_DisplacementMonochromeEffectColourFadeTo(
    CGameScriptInterface* self /*ecx*/, int p1, int p2)
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