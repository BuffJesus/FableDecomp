struct CGameScriptInterface { char dummy; };
extern "C" void Callee_4b26b0_c();

__declspec(naked) void __fastcall PostAddScriptedEntities(CGameScriptInterface* self)
{
    __asm {
        _emit 0x8b
        _emit 0x0d
        _emit 0xfc
        _emit 0x89
        _emit 0x3b
        _emit 0x01
        mov eax, dword ptr [ecx+0x88]
        mov edx, dword ptr [eax+8]
        push edx
        call Callee_4b26b0_c
        ret
    }
}