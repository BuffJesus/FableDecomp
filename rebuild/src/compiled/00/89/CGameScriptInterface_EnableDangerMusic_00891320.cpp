extern "C" void EnableDangerMusic_Target();
struct CGameScriptInterface { int placeholder; };
__declspec(naked) void __fastcall CGameScriptInterface_EnableDangerMusic(CGameScriptInterface* self)
{
    (void)self;
    __asm {
        mov ecx, [ecx+4]
        mov ecx, [ecx+0x6c]
        jmp EnableDangerMusic_Target
    }
}