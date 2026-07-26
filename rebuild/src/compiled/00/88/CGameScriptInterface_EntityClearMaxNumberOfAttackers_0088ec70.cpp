struct CGameScriptInterface { unsigned char* vt; };
__declspec(naked) void __fastcall CGameScriptInterface_EntityClearMaxNumberOfAttackers(CGameScriptInterface* self, void* edx_ignore, int entity)
{
    __asm {
        mov edx, dword ptr [esp+4]
        mov eax, dword ptr [ecx]
        push -1
        push edx
        call dword ptr [eax+0x73c]
        ret 4
    }
}