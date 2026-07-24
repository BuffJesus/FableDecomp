struct CGameScriptInterface { char pad[0x1e8]; };
__declspec(naked) void __fastcall CGameScriptInterface_GiveHeroTitle(CGameScriptInterface* self, void* edx, int title)
{
    __asm {
        mov edx, dword ptr [esp+4]
        mov eax, dword ptr [ecx]
        push 1
        push -1
        push edx
        call dword ptr [eax+0x1e4]
        ret 4
    }
}